module Parser
open AST
open FParsec
open System.Security.Principal

// Todo:
//  - Better module identifier
//  - Importations

// The Arlia parser is based on the FParsec library
// (https://github.com/stephan-tolksdorf/fparsec)

(* -------- Spaces and comments -------- *)

let pcomment = (pstring "`" >>. skipCharsTillString "`" true System.Int32.MaxValue) <?> ""

let pspaces = spaces >>. many (spaces >>. pcomment >>. spaces)

let ws = pspaces >>. many (pspaces >>. pcomment >>. pspaces) |>> (fun _ -> ())

let ws1 = spaces1

let str_ws s = pstring s .>> ws

let str_ws1 s = pstring s .>> ws1

(* -------- Utils -------- *)

let (<|>) a b = attempt a <|> b

let comma = str_ws "," <?> "comma"

let between' a b a' b' = between ((ws >>. str_ws a) <?> a') ((ws >>. str_ws b) <?> b')

let eol = (str_ws ";") <?> "end of instruction"

let leftp = "left parenthesis"
let rightp = "right parenthesis"
let lhook = "left hook"
let rhook = "right hook"
let lblock = "start block"
let rblock = "end block"
let typeassign = str_ws ":" <?> "type assignment"
let typereturn = str_ws "->" <?> "type return"
let valueassign = str_ws "=" <?> "data assignment"
let valuestorage = str_ws "<-" <?> "value storage"
(* -------- Identifiers -------- *)

// All keywords in the language are listed to prevent them from being used as identifiers.
let reserved =
    [ "var"; "func"; "let"; "type"; "module"; "while"; "for"; "return";
      "continue"; "if"; "else"; "extern"; "match"; "to"; "in"; "do"; "step";
      "throw"; "try"; "new"; "true"; "false"; "λ"; "with"; "it"; "public";
      "private"; "then"; "lambda"; "import"; "include"; "vanaur"; "interface" ]

let pidentifierraw =
    let inline isIdentifierFirstChar c = isLetter c
    let inline isIdentifierChar c = isLetter c || isDigit c
    many1Satisfy2L isIdentifierFirstChar isIdentifierChar "identifier"

let pidentifier =
    pidentifierraw
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("'" ^ s ^ "' is not expected here")
        else preturn s

let ptypeidentifier = pidentifier // Specifies the name of a type

let pidentifier_ws = pidentifier .>> ws

(* -------- Types assignment -------- *)

//  There are several kinds of typing in Arlia:
//
//      - The alias type, which refers to another type:
//          `type MyAlias Int = Integer`
//          `type Alias<T> = T` // Generic types can be used
//
//      - The product type, which defines a heterogeneous and constant type structure:
//          `type MyProd = (T1, T2, T3, ... TK)`
//          // Tuplets of a product type can be any kind of type
//
//      - The arrow type, which allows you to define a pure function:
//          `type MyFunc = (T1, T2, ... TK) -> TReturn`

let typename = ptypeidentifier |>> TypeName <?> "type name"

let tupletype = between' "(" ")" leftp rightp (sepBy typename comma) |>> TupleType <?> "tuple type"

let typeNT = typename <|> tupletype

let typefuncdef = 
    pipe2
        (between' "(" ")" leftp rightp (sepBy (ws >>. typeNT) comma))
        (str_ws "->" >>. ws >>. typeNT)
        (fun args ret -> ArrowType(args, ret)) <?> "arrow type"

let gentype = (between' "<" ">" "generic type" "generic type" (sepBy typeNT comma)) |>>
              (GenericType.GenericType) <?> "generic type" <|> (ws >>% GenericType.NoGenericType)

let typeNTF = typefuncdef <|> typeNT

let TYPE = typeNTF

let generictype = ((between' "<" ">" "" "" (sepBy typeNTF comma)) |>> GenericType.GenericType <?> ("generic type")) <|>
                  (ws >>% NoGenericType)

(* -------- Literals -------- *)

//  A literal in Arlia is either
//      - An integer number     // Integer type
//      - A floating number     // Float type
//      - A character string    // String type
//      - A character           // Char type
//      - A data identifier
//      - A Boolean value       // Boolean type

type Lit = NumberLiteralOptions
let numberFormat = Lit.AllowMinusSign
               ||| Lit.AllowFraction
               ||| Lit.AllowHexadecimal

let pnumber =
    numberLiteral numberFormat "number"
    |>> fun nl ->
            if   nl.IsInteger then Int(int nl.String)
            else                   Float(float nl.String)

let ptrue   = str_ws "true"  |>> fun _ -> Bool true
let pfalse  = str_ws "false" |>> fun _ -> Bool false

let pbool   = ptrue <|> pfalse

let inline unescape c =
    match c with
    | 'n' -> '\n' // newline
    | 'r' -> '\r' // carriage return
    | 't' -> '\t' // tabulation
    | c -> c      // character

let pexpr, pexprimpl = createParserForwardedToRef()

let pstringliteral =
    let normalChar = satisfy (fun c -> c <> '\\' && c <> '"')
    let escapedChar = pstring "\\" >>. (anyOf "\\nrt\"" |>> unescape)
    between (pstring "\"") (pstring "\"")
        (manyChars (normalChar <|> escapedChar)) |>> fun s -> Literal.String(s)

let pcharliteral =
    let normalChar = satisfy (fun c -> c <> '\\' && c <> '"')
    let escapedChar = str_ws "\\" >>. (anyOf "\\nrt\"" |>> unescape)
    between (str_ws "'") (str_ws "'") (normalChar <|> escapedChar)
    |>> fun c -> Literal.Char(c)

let pid = pidentifier |>> Literal.Identifier
let pliteral =
    (pnumber <|> pbool <|> pstringliteral <|> pcharliteral <|> pid |>> Literal) <?> "literal"
let plist =
    between' "[" "]" lhook rhook (sepBy pexpr comma) |>> ListValue <?> "list"
let pvalue' = pliteral <|> plist
let ptuple =
    between' "(" ")" leftp rightp (sepBy pvalue' comma) |>> TupleValue <?> "tuple"
let pvalue'' = pvalue' <|> ptuple
let plist' =
    between' "[" "]" lhook rhook (sepBy pvalue'' comma) |>> ListValue <?> "list"

let pvalue''' = pvalue'' <|> plist'

(* -------- Expressions -------- *)

let lambda = ((str_ws "lambda") <|> (str_ws "λ")) <?>
             ("lambda expression")
let plambdaargs = many1 ((pidentifier <|> (str_ws "_")) .>> ws)

let pdefine =
    pipe2
        (pidentifier .>> ws)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (fun name ty -> Define(name, ty))

/// 'passign' allow to assign a value to a new data: 'a = b'
let passign =
    pipe3
        (pidentifier_ws)
        (valueassign)
        (pexpr)
        (fun var _ expr -> (var, expr))

//  In Arlia, pointers do not exist, but references can be used,
//  thanks to the symbol' &'.
//  If a reference is sent to a (pure) function,
//  it will still return the value of the result,
//  with a copy of the reference.
//  The language does not (yet) plan to avoid this.

let ref' = str_ws "&" >>. ws >>. pidentifier |>> Ref

let param' = pexpr |>> Param

let pparam = ref' <|> param'

let pparams =
    (between' "(" ")" leftp rightp (sepBy pparam comma)) <|>    // as tuple parameters: foo(a, b, ... k)
    (ws >>. many1 pparam)                                       // as suite of parameters: foo a b ... k

let pinvoke =
    pidentifier_ws .>>. pparams
    |>> fun (name, parameters) -> Invoke(Literal.Identifier name, parameters)

/// Ternary operator 'if'-'else'
let pternary = 
    pipe3
        (str_ws "if" <?> "ternary operator" >>. pexpr)
        (str_ws "then" >>. ws >>. pexpr)
        (str_ws "else" >>. ws >>. pexpr)
        (fun cnd t f -> TernaryOp(Condition cnd, IfTrue t, IfFalse f))

/// Lambda expression 'lambda x y -> x + y' for example
let plambda =
    pipe2
        (ws >>. lambda >>. ws >>. plambdaargs)
        (ws >>. str_ws "->" >>. ws >>. pexpr)
        (fun args expr -> Lambda(args, expr))

/// String into the 'extern' expression for filename
let externstr = 
    let normalChar = satisfy (fun c -> c <> '"')
    between (str_ws "(" <?> leftp >>. ws >>. pstring "\"")
            (ws >>. pstring "\"" >>. str_ws ")" <?> rightp)
            (manyChars normalChar) 

/// Extern expression 'extern ("dll")("function")(parameters)'
let pextern = 
    pipe3
        (str_ws1 "extern" <?> "external call" >>. ws >>. externstr)
        (ws >>. externstr)
        (ws >>. pparams)
        (fun dll func parameters -> Expr.Extern(dll, func, parameters))

/// The wildcard item: '_'
let pwildcard =
    ((ws >>. str_ws "|" >>. ws >>. str_ws "_") >>.
    (ws >>. str_ws "=>" >>. pexpr))
    |>> Wildcard

/// A pattern is a type constructor decomposed
let pctorpattern =
    pipe3
        (ws >>. str_ws "|" >>. ws >>. ptypeidentifier)
        (ws >>. (between' "(" ")" leftp rightp (sepBy pidentifier comma)))
        (ws >>. str_ws "=>" >>. pexpr)
        (fun id args ex -> Pattern(id, args, ex))

/// A case is an expression or a literal that match with the first item
let pcase =
    pipe2
        (ws >>. str_ws "|" >>. ws >>. pexpr)    // pattern
        (ws >>. str_ws "=>" >>. ws >>. pexpr)   // if matched
        (fun x y -> Case(x, y))

//  The pattern matching acts on literals,
//  type constructors, as well as the wildcard,
//  which acts as a 'default'.

let pcaselist = many1 (pctorpattern <|> pcase)

let pmatch =
    pipe2
        (str_ws "match" >>. ws >>. pexpr)
        (str_ws "with" >>. ws >>. pcaselist)
        (fun m cases -> Match(m, cases)) <?> "match"

let pvalue = pmatch     <|> // match
             pinvoke    <|> // function invokation
             pvalue'''  <|> // values (literals, tuples and lists)
             pternary   <|> // ternary operator
             plambda    <|> // lambda expression
             pextern        // extern expression

type Assoc = Associativity

let opp = OperatorPrecedenceParser<Expr, unit, unit> ()

let eidentifier = pidentifier |>> Expr.Identifier

pexprimpl := 
    opp.ExpressionParser <?> "expression" <|>
    eidentifier <?> ""

let term = pvalue .>> ws <|> between' "(" ")" leftp rightp pexpr

opp.TermParser <- term

/// Inflix operators
let inops =
    [ "+"; "-"; "*"; "/"; "%"; "**"; "^+"; "^-"; "^*"; "^/"; "^%"; "&&"; "||";
      "^"; "=="; "!="; "<="; ">="; "<"; ">" ]

for op in inops do // Append each inflix operators to expressions
    opp.AddOperator(InfixOperator(op, ws, 1, Assoc.Left, fun x y -> InfixOp(x, op, y)))

// Dot is an infox operator
opp.AddOperator(InfixOperator(".", ws, 1, Assoc.Left, fun l r -> Dot(l, ".", r)))

let newObjConstructor, newObjConstructorimpl = createParserForwardedToRef ()

/// Type constructor
let pconstructor = 
    pipe3
        (str_ws "new" >>. ws >>. typename)
        (gentype)
        (between' "(" ")" leftp rightp (sepBy (pexpr <|> newObjConstructor) comma))
        (fun tyname genty value -> TypeConstructor(tyname, genty, value)) <?> "type constructor"

newObjConstructorimpl := attempt pconstructor

let pexpr' = (*between' "(" ")" leftp rightp*) pexpr

let pexpr'' = pexpr             <|>     // expressions
              newObjConstructor <|>     // type constructor
              pvalue            <|>     // value (literal, tuple or list)
              pternary          <|>     // ternary operator
              plambda                   // lambda expression

/// Allowed tokens for custom operator
let tokops = [ '+'; '-'; '*'; '/'; '%'; '|'; '<'; '>'; '.'; '$'; '~'; '^'; '!'; '='; '°'; '@'; '?' ]

let opidraw =
    let inline isSymbolFirstChar c = System.Char.IsSymbol c
    let inline isSymbolChar c = System.Char.IsSymbol c
    many1Satisfy2L isSymbolFirstChar isSymbolChar "custom operator"

let opid' =
    opidraw
    >>= fun s ->
        for c in s do
            if tokops |> List.exists ((<>) c) then fail ("'" ^ c.ToString() ^ "' is not an expected symbol")
            else preturn s
        preturn s

let opid = str_ws "op" >>. ws >>. opid' |>> LetFuncId.CustomOp // Améliorer
let letfuncid = opid <|> (pidentifier |>> Identifier')

(* -------- Statements -------- *)

/// Value storage 'a <- b'
let psto =
    pipe3
        (pidentifier_ws)
        (valuestorage)
        (ws >>. pexpr'')
        (fun var _ expr -> Storage(var, expr))

let pstatement, pstatementimpl = createParserForwardedToRef ()
let psinglestatement = pstatement |>> fun statement -> [statement]

let pmemberstatement, pmemberstatementimpl = createParserForwardedToRef ()

/// Statement block with single statement
let pstatementblock =
    (psinglestatement) <|>
    (between' "{" "}" lblock rblock (many pmemberstatement))

/// Statement block without single statement
let pstatementblock' =
    (between' "{" "}" lblock rblock (many pmemberstatement))
    
/// Argument
let parg = 
    pipe3
        (pidentifier)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        ((ws >>. valueassign >>. ws >>. pexpr |>> DefaultValueArg) <|> (ws >>% NoDefaultValueArg))
        (fun name ty optval -> Arg(Define(name, ty), optval))

let parglist =
    (ws >>. many1 ((between' "(" ")" leftp rightp parg) <|> parg) <?> "parameters") <|>
    (between' "(" ")" leftp rightp (sepBy parg (str_ws ","))      <?> "parameters")
    
/// Let as function
let pletfunc =
    pipe4
        ((str_ws "let" >>. ws >>. pidentifier) <|> (ws >>. pidentifier))
        (ws >>. parglist)
        ((ws >>. typereturn >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier args ty value -> LetFuncDeclr(identifier, args, ty, Value value))

/// Let as const data
let plet = 
    pipe4
        ((str_ws "let" >>. ws >>. pidentifier) <|> (ws >>. pidentifier))
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (gentype)
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier ty genty value -> LetDeclr(identifier, ty, genty, Value value))
    
/// Variable
let pvar = 
    pipe4
        (str_ws "var" >>. ws >>. pidentifier)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (gentype)
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier ty genty value -> VarDeclr(identifier, ty, genty, Value value))

/// Function
let pfunc =
    pipe4
        (str_ws "func" >>. ws >>. letfuncid)
        (ws >>. parglist)
        ((ws >>. typereturn >>. ws >>. ptypeidentifier |>> TypeName) <|> (ws >>% ImplicitType))
        (pstatementblock')
        (fun identifier args ty block -> FuncDefinition(identifier, args, ty, block))

(* -------- Selection statements -------- *)

/// If statement
let pif =
    pipe2
        (str_ws "if" >>. pexpr')
        (str_ws "then" >>. ws >>. pstatementblock)
        (fun e block -> If(e, block))

/// If - else structure
let pifelse =
    pipe3
        (str_ws "if" >>. pexpr')
        (str_ws "then" >>. ws >>. pstatementblock)
        (str_ws "else" >>. pstatementblock)
        (fun i ib eb -> IfElse(i, ib, eb))

(* -------- Iteration statements -------- *)

// For - step - to arguments

let pforstepargs =
    let pint = attempt passign
    pipe3
        (pint .>> str_ws "to")
        (pexpr .>> str_ws "step")
        (pexpr)
        (fun from until steps -> from, until, steps)

let pforargs =
    let pint = attempt passign
    pipe2
        (pint .>> str_ws "to")
        (pexpr)
        (fun from until -> from, until)

let pforstep =
    pipe2
        (str_ws "for" >>.  pforstepargs) 
        (str_ws "do" >>. ws >>. pstatementblock)
        (fun (init, until, iter) block -> ForStep(init, To until, Step iter, block))

let pfor =
    pipe2
        (str_ws "for" >>. pforargs)
        (str_ws "do" >>. ws >>. pstatementblock)
        (fun (init, until) block -> For(init, To until, block))

let pwhile =
    pipe2
        (str_ws "while" >>. pexpr') 
        (str_ws "do" >>. ws >>. pstatementblock)
        (fun e block -> While(e, block))

let pdowhile =
    pipe2
        (str_ws "do" >>. pstatementblock)
        (str_ws "while" >>. pexpr')
        (fun block w -> DoWhile(block, w))

(* -------- Exceptions -------- *)

let pthrow = str_ws1 "throw" >>. pexpr'' |>> Throw

let ptry =
    pipe2
        (str_ws "try" >>. pexpr)
        (ws >>. str_ws "with" >>. ws >>. pcaselist)
        (fun ex cases -> Try(ex, cases))

(* -------- Jump statements -------- *)

let preturn = str_ws1 "return" >>. pexpr'' |>> Return

let pcontinue = str_ws "continue" |>> fun _ -> Continue

(* -------- Call statement -------- *)

let pcall = pidentifier .>>. pparams
            |>> fun (name, parameters) -> FuncInvocation(name, parameters)

/// Extern expression 'extern ("dll")("function")(parameters)'
let pextern_stmt = 
    pipe3
        (str_ws1 "extern" <?> "external call" >>. ws >>. externstr)
        (ws >>. externstr)
        (ws >>. pparams)
        (fun dll func parameters -> Statement.Extern(dll, func, parameters))

(* -------- Type declaration -------- *)

let paccess = 
    ((ws >>. (str_ws "private") >>. ws |>> fun _ -> Private) <|>
     (ws >>. (str_ws "public") >>. ws  |>> fun _ -> Public)  <|>
     (ws >>% Public)) /// A none indicate statement access if always public

let pargconstruct = 
    pipe4
        (paccess)
        (pidentifier)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        ((ws >>. valueassign >>. ws >>. pexpr |>> DefaultValueArg) <|> (ws >>% NoDefaultValueArg))
        (fun access name ty optval -> ArgFieldConstructor(access, Define(name, ty), optval))

let pargconstructlist = (between' "(" ")" leftp rightp (sepBy pargconstruct comma)) <?>
                        ("constructor parameters")

let psumconstructor a =
    pipe2
        (if a = true then (ws >>. str_ws "|" >>. ws >>. ptypeidentifier) else (ws >>. ptypeidentifier))
        (
            (ws >>. (between' "(" ")" leftp rightp (sepBy1 TYPE comma)))
            <|>
            (ws >>% [EnumType])
        )
        (fun identifier args -> SumConstructor(identifier, args)) <?> "additional constructor(s)"

let psumconstructors =
    many (psumconstructor true)
    |>> SumConstructors <?> "constructors"

/// Statements for class type
let ptypeasclassstatement = 
    pipe2
        (ws >>. paccess)
        (ws >>. pstatement)
        (fun access stmt -> Member(access, stmt))

let pmembers = (between' "{" "}" rblock lblock (many ptypeasclassstatement)) <?>
               ("object members or end of line")

/// Type as class: fields, methods and ONE constructor
let ptypeasclass =
    pipe4
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (ws >>. pargconstructlist)
        (pmembers)
        (fun identifier gens args members -> TypeAsClass((identifier, gens, args), members)) <?> "class type"

/// Type as structure constructor
let ptypeasstruct =
    pipe3
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (ws >>. pargconstructlist)
        (fun identifier gens args -> TypeAsStruct(identifier, gens, args)) <?> "structure type"

/// Type as alias
let ptypeasalias =
    pipe3
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (valueassign >>. TYPE)
        (fun identifier gens alias -> TypeAsAlias(TypeName identifier, gens, alias)) <?> "alias type"

/// Sum type
let psumtype =
    pipe3
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (ws >>. valueassign >>. ws >>. psumconstructors)
        (fun identifier gens constructors -> TypeAsSum(TypeName identifier, gens, constructors)) <?> "sum type"

(* -------- Scope -------- *)

let pmoduleblock = many pstatement

let pmodulename = pidentifier

let pmodule = 
    pipe2
        (str_ws "module" >>. ws >>. pmodulename)
        (eol >>. ws >>. pmoduleblock)
        (fun name block -> Module(name, block))

(* -------- Processor -------- *)

let includefilename = 
    let normalChar = satisfy (fun c -> c <> '"')
    between (str_ws "\"") (str_ws "\"") (manyChars normalChar)

let pinclude = str_ws1 "include" >>. includefilename |>> Include

(* -------- Statement implementation -------- *)

// Expressions can be found in top-level, there is no need for a body for.
let paction = pmatch <|> pexpr'' |>> AnonymousExpression

//  Some instructions cannot be used within another,
//  so there are two distinct groups, some of which have been removed or added.

pmemberstatementimpl :=
    attempt (preturn .>> eol)        <|>    // return
    attempt (pcontinue .>> eol)      <|>    // continue
    attempt (pletfunc .>> eol)       <|>    // let ()
    attempt (plet .>> eol)           <|>    // let
    attempt (pvar .>> eol)           <|>    // var
    attempt (psto .>> eol)           <|>    // a <- b
    attempt (pifelse)                <|>    // if else
    attempt (pif)                    <|>    // if
    attempt (pfor)                   <|>    // for
    attempt (pforstep)               <|>    // for step
    attempt (pwhile)                 <|>    // while
    attempt (pdowhile)               <|>    // do while
    attempt (pcall .>> eol)          <|>    // foo()
    attempt (pextern_stmt .>> eol)   <|>    // extern ()()()
    attempt (pthrow .>> eol)         <|>    // throw
    attempt (ptry .>> eol)           <|>    // try
    attempt (ptypeasclass)           <|>    // type () {}
    attempt (ptypeasstruct .>> eol)  <|>    // type ()
    attempt (psumtype .>> eol)       <|>    // type = | | ... |
    attempt (ptypeasalias .>> eol)          // type

pstatementimpl :=
    attempt (preturn .>> eol)        <|>    // return
    attempt (pcontinue .>> eol)      <|>    // continue
    attempt (pletfunc .>> eol)       <|>    // let ()
    attempt (plet .>> eol)           <|>    // let
    attempt (pvar .>> eol)           <|>    // var
    attempt (psto .>> eol)           <|>    // a <- b
    attempt (pifelse)                <|>    // if else
    attempt (pif)                    <|>    // if
    attempt (pfor)                   <|>    // for
    attempt (pforstep)               <|>    // for step
    attempt (pwhile)                 <|>    // while
    attempt (pdowhile)               <|>    // do while
    attempt (pcall .>> eol)          <|>    // foo()
    attempt (pfunc)                  <|>    // func
    attempt (pthrow .>> eol)         <|>    // throw
    attempt (ptry .>> eol)           <|>    // try
    attempt (ptypeasclass)           <|>    // type () {}
    attempt (ptypeasstruct .>> eol)  <|>    // type ()
    attempt (psumtype .>> eol)       <|>    // type = | | ... |
    attempt (ptypeasalias .>> eol)   <|>    // type
    attempt (pextern_stmt .>> eol)   <|>    // extern ()()()
    attempt (pinclude .>> eol)       <|>    // include
    attempt (paction .>> eol)               // expr

(* -------- Program -------- *)

let pprog, pprogimpl = createParserForwardedToRef ()
pprogimpl := ws >>. (attempt (manyTill (pstatement <|> pmodule) (eof <?> "")) |>> Program)

(* -------- Parse function call -------- *)

// Parsing error management functions

let showFinalErr (n : int) (s : string) (i : int) =
    printfn "\n%s" ((n - 1).ToString() ^ " |")
    printfn "%s" ((n.ToString()) ^ " | " ^ s)
    printf "%s" ((n + 1).ToString() ^ " |")
    System.Console.ForegroundColor <- System.ConsoleColor.Red
    printfn "%s" (new string(' ', i) ^ "↑")
    System.Console.ResetColor()

let getErrorPosResult (msg : string) : string =
    let n' = msg.LastIndexOf("Error in")
    let n = msg.Substring(0, n')
    msg.Substring(n', n.IndexOf("\n"))

let lineOfError msg =
    let err = getErrorPosResult msg
    System.Int32.Parse
        (err.Substring
             (err.IndexOf(": ") + 2,
              err.Substring(err.IndexOf(": ")).IndexOf(" Col") - 2))

let colOfError msg =
    let err = getErrorPosResult msg
    System.Int32.Parse(err.Substring(err.IndexOf("Col: ") + 5))

let completeErrorMessage (err : string) =
    let splt = err.Split '\n'

    let isexpect =
        if splt.[splt.Length - 3].StartsWith "  Expecting:" then 3
        else 2

    let mutable errmsg = (splt.[splt.Length - (isexpect)] ^ "\n")
    errmsg <- (errmsg ^ (if isexpect = 3 then splt.[splt.Length - 2]
                         else "")).Substring 1
    errmsg

/// The parse function, that create and return the AST generated (AST.Program), can also display errors
let rec parse input filename =
    let mutable source = input
    if source.ToString().EndsWith("\n") = false then source <- source ^ "\n"
    match (run pprog input) with
    | Success(result, _, _) -> result
    | Failure(_, error, _) ->
        let err = error.ToString()
        let ln = lineOfError err
        let col = colOfError err
        let line = (source.Split('\n').[ln - 1]).Trim()
        printf "%s" ("Parsing error in '" ^ filename ^ "': ")
        showFinalErr ln line col
        Errors.showErr ("→" ^ (completeErrorMessage err)) false
        let mutable si = 0
        let mutable l = 0
        for i = 1 to ln do
            si <- source.IndexOf('\n', si) + 1
            l <- l + 1
        source <- new string('\n', l) ^ source.Substring(si)
        parse source filename
