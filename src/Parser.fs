module Parser
open AST
open FParsec


// Todo:
//  - Sum type
//  - References
//  - End of line
//  - Wildcard as parameter
//  - Patterns
//  - Reserved types: Integer, Float, Boolean, Char, String, Real
//  - Best module identifier
//  - Display entier error message
//  - Importations
//  - (new T())


// The Arlia parser is based on the FParsec library
// (https://github.com/stephan-tolksdorf/fparsec)

(* -------- Spaces and comments -------- *)

let max = System.Int32.MaxValue

let pcomment = (pstring "`" >>. skipCharsTillString "`" true max) <?> ""

let pspaces = spaces >>. many (spaces >>. pcomment >>. spaces)

let ws = pspaces >>. many (pspaces >>. pcomment >>. pspaces) |>> (fun _ -> ())

let ws1 = spaces1

let str_ws s = pstring s .>> ws

let str_ws1 s = pstring s .>> ws1

(* -------- Utils -------- *)

let (<|>) a b = attempt a <|> b

let comma = str_ws "," <?> "comma"

let between' a b a' b' = between ((ws >>. str_ws a) <?> a') ((ws >>. str_ws b) <?> b')

let eol = (str_ws ";" <?> "end of line")

let leftp    = "left parenthesis"
let rightp   = "right parenthesis"
let lhook    = "left hook"
let rhook    = "right hook"
let lblock   = "start block"
let rblock   = "end block"

let typeassign   = str_ws ":"  <?> "type assignment"
let typereturn   = str_ws "->" <?> "type return"
let valueassign  = str_ws "="  <?> "value assignment"
let valuestorage = str_ws "<-" <?> "value storage"

(* -------- Identifiers -------- *)

let reserved = [ 
                    "var"; "func"; "let"; "type"; "module";
                    "while"; "for"; "each"; "return"; "continue";
                    "if"; "elif"; "else"; "extern"; "match";
                    "to"; "in"; "do"; "step"; "throw"; "try";
                    "catch"; "new"; "true"; "false";"λ"; "with";
                    "it"; "public"; "private"; "then" ; "lambda";
                    "import"; "include"; "vanaur";
               ]

let pidentifierraw =
    let inline isIdentifierFirstChar c = isLetter c
    let inline isIdentifierChar c = isLetter c || isDigit c
    many1Satisfy2L isIdentifierFirstChar isIdentifierChar "identifier"

let pidentifier =
    pidentifierraw
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("'" ^ s ^ "' is not expected here")
        else preturn s

let ptypeidentifier = pidentifier

let pidentifier_ws = pidentifier .>> ws

(* -------- Types assignment -------- *)

let typename = ptypeidentifier |>> TypeName <?> "type name"
let tupletype = between' "(" ")" leftp rightp (sepBy typename comma) |>> TupleType <?> "tuple type"
let typeNT = typename <|> tupletype
let typefuncdef = 
    pipe2
        (between' "(" ")" leftp rightp (sepBy (ws >>. typeNT) comma))
        (str_ws "->" >>. ws >>. typeNT)
        (fun args ret -> TypeFuncDef(args, ret)) <?> "function interface"
let gentype = (between' "<" ">" "generic type" "generic type" (sepBy typeNT comma)) |>>
              (GenericType.GenericType) <?> "generic type" <|> (ws >>% GenericType.NoGenericType)

let typeNTF = typefuncdef <|> typeNT

let TYPE = typeNTF

let generictype = ((between' "<" ">" "" "" (sepBy typeNTF comma)) |>> GenericType.GenericType <?> ("generic type")) <|>
                  (ws >>% NoGenericType)

(* -------- Literals -------- *)

type Lit = NumberLiteralOptions
let numberFormat = Lit.AllowMinusSign ||| Lit.AllowFraction ||| Lit.AllowHexadecimal

let pnumber =
    numberLiteral numberFormat "number"
    |>> fun nl ->
            if   nl.IsInteger then Int(int nl.String)
            else                   Float(float nl.String)

let ptrue   = str_ws "true"  |>> fun _ -> Bool(true)
let pfalse  = str_ws "false" |>> fun _ -> Bool(false)

let pbool   = ptrue <|> pfalse

let inline unescape c = match c with
                        | 'n' -> '\n'
                        | 'r' -> '\r'
                        | 't' -> '\t'
                        | c   -> c

let pexpr, pexprimpl = createParserForwardedToRef ()

let pstringliteral =
    let normalChar = satisfy (fun c -> c <> '\\' && c <> '"')
    let escapedChar = pstring "\\" >>. (anyOf "\\nrt\"" |>> unescape)
    between (pstring "\"") (pstring "\"")
            (manyChars (normalChar <|> escapedChar))
    |>> fun s -> Literal.String(s)

let pcharliteral   =
    let normalChar = satisfy (fun c -> c <> '\\' && c <> '"')
    let escapedChar = str_ws "\\" >>. (anyOf "\\nrt\"" |>> unescape)
    between (str_ws "'") (str_ws "'")
        (normalChar <|> escapedChar)
    |>> fun c -> Literal.Char(c)

let pid = pidentifier |>> Literal.Identifier

let pliteral = (pnumber <|> pbool <|> pstringliteral <|> pcharliteral <|> pid |>> Literal) <?> "literal"

let plist = between' "[" "]" lhook rhook (sepBy pexpr comma)     |>> ListValue  <?> "list"
let pvalue' = pliteral <|> plist
let ptuple = between' "(" ")" leftp rightp (sepBy pvalue' comma) |>> TupleValue <?> "tuple"
let pvalue'' = pvalue' <|> ptuple
let plist' = between' "[" "]" lhook rhook (sepBy pvalue'' comma) |>> ListValue  <?> "list"
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
let passign = pipe3 pidentifier_ws valueassign pexpr (fun var _ expr -> (var, expr))

let pparams = (between' "(" ")" leftp rightp (sepBy pexpr comma))

let pinvoke = pidentifier_ws .>>. pparams
              |>> fun (name, parameters) -> Invoke(Identifier name, parameters)

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
        (fun dll func parameters -> Extern(dll, func, parameters))

let pwildcard = 
    ws >>. str_ws "|" >>. ws >>. str_ws "_" >>. ws >>. str_ws "=>" >>. ws >>. pexpr
    |>> (fun e -> Wildcard e) <?> "wildcard" <|>
        (ws >>% NoWildcard)

let ppattern = 0

let pcase =
    pipe2
        (ws >>. str_ws "|" >>. ws >>. pexpr)    // pattern
        (ws >>. str_ws "=>" >>. ws >>. pexpr)   // if matched
        (fun x y -> Case(x, y))

let pcaselist = many1 pcase

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

pexprimpl := opp.ExpressionParser <?> "expression"

let term = pvalue .>> ws <|> between' "(" ")" leftp rightp pexpr

opp.TermParser <- term

/// Inflix operators
let inops = [ "+"; "-"; "*"; "/"; "%"; "**"; "^+"; "^-"; "^*"; "^/"; "^%"; "&&";
              "||"; "&"; "^"; "=="; "!="; "<="; ">="; "<"; ">"; "to"; "step" ]

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

let pexpr' = between' "(" ")" leftp rightp pexpr

let pexpr'' = pexpr             <|>     // expressions
              newObjConstructor <|>     // type constructor
              pvalue            <|>     // value (literal, tuple or list)
              pternary          <|>     // ternary operator
              plambda                   // lambda expression

let tokops = [ "+"; "-"; "*"; "/"; "%"; "|"; "<"; ">"; "."; "$"; "~"; "^"; "!"; "="; "°"; "@"; "?" ]

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
    
let parg = 
    pipe3
        (pidentifier)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        ((ws >>. valueassign >>. ws >>. pexpr |>> DefaultValueArg) <|> (ws >>% NoDefaultValueArg))
        (fun name ty optval -> Arg(Define(name, ty), optval))

let parglist = between' "(" ")" leftp rightp (sepBy parg (str_ws "," <?> "other parameters")) <?> "parameters"

let pletfunc =
    pipe4
        ((str_ws "let" >>. ws >>. pidentifier) <|> (ws >>. pidentifier))
        (ws >>. parglist)
        ((ws >>. typereturn >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier args ty value -> LetFuncDeclr(identifier, args, ty, Value value))

let plet = 
    pipe4
        ((str_ws "let" >>. ws >>. pidentifier) <|> (ws >>. pidentifier))
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (gentype)
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier ty genty value -> LetDeclr(identifier, ty, genty, Value value))
    
let pvar = 
    pipe4
        (str_ws "var" >>. ws >>. pidentifier)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (gentype)
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier ty genty value -> VarDeclr(identifier, ty, genty, Value value))

let pfunc =
    pipe4
        (str_ws "func" >>. ws >>. pidentifier)
        (ws >>. parglist)
        ((ws >>. typereturn >>. ws >>. ptypeidentifier |>> TypeName) <|> (ws >>% ImplicitType))
        (pstatementblock')
        (fun identifier args ty block -> FuncDefinition(identifier, args, ty, block))

(* -------- Selection statements -------- *)

let pif =
    pipe2
        (str_ws "if" >>. pexpr')
        (pstatementblock)
        (fun e block -> If(e, block))

let pifelse =
    pipe3
        (str_ws "if" >>. pexpr')
        (pstatementblock)
        (str_ws "else" >>. pstatementblock)
        (fun i ib eb -> IfElse(i, ib, eb))

(* -------- Iteration statements -------- *)

let pforstepargs =
    let pint = attempt passign
    pipe3 (pint .>> str_ws "to")
          (pexpr .>> str_ws "step")
          (pexpr)
          (fun from until steps -> from, until, steps)

let pforargs =
    let pint = attempt passign
    pipe2 (pint .>> str_ws "to")
          (pexpr)
          (fun from until -> from, until)

let pforstep =
    pipe2 (str_ws "for" >>. between' "(" ")" leftp rightp pforstepargs) pstatementblock
          (fun (init, until, iter) block -> ForStep(init, To until, Step iter, block))

let pfor =
    pipe2 (str_ws "for" >>. between' "(" ")" leftp rightp pforargs) pstatementblock
          (fun (init, until) block -> For(init, To until, block))

let pwhile =
    pipe2 (str_ws "while" >>. pexpr') pstatementblock
          (fun e block -> While(e, block))

let pdowhile =
    pipe2 (str_ws "do" >>. pstatementblock)
          (str_ws "while" >>. pexpr')
          (fun block w -> DoWhile(block, w))

(* -------- Exceptions -------- *)

let pthrow = str_ws1 "throw" >>. pexpr'' |>> Throw

let ptry = str_ws "try" >>. pstatementblock |>> fun block -> Try block

let pcatch =
    pipe2
        (str_ws "catch" >>. between' "(" ")" leftp rightp pdefine)
        (pstatementblock)
        (fun d block -> Catch(d, block))

(* -------- Jump statements -------- *)

let preturn = str_ws1 "return" >>. pexpr'' |>> Return

let pcontinue = str_ws "continue" |>> fun _ -> Continue

(* -------- Functions -------- *)

let pcall = pidentifier .>>. pparams |>> fun (name, parameters) -> FuncInvocation(name, parameters)

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
        (fun identifier gens args members -> TypeAsClass((identifier, gens, args), members))

/// Type as structure constructor
let ptypeasstruct =
    pipe3
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (ws >>. pargconstructlist)
        (fun identifier gens args -> TypeAsStruct(identifier, gens, args))

/// Type as alias
let ptypeasalias =
    pipe3
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (valueassign >>. TYPE)
        (fun identifier gens alias -> TypeAsAlias(TypeName identifier, gens, alias))

(* -------- Scope -------- *)

let pmoduleblock = many pstatement

let pmodulename =  pexpr''

let pmodule = 
    pipe2
        (str_ws "module" >>. ws >>. pmodulename)
        (ws >>. pmoduleblock)
        (fun name block -> Module(name, block))

(* -------- Processor -------- *)

let includefilename = 
    let normalChar = satisfy (fun c -> c <> '"')
    between (str_ws "\"") (str_ws "\"") (manyChars normalChar)

let pinclude = str_ws1 "include" >>. includefilename |>> Include

(* -------- Statement implementation -------- *)

let paction = pmatch <|> pexpr'' |>> AnonymousExpression

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
    attempt (pthrow .>> eol)         <|>    // throw
    attempt (ptry)                   <|>    // try
    attempt (pcatch)                 <|>    // catch
    attempt (ptypeasclass)           <|>    // type () {}
    attempt (ptypeasstruct .>> eol)  <|>    // type ()
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
    attempt (ptry)                   <|>    // try
    attempt (pcatch)                 <|>    // catch
    attempt (ptypeasclass)           <|>    // type () {}
    attempt (ptypeasstruct .>> eol)  <|>    // type ()
    attempt (ptypeasalias .>> eol)   <|>    // type
    attempt (pinclude .>> eol)       <|>    // include
    attempt (paction .>> eol)               // expr

(* -------- Program -------- *)

let pprog, pprogimpl = createParserForwardedToRef ()
pprogimpl := ws >>. (attempt (manyTill (pstatement <|> pmodule) (eof <?> "")) |>> Program)

(* -------- Parse function call -------- *)

let showFinalErr (n: int) (s: string) (i: int) =
    printfn "\n%s"  ((n - 1).ToString() ^ " |")
    printfn "%s"    ((n.ToString()) ^ " | " ^ s)
    printf  "%s"    ((n + 1).ToString() ^ " |")
    System.Console.ForegroundColor <- System.ConsoleColor.Red
    printfn "%s"    (new string(' ', i) ^ "↑")
    System.Console.ResetColor()

let getErrorPosResult (msg: string): string =
    let n' = msg.LastIndexOf("Error in")
    let n = msg.Substring(0, n')
    msg.Substring(n', n.IndexOf("\n"))

let lineOfError msg = 
    let err = getErrorPosResult msg 
    System.Int32.Parse(err.Substring(err.IndexOf(": ") + 2, err.Substring(err.IndexOf(": ")).IndexOf(" Col") - 2))

let colOfError msg = 
    let err = getErrorPosResult msg
    System.Int32.Parse(err.Substring(err.IndexOf("Col: ") + 5))

let rec parse input filename =
    let mutable source = input
    if source.ToString().EndsWith("\n") = false then source <- source ^ "\n"
    match (run pprog input) with
    | Success(result, _, _) -> result
    | Failure(_, error, _) ->
        let err = error.ToString()
        let ln  = lineOfError err
        let col = colOfError err
        let line = (source.Split('\n').[ln - 1]).Trim()
        printf  "%s" ("Parsing error in '" ^ filename ^ "': ")
        showFinalErr ln line col
        Errors.showErr ("→" ^ err.Split('\n').[err.Split('\n').Length - 2].Substring(1) ^ "\n") false

        let mutable si = 0
        let mutable l = 0
        for i = 1 to ln do
            si <- source.IndexOf('\n', si) + 1
            l <- l + 1
        source <- new string('\n', l) ^ source.Substring(si)

        parse source filename
