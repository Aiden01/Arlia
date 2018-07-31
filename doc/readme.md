# The Arlia programming language
## What is Arlia in general terms?

Arlia is a new multi-paradigm programming language which wants to be object-oriented, in fact, Arlia wants above all to be object-oriented according to a definition close to the original. By using Arlia, many concepts from most languages are present, but the language makes them either simpler to use, or more complex by allowing a more abstract (= easier, but without costs), adapted, and powerful use. The object-oriented paradigm allows each manipulable value to be a potential object; allowing the developer to use the language in the best possible way. The language is at the same time very close to the machine, but also high level (other than in C++ or Rust for example), which allows to have a control on all during specific projects without worrying about memory, dynamic allocation or too complicated pointers. The language is also fast and efficient at runtime, as it rarely wastes resources (especially when the user or library allows it). It is also important to specify that Arlia is a very customizable language, which can, in a certain time, confuse its use, but this "weakness" is also a strength that makes development faster, more efficient and maintainable on large projects. It is a language that also has an integrated exception system (= present in the executable itself); this system does not slow down performance, and does not greatly increase the size of the executable. Whenever code is at risk of causing an error, depending on its level of dangerousness (= harmful for the whole program or not), the integrated system takes over and displays a message bypassing the error or tries again with other parameters. If you have knowledge in a language close to C-like, that you know object oriented and functional paradigm, then Arlia is a language that you will have no trouble learning at this level.

## Is Arlia a complicated language?

The answer is simple: only if the developer makes the language become one. That is, if you don't follow the optional language guideline, the developer moves further away from the path to an optimal program. However, taking other paths than the one proposed can be an advantage and an asset in the case of large projects. To conclude, no, the language is not complicated, but also more or less complex and efficient.

## On which platforms can Arlia be deployed?

For the first version of the language, only under Windows. But it is expected that the language will be extended SSSO (= Same Source Same Output) on Windows, Linux and Mac.

___

# Documentation
## Hello, world!
```
import dio;

func main() {
    @dio:display.writeln("Hello, world!");
}

```
As you can see it, the language to keep the principle of main function. The dio library offers a complete set of display, stream, input and output functionality. For more information about this library refer to the "dio library" page.

## Comments
Language has a double tag commenting system: 
```
` Hello, I'm a comment! `
```

## Standard types
In Arlia, a type represents above all the place that the value(s) used will take in memory. To make it simpler: there is no type of standard in Arlia. Not even char, int, doubles, ....

### How are the variables typed in this case?

It's simple, there are 4 ways:
 - The type inference ;
 - Suffixes to values ;
 - A digital type ;
 - A custom type.

### The type inference
If no type is defined for a variable, but a value has been assigned to it, the compiler then determines the size that this value will take in memory, and thus assigns it a size/"type" :

| Compiler types names   | size (bytes) |
|:----------------------:|:------------:|
|         byte           |       1      |
|         word           |       2      |
|         dword          |       4      |
|         fword          |       6      |
|         qword          |       8      |
|         tword          |      10      |
|        dqword          |      16      |
|        qqword          |      32      |
|        dqqword         |      64      |

Obviously, this table is given as an example, none of these values are explicitly found in the language.

As a general rule, the first 5 types are often used.

Here is an example:

```
var MyVar = 7;
```

Here, the compiler will determine that this variable will have a size of 2 bytes, therefore of type "word".

### Suffixes to values
A suffix is a letter at the end of a value (always numeric) that tells the compiler its size: 

| Suffixs | size (bits) | Name correspondance |
|:-------:|:-----------:|:-------------------:|
|    .b   |      1      |       boolean       |
|    .c   |      1      |         char        |
|    .i   |      2      |       integer       |
|    .s   |      4      |        string       |
|    .f   |      8      |        float        |
|    .r   |      16     |         real        |

Example:

```
var MyBool = 1b;

var MyChar = 97c;

var MyInteger = 32i;

var MyString = 33s;

var MyFloat = 83f;

var MyReal = 89r;
```

Note: a variable cannot change type along the way.

### A digital type
Now that you know a little more about typing and memory sizes, this part should not be complicated to understand. The numerical type of a variable will represent its size in memory. If you define a numerical type of 2, it is exactly the same as defining a suffix variable.i Just refer to the table above.

Example:

```
var MyInteger : 2 = 32;
```

Why then allow this typing if suffixes already allow it?
It is simple: suffixes are on the one hand higher level, and on the other hand, do not allow to use sizes higher than 16 bytes.

## A custom type
Custom types will be seen later in this documentation, but we can already see the types created by the APIL library (Arlia Pre Integrated Library), usable throughout a project, and favored previous typing:

| Types names | size (bits) |
|:-----------:|:-----------:|
|   boolean   |      1      |
|     char    |      1      |
|   integer   |      2      |
|    string   |      4      |
|    float    |      8      |
|     real    |      16     |

Example:

```
var MyBool : boolean = true;

var MyChar : char = 'a';

var MyInteger : integer = 32;

var MyString : string = "Hey!";

var MyFloat : float = 34.0328;

var MyReal : real = 89;
```

## Arrays and containers
An array, in Arlia, is a variable having several indexed values and the same type. It is important to note that it is not the type that is structured, but the variable. In many languages, we can see a syntax similar to this one:

```csharp
// C#
int[] MyArray = new int[n];
```
However, in Arlia, the correspondence would be this:
```
var MyArray[n] : integer;
```

And in no case this:

```
var MyArray : integer[n];
```

(Unless the language is modified by user)

Arlia array are all potentially dynamic in size if not defined, example :

```
var MyDynamicArray[] : integer;

var MyStaticArray[4] : integer;
```

To add a value to an array of a defined size:

```
MyStaticArray[index] = value;
```
If the index is smaller or larger than the defined size, there is an error.

To add a value to an array of dynamic size: 

```
MyDynamicArray += value;
```

To set an array value to an array of dynamic size: 

```
MyDynamicArray = AnArray;
```

We could also modify the language to allow an add method to be added to the array object, but we will see that in the related section.

To make it simpler, the APIL library offers container types:

| Containers names |        templates arguments       |
|:----------------:|:--------------------------------:|
|       array      |         \<typename, size>         |
|       list       |            \<typename>            |
|    dictionary    | \<TypenameOfKey, TypenameOfValue> |
|       stack      |     \<typename, max = NOLIMIT>    |

 - array : Is identical to a normal painting but with some methods and a defined size:
 ```
 var MyArray : array<integer, 9>;
 ```

 - list : Is a more manageable container with dynamic size and can insert elements in addition to adding and removing elements (same as std::vector in C++):

 ```
 var MyList : list<integer>;
 ```

 - dictionary: Is a dictionary that at each key has a value:

 ```
 var MyDico : dictionary<integer, string>;
 ```

 - stack: A stack is a container to which one can only add values and have access with an optinal maximum value. But a stack can't be changed, just deleted:

 ```
 var MyStack : stack<integer, 99>;
 ```

More explanations about these containers in the dedicated page.

## Conditions
Conditions are used to check whether a value meets a condition. If ```true```, then we continue, if ```false```, then we stop the conditional block:

```
if (true) {
    ...
} elif (true) {
    ...
} else {
    ...
}
```

Note 1: If there is only one instruction, braces are not necessary:

```
if (true)
    ...
elif (true)
    ...
else
    ...
```

or:

```
if (true)  ...
elif (true) ...
else ...
```

Note 2: parentheses are not mandatory in this type of Arlia expression (loops included), unless you decide not to put braces because there is only one:

```
if true {
    ...
} elif true {
    ...
} else {
    ...
}
```

And, obviously, you manage the syntax as you see fit:

```
if (true)
{
    ...
}
elif (true)
{
    ...
} else
{
    ...
}
```

...


### Operators of conditions :

| Symboles |           Explication           |
|:--------:|:-------------------------------:|
|    ==    |        is equal to value?       |
|    !=    |      is different to value?     |
|     !    |        Opposite to value        |
|    is    |      is of the value type?      |
|   isnt   |    is not of the value type?    |
|    &&    |               and               |
|    \|\|  |                or               |
|     >    |      is grater than value?      |
|     <    |      is smaller than value?     |
|    >=    |  is grater or equal than value? |
|    <=    | is smaller or equal than value? |
|   true   |           always true           |
|   false  |           always false          |

```is``` and ```isnt``` are value type check operators:

```
var test : char = 'k';

if (test is char) {
    ` Ok, test is a char `
}
```
```
var test : char = 'k';

if (test isnt boolean) {
    ` Ok, test is not a boolean `
}
```

We can do the same to compare the types of two variables:

```
var one : char = 'k';
var two : char = 'p';

if (one is two) {
    ` Ok, one and two have the same type `
}
```
```
var one : char = 'k';
var two : boolean = true;

if (one isnt two) {
    ` Ok, one and two have not the same type `
}
```

We can also use these operators outside conditional blocks:

```
var test : boolean = (true || false);       ` -> true `

var test : boolean = (4 > 6);               ` -> false `
```

To check if a variable is empty, we can also do this:

```
var test;

if (!&test) {
    ` Ok, the "test" variable is empty `
}
```

Here, we just check the presence of the value in the memory via the pointers.

The language also offers a "switch/case" structure of correspondence thanks to the ```match``` keyword:

```
match (n) {
    case (0) ...
    case (1) ...
    case (2) ...
    case (3) ...
    ...
    default ...
}
```

```break``` does not exist in Arlia, as soon as the expression to a correspondent, it does not search any more.

As with if/elif/else syntax, parentheses are only mandatory if there is more than one statement after, except for ```default```.

If we want to match ```n``` to condition operators, we can create an image of ```n``` :

```
match (nbr --> n) {
    case (nbr > 0) ...
    case (nbr < 5) ...
    ...
    default ...
}
```

We'll see "images" again later.

We can also note that ```n``` can be anything: a number, a character, an enumeration value, a character string, a type, ...

## Loops
There are three loop statements in Arlia:

| Statements |
|:----------:|
|    while   |
| do - while |
|     for    |

The ```until``` loop is not present in Arlia, because it is only an imitable variant of the ```while``` loop.

The ```while``` and ```do - while``` loops don't change from the C-like :

```
while (condition) {
    ` instructions `
}


do {
    ` instructions `
} while (condition);

```

The ```for``` loop, on the other hand, has evolved a lot:

|      Types of for loop      |
|:---------------------------:|
| index; condition; iteration |
|         index; max;         |
|    index; max; iteration    |
|         index; array        |

### index; condition; iteration:
It's the lambda ```for``` loop:

```
for (i : integer = 0; i < 10; ++i) {
    ...
}
```

### index; max:
This is a more abstract variant of the lambda ```for``` loop:

```
for (i : integer = 0 to 10) {
    ...
}
```

### index; max; iteration:
It is the same as above but allows to define the value of iterations :

```
for (i : integer = 0 to 10 step 2) {
    ...
}
```

Here, there will be 5 iterations since i will be worth this each turn:

```
i = 0
i = 2
i = 4
i = 6
i = 8
i = 10 -> exit loop
```

The value of iterations can be float, but be careful to reach the goal!

```
for (i : float = 0 to 10 step 0.5) {
    ...
}
```

20 iterations here.

But :

```
for (i : float = 0 to 10 step 0.7) {
    ...
}
```

```0.7``` not being a multiple of 10, the loop will be infinite.

### index; array:
Here, we just iterate from an array :

```
for (i : integer in { 34, 76, 23, 87 }) {
    ...
}
```

There will be as many iterations as there are values in the array, but the index will take the currently indexed value from the array:

```
i = 34
i = 76
i = 23
i = 87
```

And the array can, of course, be a variable:

```
var MyArray[] : integer = { 34, 76, 23, 87 };

for (i : integer in MyArray) {
    ...
}
```

## The ```each``` and ```to``` keyword with iteration
Arlia allows a complete management of arrays thanks, in one way, to loops, and in another way, to iterative keywords :

### ```each```

This keyword is used to iterate or represent an array:

```
for each (chr : char in "Hello, world!") {
    ...
}
```

chr will be equal to :

```
chr = 'H'
chr = 'e'
chr = 'l'
chr = 'l'
chr = 'o'
chr = ','
chr = ' '
chr = 'w'
chr = 'o'
chr = 'r'
chr = 'l'
chr = 'd'
chr = '!'
```

But we can also use it to condition each value in an array:

```
if each (chr : char in text == 'x') {
    ...
}
```

So here we check if each value of an array string (here ```text```) is equal to' x'.

And we can do the same for the ```while``` loop:

```
while each (chr : char in text != 'x') {
    ...
}
```

But we can also use each to fill in an array:

```
var MyArray[] = each item in text;
```

or

```
var MyArray[] = each (item in text);
```

But it's completely stupid to do that, indeed, we can do this instead to avoid performance and speed losses:

```
var MyArray[] = text;
```

### ```to```
This keyword allows to create anonymous arrays, and/or to define the value of a suite :

```
var MyArray[] = 0 to 10;
```

instead of doing:

```
var MyArray[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
```

We can also condition this suite:

```
var MySuite[] = (i : integer = 0 to 100 | i % 2 == 0);
```

Here, we will therefore have all numbers multiple of two, and only those up to 100 :

```
var MySuite[] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30, ..., 100 };
```

Basically, ```to``` create an anonymous array that starts at n and ends at m at which it is possible to condition the values thanks to the operator | ("with").

This also allows you to create substring quickly and without going through a loops:

```
var MyString : string = "My name is Bob !";
var MySubstring : string = (MyString[10] to MyString[13]);
```

(Parentheses are obviously not mandatory, but it is to better synthesize).

Which gives us here:

```
MySubstring = "Bob";
```

## The ```|``` ("with") operator
This operator, as seen above, provides additional packaging information. It is more useful when you want to generate a sequence of numbers when creating a table, as seen above.

Examples:

```
var foo = (n | n > 0 && n < 10);
```

Here, it is exactly as if we were conditioning ```n``` before assigning it to the ```foo``` variable:

```
if (n > 0 && n < 10)
    foo = n;
```

If the condition (in both cases) is not met, then foo simply has no value. This can cause errors if it should have been his first assignment, but can be an advantage in terms of space and visibility.

Example, the Fibonacci suite with ```|``` operator:

```
var fibs[] = (n = 0 to 100 | n <= 1 ? n = 1 : n = (fibs[n - 1] + fibs[n - 2]));
```

(We use here the expression of [ternary](https://en.wikipedia.org/wiki/%3F:) conditioning present in most languages).

The result would be equivalent to writing this:

```
var fibs[];

for (n --> nbr = 0 to 100) {
    if (n <= 1)
        n = 1;
    else
        n = (fibs[n - 1] + fibs[n - 2]);
    fibs += n;
}
```

You will notice that the ```to``` instruction iterates constantly, even if its index is modified along the way. But since we are in a ```for``` loop, the index cannot be changed without changing the behavior of iterations. This is why we create an index image with each iteration.

You will also notice that expression 1 is shorter to write, but just as readable and comprehensible as expressino 2.

### Exit a loop

To exit a loop directly, the ```continue``` instruction is sufficient:

```
while (true) {
    ...
    continue;
    ...
}

`. <-- you are here now`
```

## Ternary condition
A ternary condition is a way to assign a value according to a condition, and all this in the same expression thanks to a specific syntax :

```
(condition) ? ([value if true] : [value if false])
```

Example:

```
var foo : boolean = (a > b ? true : false);
```

If ```a``` is actually greater than ```b```, then ```foo``` will have a ```true``` value, otherwise ```foo``` will have a ```false``` value.

Another example:

```
var str : string = (a != b ? "Yes" : "No");
```

If ```a``` is actually different than ```b```, then ```foo``` will have ```"Yes"``` value, otherwise ```foo``` will have ```"No"``` value.


It's very simple, and it makes writing faster. We can also combine this technique with others, as seen above with ```to```.

## Language operators
Here are the Arlia language operators:

| Operators | Explanation             |
|:---------:|:-----------------------:|
|     +     |          add            |
|     -     |          sub            |
|     *     |          mult           |
|     /     |          div            |
|     %     |          mod            |
|     **    |          pow            |
|     ++    |          inc            |
|     --    |          dec            |
|     +=    |        add over         |
|     -=    |        sub over         |
|     *=    |       mult over         |
|     /=    |        div over         |
|     %=    |        mod over         |
|    **=    |        pow over         |
|     ^     |         concat          |
|     ^=    |      concat over        |
|     ^+    |     sum of arrays       |
|     ^-    |  difference of arrays   |
|     ^*    |   product of arrays     |
|     ^/    |   quotient of arrays    |
|     ^%    |    modulo of arrays     |
|    ^**    |    power of arrays      |
|     \|    |          with           |
|     ?     |   ternary condition     |
|     :     |     call | between      |
|     >     |      greater than       |
|     <     |      smaller than       |
|     >=    | greater or equal than   |
|     <=    | smaller or equal than   |
|     &&    |          and            |
|     \|\|  |           or            |
|     ==    |       is equal ?        |
|     !=    |     is not equal ?      |
|     !     |        opposite         | 
|     is    |        is type          |
|    isnt   |      is not type        |
|    -->    |      image              |

You already know the vast majority, drawn from other languages, as well as from this documentation.

## Concatenation

In most languages, to concatenate two arrays or strings, either a function, a method, or a loop is used. In Arlia, only one operator is needed. There are also derivatives that allow the sum of the values of two arrays in a single move, and so on (+, -, *, /, **, %). Here are these operators:

| Operators |        Explanation      |
|:---------:|:-----------------------:|
|     ^     |         concat          |
|     ^=    |      concat over        |
|     ^+    |     sum of arrays       |
|     ^-    |  difference of arrays   |
|     ^*    |   product of arrays     |
|     ^/    |   quotient of arrays    |
|     ^%    |    modulo of arrays     |
|    ^**    |    power of arrays      |

The concatenation symbol is the circumflex accent (```^```):

```
var One[] = { 5, 6, 7, 8 };
var Two[] = { 5, 4, 3, 2 };

var Sum[] = One ^+ Two;
```

```sum``` will then equal:

```
sum = { 10, 10, 10, 10 }
```

If we just want to add a array to another array:

```
var One[] = { 5, 6, 7, 8 };
var Two[] = { 5, 4, 3, 2 };

Two ^= One;
```

```Two``` will then equal:

```
Two = { 5, 4, 3, 2, 5, 6, 7, 8 };
```

The principle is simple, it is identical for the rest of the concatenation operators.

If you try to perform a concatenation operation on a array of non-numeric values, you may cause errors. In this case, make sure that the array meets your needs before performing the operation.

## Functions
There are two distinct types of functions in Arlia, one is "basic", while the other is more special and drawn from the functional paradigm.

### ```func```

Here is the prototype of a function:

```
func name(args) : type {
    ` instructions `
}
```

A function may not have a type, the first. If this is the case, then this function may not return values (void in C-like), but if it returns a value, then the function will take the implicit type of the returned value. This means that it is the first value returned from a non-typical function that determines its type. That way, you don't get confused:

```
func Addition(a : integer, b : integer) : float {
    return a + b;
}
```

```
func Addition(a, b) {
    return a + b;
}
```

A function can take optional parameters:

```
func foo(a : integer, b : integer, c : boolean = true) {
    ...
}
```

Here, parameter ```c``` is optional since a potential value is already assigned to it.

A function, in Arlia, can also take an infinity of parameters:

```
func foo(arg : integer...) {
    ...
}
```

To process this:

```
func foo(args : integer...) {
    for each (arg in args) {
        ...
    }
}
```

Just like an array.

Note also that a function can explicitly return an array (i.e. it will necessarily return an array):

```
func ReturnArray(text : string)[] : char {
    return each chr : char in text;
}

func main() {
    var EachChar[] : char = ReturnArray("Hello");
}

```

As you can see, the keyword each creates an anonymous array that will be returned from the function.

### ```ret```
ret can have several types of use. First of all, know that a ret expression is a non-modifiable expression ("constate") for the developer, but whose value can evolve throughout the program. Here are the prototypes:

Constant:

```
ret name = value;
```

Here, the value is always constant and nothing can change it.

Constant mutable:

```
ret name = str.size();
```

Here, the value cannot be modified by the developer, but can potentially change value during program execution. As you can indeed see, ```str.size()``` can be different with each call, the value will change according to that.

And finally, ret can take parameters. In a way, it is a constant variable with potential parameters :

```
ret add(x, y) = x + y;
```

We can compare this behavior to that of a language from the ML family:

```ocaml
let add x y = x + y;;
```

With the ready difference that ret has as value only an expression:

```
ret foo() = ...;
            ...;
            ...;
            ...;
```

This will not work.

## Exceptions
As said in the introduction, the language has an embedded exception system. This means that even if you don't try to catch an exception, it can be thrown.
However, you can customize these exceptions:

```
throw "Hello, I'm an exception!";
...
try {
    ...
} catch (msg : string) {
    ...
}
```

Concrete example:

```
func divide(x : integer, y : integer) : float {
    if (y == 0) {
        throw "Can't divide by zero!";
    }
    return (x / y);
}
```

In this case, if y is actually equal to 0, and the exception thrown by the function is caught, the function is interrupted:

```
func divide(x : integer, y : integer) : float {
    if (y == 0) {
        throw "Can't divide by zero!";
    }
    return (x / y);
}

func main() {
    try {
        divide(7, 0);
    } catch (msg) {
        @dio:display.writeln(msg);
    }
}
```

This exception as example being so simple, you don't even need to declare it since the embedded language system supports it:

```
func divide(x : integer, y : integer) : float {
    return (x / y);
}

func main() {
    divide(7, 0);
}
```

The generated assembler code for the divide function will be the following:

```asm
divide:
    push ebp
    mov ebp, esp
    mov DWORD [epb-2], edi
    pop edi
    mov DWORD [ebp-4], esi
    pop esi
    test [ebp-4]
    jz _ExcptDivByZero
    div [epb-2], [ebp-4]
    mov ebx, [ebp-2]
    push esp
    mov esp, ebp
    pop ebp
```

This code is robust and manages a potential division by zero.
Most of the time, your exceptions will be more in the order of "customizing" the program, but can still be very useful.

## Enum
An enumeration is simply a way to declare several constants in the same sequence, like the C-like:

```
enum fruits {
    APPLE,
    BANANA,
    ORANGE,
    PEACH
}
```

There's nothing else special compared to the C-like.

## Structure and encapsulation
A structure is a way to encapsulate several objects in order to structure them:

```
structure MyStruct {
    var number : real;
    var str : string;
    var bool : boolean;
}
```

Be aware that if the structure is not rendered as a "type", then this same structure will be unique :

```
structure MyStruct {
    var number : real;
    var str : string;
    var bool : boolean;
}

MyStruct.number = 98;
MyStruct.str = "Hey!";
MyStruct.bool = false;
```

To make this structure instantiable, we only have to use the keyword ```data```:

```
structure MyStruct {
    var number : real;
    var str : string;
    var bool : boolean;
}

data MyStruct : MyStruct;

var struct : MyStruct;

struct.number = 98;
struct.str = "Hey!";
struct.bool = false;
```

Be careful, don't misunderstand too much about the keyword ```data```, although it is very similar to the keyword ```typedef``` in C, this one allows to create instances of objects who inherits the members of the structure..

Concrete example:

```
structure Hero {
    var HP : integer = 100;
    var Mana : integer = 100;
    var Name : string = "Sprite";
}

data BatMan : Hero;
data SpiderMan : Hero;

```

To fill more quickly the values of the public fields of the object (in a post-typed structure, everything is public), we can use the keyword ```set``` :

```
set BatMan { 34, 22, "Batman" };
set SpiderMan { 76, 12, "Spider-man" };
```

## Typed structures
A structure defined as an immediate type (not post-typed, as seen above) can be considered as a class in an OO language such as C++ or C#. Here is the prototype:

```
data structure MyObject : inheritance {
    private:
        ...
    public:
        ...
    static:
        ...
    upon:
        ...
}
```

To make it easier and lighter, APIL adds the object preprocessor definition :

```
define object data structure;
```

This allows us to write object instead of data structure when we want to define an object instance:

```
object MyObject : inheritance {
    private:
        ...
    public:
        ...
    static:
        ...
    upon:
        ...
}
```

The ```private``` and ```public``` blocks are simple, ```private``` defines that everything in its grip is ```private``` to the developer (but not when inheriting), and aim towards that with ```public```.

### ```static```

A ```static``` member, in Arlia, is defined by the language as being trans-object. That is, you don't need to use the [object] type through a variable to be able to use these ```static``` members. However, a ```static``` field keeps its last value in memory:

```
object foo {
    static:
        var a : integer;
        func bar() {
            ...
        }
}

foo.a = 9;
foo.bar();
```

In comparison, it is exactly the same as a non post-typed ```structure```, except that a non post-typed ```structure``` only accepts ```ret``` functions.

The display method you have seen a few times uses this principle:

```
@dio:display.writeln("Hello");
```

the @ just refers to a namespace:

```
namespace dio;

object display {
    ...
    static:
        func writeln(text) {
            ...
        }
    ...
}
```

However, it should not be abused; it is considered a "dirty" practice.

### ```upon```
Thanks to this specific member definition block, we can interact with the object implicitly, notably by defining operators. To represent the object, we use ```it```. In C++-like, we could match it to ```this```.

Before, we must see two concepts: ```images``` and ```it```.

#### ```-->``` or ```image```
This operator (```-->```) allows to represent an object as an modifiable image:

```
var MyVar : integer = 87;

var MyImage --> MyVar;
```

```MyImage``` will have absolutely all the properties of ```MyVar```. In the generated assembler code, you just have to understand that no copy is made, but a pointer create a reference. The compiler makes this referenced object modifiable. This increases performance if you need to use a value without copying it.

We will use ```it``` in ```upon``` and encapsulation in general to make references.

#### ```it```
```it``` is a pointer that allows to represent the object once instanciated. We can also use it to create operators and thus interact with the object implicitly:

```
object number {
    private:
        var nbr : real;
    upon:
        it --> nbr;
}
```

Here we thus join the concept of image, allowing to define the object once instantiated as referenced to a certain method (or other) according to the defined operator. In this case, we allow the object to interact with a private member (nbr) as if we were interacting directly with that same member :

```
object number {
    private:
        var nbr : real;
    upon:
        it --> nbr;
}

var MyNumber : number = 9;
MyNumber.annihilate();
```

```annihilate()``` is a method of type ```real```, defined as private field type ```number``` of the object ```number```. As you can see, we only do a simple interaction here thanks to the references.

But we can make this concept more useful. Let's create our own container of ```integer``` for example:

```
object vector {
    private:
        var container[] : integer = { 0 };
        func get(index : integer) : integer {
            if (index > size) throw "Over vector current size";
            if (index < 0)    throw "Below vector current size";
            return container[index];
        }
    public:
        ret size = (sizeof(container) / sizeof(container[0]));
        func push(item : integer) {
            container += item;
        }
        func pop() {
            delete container[size];
        }
    upon:
        it+=(item : integer) --> push(item);
        it[index : integer] --> get(index);
}


var vec : vector;
vec.push(9);
vec.push(3);
vec.pop();
vec += 6;
var n : integer = vec[1];
```

We will see the keyword ```delete``` later.

As you can also notice, the use of pointers is often abstract syntax, but without any costs.

## Pointers
The notion of pointers being very useful, the language allows them. Only the syntax changes from C-like, the rest is almost identical:

```
var MyVar = 9;
var Pointer = &MyVar;
~Pointer = 4;

` MyVar is equal to 4 `
```

## Template
Templates are used to give information as a parameter to the compiler so that it can assemble code that meets these criteria when they are called. We can define templates on objects, as well as on functions:

```
object <template> name {
    ...
}

func <template> name() {
    ...
}
```

Example:

```
object <MAX : integer> container {
    ...
    func add(item) {
        if (size != MAX) { ... }
        else { ... }
    }
    ...
}
```

There are two keywords related to templates:

### ```typename```

```typename``` represents the type of an object or a value:

```
object <type : typename> vector {
    private:
        var container[] : type;
    ...
}
```

This makes it possible to widely extend the capabilities of language.

### ```typesize```

```typesize``` represents the object size AND can be used to represent the variable size:

```
object <typesize = 2> integer {
    private:
        var rlvalue : typesize = 0;
    ...
}
```

Here is a small extract of the integer object. We can therefore see that its size in memory is set to 2 and that we can use this same size to type its fields.

This is exactly the same as what we saw above with standard language types.

However, this keyword being reserved for lower level operations, it is rare that you have to use it. Nevertheless, APIL allows to abstract sizes thanks to preprocessor definitions, according to the list of types that the compiler assigns during the inference of type:

| Compiler types names   | size (bytes) |
|:----------------------:|:------------:|
|         byte           |       1      |
|         word           |       2      |
|         dword          |       4      |
|         fword          |       6      |
|         qword          |       8      |
|         tword          |      10      |
|        dqword          |      16      |
|        qqword          |      32      |
|        dqqword         |      64      |

Usually, the size of an object is determined by the sum of the sizes of its members. Except ```typesize```, if defined, will represent this new value. We will use the ```sizeof()``` operator to represent the size of an object.
On variables and functions, ```typesize``` is automatically defined by the compiler.

## Rules

We are entering a more complex part of the language: the rules. A rule is a type that only accepts a certain type of value. For example, we can create a type that only accepts lowercase characters:

```
rule lowercase[] : char = ('a' to 'z');
```

In fact, rules are almost useless as a type of definition. We'll use them more for a purpose like this:

```
rule lowercase[] : char = ('a' to 'z');

var chr : char = 'J'

if (chr isnt lowercase)
    ` chr is not a lowercase character `
```

To better understand the rules, we must know their origins:

Indeed, as object, rule is a keyword defined by a preprocessor directive:

```
define rule data ret;
```

Indeed, ```data ret``` is a strange association, but when you think about it: not so much. Indeed, we define a "lambda" constant that can accept several values if it is an array as an object instance. Which gives us this result.

Another example:

```
rule stupid[] : string = { "racist", "riche", "innocent" };

var Trump;

if (Trump is stupid) {
    ` instructions `
}

```

At first, beginners don't see much point in this instruction, this concept. But know that it will be very useful to you in due course.

Concrete example in a game:

```
...
match (sprite --> sprite1) {
    case (sprite is enemy) ...
    case (sprite is sword) ...
    case (sprite is monster) ...
    case (sprite is ally) ...
    default ...
}
...
```

Instead of over-stepping the types conditions, this is simpler and prettier.

## ```delete```, ```free``` and ```new```
In Arlia, there are memory management operators:

### ```delete```

```delete``` simply deletes a variable or an object:

```
var MyVar = 9;
delete MyVar;
```

If we try to use ```MyVar```, there will be an error because ```MyVar``` will no longer exist.

### ```free```

Unlike ```delete```, ```free``` re-instantiates a variable or an object to its initial state by deleting all values and potentially freeing up memory used:

```
var MyVar = 9;
free MyVar;
```

Here, ```MyVar``` will simply be worth ```0```, but it obviously depends on the types, or the objects.

### ```new```

```new``` is a way to create an anonymous object instance:

```
var wind : @nut:window;
set wind {
    text = "My window";
    size = new @nut:2Dsize(500, 500);
}
```

The nut library allows GUI management.

As you can see, here we define the window size using an anonymous 2D size.

Another example: 

```
func foo() : list<integer> {
    return new list<integer>{ 8, 4, 2, 0 };
}
```

## ```namespace```
A namespace is a way to better structure its program, functions and variables. Unlike C++-like, a namespace is declared as is (so as not to overload the code.):

```
namespace MyNamespace;
...
namespace MyOtherNamespace;
...
```

To use a namespace, we use the ```@``` symbol:

```
@MyNamespace...
```

If you need to use many functions from the same namespace, it is possible to reduce the writing:

```
@MyNamespace;

func1();
func2();
func3();
...
```

instead of doing this:

```
@MyNamespace.func1();
@MyNamespace.func2();
@MyNamespace.func3();
...
```

However, beware of calls and cross names!

To call an object from a namespace, this does not change, just use the call operator / type assignment:

```
@namespace:object.method();
```
```
var MyVar : @namespace:object;
```

Once again, the syntax is not fixed, but it is advised to keep the right and left values stuck to the symbol so as not to get confused :

```
var MyVar : @namespace : object;
```
to
```
var MyVar : @namespace:object;
```

## Preprocessor Instructions

| statements |
|:----------:|
|   import   |
|   define   |
|    ifdef   |
|    elifdef |
|   elsdef   |
|   extern   |
|   proc     |

### ```import```

Simply import a header or a library. Attention, the format must always be in ```.k``` format:

```
import header;
```

The source file must have the format ```.arl``` :

```
Source.arl
header.k
```

Files cannot be binary.

### ```define```

Allows to define a value or a just nominative definition (attention: not a macro, as in C-like) :

```
define MAX = 100;
define MIN = 0;

define Windows;
define Linux;
define Mac;
```

### ```ifdef``` / ```elifdef``` / ```elsdef```

Simply check whether a preprocessor definition (define) is defined or not:

```
ifdef Windows {
    ...
}
elifdef Linux {
    ...
}
elifdef Mac {
    ...
}
elsdef {
    ...
}
```

### ```extern```

```extern``` is different from the C-like extern, this one just allows to write assembler code directly into the program. The assembler used is [FASM](https://flatassembler.net/docs.php?article=manual):


```
extern "
    push eax
    mov eax, 8
";
```

We will see a "different" use of ```extern``` in the next point.

### ```proc```

```proc``` allows you to create your own keywords, operators and/or features. Basically: it extends the capabilities of the language. Here are the possible prototypes:


#### A keyword operator

```
proc "name"(args) {
    ...
}
```

As you can see, the idtantifier of a ```proc``` is always between double quotes.

Example :

```
proc "isdef"(def) {
    ifdef def {
        return true;
    }
    return false;
}

...

define Windows;

if (isdef(Windows))
    ` Ok, Windows is defined `

```

#### An operator

```
proc a, "$" {
    return a * 1,17;
}

` We just convert the a argument into dollar `

var sold : integer = 9$;

` sold = 10.53 $ `

```

#### A keyword

```
proc "say", text {
    @dio:display.writeln(text);
}

say "Hello!";

```

## Modify an intrinsic property of the language

The language allows to modify its properties via, in particular, the object-oriented paradigm as follows. How about allowing strings to display themselves via a method for example? This is:

```
rule String[] : array<char> = (ASCII);
```

First, we must allow language to recognize what a character string is. We therefore use a rule that includes all the characters of the ASCII table (defined in dio).

Then we create an anonymous object instance that inherits this rule:

```
object : String {

}
```

And then we just have to create methods, in our case:

```
func display() {
    @dio:display.writeln(it);
}
```

If we put it all together:

```
rule String[] : array<char> = (ASCII);

object : String {
    public:
        func display() {
            @dio:display.writeln(it);
        }
}
```

And to call this method:

```
"Hello, world!".display();
```

As any manipulable value is an Arlia object, you can do the same for any other object.

We said in the introduction that we would be able to recreate the Ruby loop principle:

```ruby
3.times do
    ...
end
```

Through OO and processes (```proc```), we can imitate the principle:

```
rule number : array<real> = (0 to 9);

object : number {
    public:
        proc "time"() for (i : integer = 1; i < it; ++i);
}

3.time() {
    @dio:display.writeln("Bonjour !");
}

```

As you can see, 3 being an object, we knew how to add a functionality thanks to a process integrated to the object.

## APIL Preprocessor Definitions and Standard Process (PDSP)

|    Names   |                    Values                    |
|:----------:|:--------------------------------------------:|
|    BYTE    |                       ```1```                      |
|    WORD    |                       ```2```                      |
|    DWORD   |                       ```4   ```                   |
|    FWORD   |                       ```6```                      |
|    QWORD   |                       ```8```                      |
|    TWORD   |                      ```10```                      |
|   DQWORD   |                      ```16```                      |
|   QQWORD   |                     ``` 32```                      |
|    ZWORD   |                      ```64```                      |
|   object   |                ```data structure```                |
|    rule    |                   ```data ret```                   |
|  _asm(asm) |                  ```extern asm```                  |
| breakpoint |                 ```_asm("int 3")```                |
| isdef(def) | ```ifdef { def { return true } return false }``` |
| LIBDIR | ```"INCLUDE"``` |
| USESTD(name, dll, file) | ```"use '" + name + "', from '" + dll + "', in '" + LIBDIR + "\\" + file + "'"``` | 
| USE(name, dll, file) | ```"use '" + name + "', from '" + dll + "', in '" + file + "'"``` |
| TEST(VarName) | ```extern "test " + &VarName + ", " + \&VarName```

## Target assembler
The target language of the Arlia compiler is [FASM](https://flatassembler.net/docs.php?article=manual).

## Main function
The main function can take no parameters, and not be typed. Here are the different prototypes:

```
func main() {
    ...
}
```
```
func main() : typename {
    ...
}
```
```
func main(argc : integer, argv[] : string) {
    ...
}
```
```
func main(args : list<string>) {
    ...
}
```

## typename as type
Arlia also allows an object to have an object name as type. Obviously, the value will be the instance of the object in question:

```
var MyObj : typename = integer;
```
```
var MyObj : typename = string;
```
```
var MyObj : typename = list<integer>;
```
```
var MyObj : typename = char;

var MyChar : MyObj = 'h';
```

This is not very useful in a general way, but of course it is very useful locally when instance an object.

## ```extern```
As mentioned above, ```extern``` allows writing FASM assembler code directly into the program. Preprocessor directives also use it to call a DLL feature:

```
extern USESTD("puts", "msvcrt", "win32a.inc");
```

Or even:

```
define PUTS USESTD("puts", "msvcrt", "win32a.inc");

extern PUTS;
```

```USESTD``` allows access to reserved libraries, while ```USE``` allows access to any library :

```
extern USE("MyFunc", "MyDLL", "MyFile");
```

## ```TEST(VarName)```
This macro allows to check if an address in memory is ```null``` or not:

```
var MyVar;
if (TEST(MyVar)) {
    ` MyVar is null `
}
```

## Names
In Arlia, the names given to objects can only contain letters and numbers:

```
var ABC    ` ok `
var ABC76  ` ok `
var ABC_76 ` error `
var ABCé76 ` error `
```

___
