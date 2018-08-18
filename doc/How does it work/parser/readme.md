# How does it work ?

Suppose a syntax such as Arlia's (roughly, a mixture between ML-like and C-like).
And suppose we already have a token suite all with a lexical identity.

Example :

```
var foo : integer = 8 + 3;
```

Here are the lexems:

|  tokens |         id        |
|:-------:|:-----------------:|
|   var   |      VARIABLE     |
|   foo   |     IDENTIFIER    |
|    :    | INSTANCING_OBJECT |
| integer |     IDENTIFIER    |
|    =    |    ASSIGNATION    |
|    8    |       NUMBER      |
|    +    |      ADDITION     |
|    3    |       NUMBER      |
|    ;    |      ENDLINE      |

That we can represent as it is:

```
{
    Program:
  [
      ["var": VARIABLE],
      ["foo": IDENTIFIER],
      [":":INSTANCING_OBJECT],
      ["integer": IDENTIFIER],
      ["=": ASSIGNATION],
      ["8": NUMBER],
      ["+": ADDITION],
      ["3": NUMBER],
      [";": ENFLINE]
  ]
}
```

The parser will have to build an AST that will look like this:

```
       [EXPR]
         |
 [VAR_DECLARATION]
         |
        (=)
       /   \
      /     \
     (:)    (+)
    /   \   / \
   /     \ 8   3 
 foo  integer  
```

We see therefore that we declare a variable whose left values determine the nature of the right values, indeed, we create a variable 'foo' of type 'integer' which has as value `8+3`

### How did parser recognize that expression?

He gets, in entry, the expression we saw above. He recognizes the first lexeme (var), so he knows what to expect for the future. It scrolls through it until it finds a lexical identity of value assignment:

```
var identifier = rvalue;
```

If there is none, it looks for a type assignment lexical identity :

```
var identifier : rtype;
```

If there is none, then it is a statement that will look like this :

```
var identifier;
```

If the expression is affected by a value (=), it separates the left and right values :

```
left:
    identifier
node:
    =
right:
    rvalue
```

or:

```
left:
    identifier : rtype
node:
    =
right:
    rvalue
```

If the expression on the left is assigned by a type, the same schema is created:

```
left:
    left:
        identifier
    node:
        :
    right:
        rtype
node:
    =
right:
    rvalue
```

Now, only the rvalue can contain other expressions:

```
8 + 3
```

So we have a new node:

```
left:
    "8"
node:
    "+"
right:
   "3"
```

And so on.

Here is what this would give to our initial expression (`var foo : integer = 8 + 3`):

```
left:
    left:
        "foo"
    node:
        ":"
    right:
        "integer"
node:
    "="
right:
    left:
        "8"
    node:
        "+"
    right:
        "3"
```

To ensure the validity of the temporary tree, there are of course token verification phases.

In other words, we get this tree (whose main node is the sign '='):

```
             "="
           /    \
         ":"    "+"
        /  \    / \
       /    \ "8" "3"  
    "foo"  "integer"
```

The parser, once the temporary ast is generated, adds it to the final tree where it specifies what type of expression it is:

```
          [EXP]
            |
  [VARIABLE_DECLARATION]
{          ...           }
```

To generate the assembler code, the final ast is traversed node by node :

```
[EXP1] { ... }, -> [EXP2] { ... }, -> [EXP3] { ... }
```

And specialized functions recovers the data in the tree to build a data encapsulation structure:

```C++
struct VariableDecl {
    std::string identifier;
    type_t type;            // type as object (ex: integer | with all its properties)
    expr_t value;           // value as expression (ex: 8 + 3)
    adrs_t adress;          // memory adress for asm (ex: [esp-4])
};

typedef VariableDecl variable_t;

```

Each time a new variable is declared, its model (so structure above) is defined and added to a backup array:

```
std::vector<variable_t> ListOfVariables;
```

# How does the AST work?

The parser calls the `Lexer::next()` function which returns the next token of the stream of the file to be parsed. Even before designing the AST, several things can happen:

 - Importing a file (via recursion, the parser uses itself to parse the called file) ;
 - A preprocessor definition (a subparer takes care of the preprocessor) ;
 - token errors (unknown token for example (/!\ there is no multi-pass: anything that is used before being declared (idem for the preprocessor) will be considered an error)) ;


After which, if the token being processed is recognized by the compiler as a statement, it sends the rest of the expression definition (= to the end of the line or an obvious delimiter token) to the corresponding AST generation class, example :

```Csharp
var
```

the parser recognizes that it is a variable declaration, and therefore sends the rest of the line to the corresponding AST class:

```Csharp
var foo = 8;
```

```
AST::VarDecl;
```

The syntax analysis then begins:

```
AST::name → "foo" LL_identifier → "__foo"
```
```
AST::type → "undefined"
```
```
AST::VarAssignationExpr → "8" IntLiteral → "8" size → 4 bytes
```

And will form the following tree:

```
Program
`-VarDecl foo 4
 `-IntegerLiteral 8 4
```

By convention, the tree functions as such:

```
Program
`-STATEMENT INFO
 `-EXPRESSION INFO
```

depending on whether it is a statement, definition, assignment or other, the information will change.

### What is an identifier?

The program considers the name of a variable/function/object as an identifier to identify it and make it unique.

By default, any object declared outside any code block is identified as unique. Example :

```
var MyVar = 8;
```

`MyVar` will have as identifier:

```
__MyVar
```

Another example:

```
namespace foo;
var MyVar = 8;
```

Here, MyVar will have an identifier corresponding to the namespace in which it is contained:

```
____foo__MyVar
```

This is more useful when using functions:

```
namespace foo;
func addition(a, b);
```

will give this:

```
____foo__addition
```

It is this identifier that is used to declare the labels in the corresponding assembler code:

```asm
____foo__addition:
    ...
```

The same goes for objects. Here is a complete example:

```
namespace dio;

object display {
    static func write(text);
    static func writeln(text);
}

implement display.write {
    extern PUTS;
    puts(text)
}

implement display.writeln {
    display.write(text);
}

namespace;

func main() {
    @dio:display.writeln("Hello, world!");
}

```

Give:

```asm
____dio___display__write:
    push ebp
    mov ebp, esp
    test edi
    jz _excpt_null ; null value exception
    call puts
    pop ebp
    ret

____dio___display__writeln:
    push ebp
    mov ebp, esp
    test edi
    jz _excpt_null ; null value exception
    call ____dio___display__write
    pop ebp
    ret

__main:
    push ebp
    mov ebp, esp
    mov edi, GV0 ; GV0 is the string "Hello, world!" as memory pointer
    call ____dio___display__writeln
    pop ebp
    ret
```

As you can see, the identifiers are unique, and allow the assembler code to be represented more efficiently.
