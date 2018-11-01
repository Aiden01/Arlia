module Parser
open AST
open FParsec

(* -------- Spaces and comments -------- *)

let max = System.Int32.MaxValue
let pcomment = (pstring "`" >>. skipCharsTillString "`" true max) <?> ""
let pspaces = spaces >>. many (spaces >>. pcomment >>. spaces)

let ws = pspaces >>. many (pspaces >>. pcomment >>. pspaces) |>> (fun _ -> ())
let ws1 = spaces1
let str_ws s = pstring s .>> ws
let str_ws1 s = pstring s .>> ws1

(* -------- Utils -------- *)

let comma = str_ws ","
let (<|>) a b = attempt a <|> b
let between' a b a' b' = between ((ws >>. str_ws a) <?> a') ((ws >>. str_ws b) <?> b')
let eol = (str_ws ";" <?> "end of line") <|> (preturn "" <?> "line break")

let leftp = "left parenthesis"
let rightp = "right parenthesis"
let lhook = "left hook"
let rhook = "right hook"
let lblock = "start block"
let rblock = "end block"

let typeassign  = str_ws ":" <?> "type assignment"
let valueassign = str_ws "=" <?> "value assignment"

(* -------- Identifiers -------- *)

let reserved = [ 
                    "var"; "func"; "let"; "type"; "is"; "isnt";
                    "module"; "while"; "for"; "each"; "return";
                    "continue"; "if"; "elif"; "else"; "extern";
                    "match"; "to"; "in"; "do"; "step"; "throw"; 
                    "try"; "catch"; "new"; "true"; "false";"λ";
                    "with"; "it"; "public"; "private"; "then" ;
                    "lambda"; "import"; "include"; "vanaur"
               ]

let pidentifierraw =
    let inline isIdentifierFirstChar c = isLetter c
    let inline isIdentifierChar c = isLetter c || isDigit c
    many1Satisfy2L isIdentifierFirstChar isIdentifierChar "identifier"

let pidentifier =
    pidentifierraw
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("Bad identifier: '" + s + "' is a reserved keyword")
        else preturn s

let ptypeidentifier = 
    pidentifierraw
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("Bad type identifier: '" + s + "' is a reserved keyword")
        else preturn s

let pidentifier_ws = pidentifier .>> ws

(* -------- Types assignment -------- *)

let typename = ptypeidentifier |>> TypeName <?> "type name"
let tupletype = between' "(" ")" leftp rightp (sepBy typename comma) |>> TupleType <?> "tuple type"
let typeNT = typename <|> tupletype
let typefuncdef = many (ws >>. typeNT >>. (str_ws "->") >>. ws >>. typeNT) |>> TypeFuncDef <?> "function interface"
let typeNTF = typeNT <|> typefuncdef

let generictype = ((between' "<" ">" "" "" (sepBy typeNTF comma)) |>> GenericType <?> ("generic type")) <|>
                  (ws >>% NoGenericType)

let TYPE = typeNTF

(* -------- Literals -------- *)

type Lit = NumberLiteralOptions
let numberFormat = Lit.AllowMinusSign ||| Lit.AllowFraction ||| Lit.AllowHexadecimal

let pnumber: Parser<Literal, unit> =
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
let plambdaargs = many1 (pidentifier .>> ws)

let pdefine =
    pipe2
        (pidentifier .>> ws1)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (fun name ty -> Define(name, ty))

let passign = pipe3 pidentifier_ws valueassign pexpr (fun var _ expr -> Assign(var, expr))

let pparams = (between' "(" ")" leftp rightp (sepBy pexpr comma))

let pinvoke = pidentifier_ws .>>. pparams |>> fun (name, parameters) -> Invoke(Identifier name, parameters)

let pternary' = 
    pipe3
        (str_ws "if" <?> "ternary operator" >>. pexpr)
        (str_ws "then" >>. ws >>. pexpr)
        (str_ws "else" >>. ws >>. pexpr)
        (fun cnd t f -> TernaryOp(Condition cnd, IfTrue t, IfFalse f))

let plambda =
    pipe2
        (ws >>. lambda >>. ws >>. plambdaargs)
        (ws >>. str_ws "->" >>. ws >>. pexpr)
        (fun args expr -> Lambda(args, expr))

let externstr = 
    let normalChar = satisfy (fun c -> c <> '"')
    between (str_ws "(" >>. ws >>. pstring "\"") (ws >>. pstring "\"" >>. str_ws ")") (manyChars normalChar) 

let pextern = 
    pipe3
        (str_ws1 "extern" <?> "external call" >>. ws >>. externstr)
        (ws >>. externstr)
        (ws >>. pparams)
        (fun dll func parameters -> Extern(dll, func, parameters))

let pvalue = pinvoke <|> pvalue''' <|> pternary' <|> plambda <|> pextern

type Assoc = Associativity

let opp = OperatorPrecedenceParser<Expr, unit, unit> ()
pexprimpl := opp.ExpressionParser <?> "expression"
let term = pvalue .>> ws <|> between' "(" ")" leftp rightp pexpr
opp.TermParser <- term
let inops   = [ "+"; "-"; "*"; "/"; "%"; "**"; "^+"; "^-"; "^*"; "^/"; "^%"; "&&";
                "||"; "&"; "|"; "^"; "=="; "!="; "<="; ">="; "<"; ">"; "to" ]
let preops  = [ "-"; "++"; "--"; "each" ]
let postops = [ "++"; "--" ]

for op in inops do opp.AddOperator(InfixOperator(op, ws, 1, Assoc.Left, fun x y -> InfixOp(x, op, y)))
for op in preops do opp.AddOperator(PrefixOperator(op, ws, 1, true, fun x -> PrefixOp(op, x)))
for op in postops do opp.AddOperator(PostfixOperator(op, ws, 1, true, fun x -> PostfixOp(x, op)))

opp.AddOperator(InfixOperator(".", ws, 1, Assoc.Left, fun l r -> Dot(l, ".", r)))

let pexpr' = between' "(" ")" leftp rightp pexpr

let newObjConstructor, newObjConstructorimpl = createParserForwardedToRef ()

newObjConstructorimpl := 
    (str_ws "new" >>. ws >>. ptypeidentifier >>= fun typeName ->
    between' "(" ")" leftp rightp (sepBy (pexpr <|> newObjConstructor) comma) |>> fun typeValues ->
    TypeConstructor(typeName, typeValues)) <?> ("constructor calling")

let passignment = attempt passign |>> fun c -> Assignment(c)

let pexpr'' = pexpr <|> newObjConstructor <|> pvalue <|> pternary' <|> plambda

(* -------- Statements -------- *)

let pstatement, pstatementimpl = createParserForwardedToRef ()
let psinglestatement = pstatement |>> fun statement -> [statement]

let pstatementblock =
    (psinglestatement) <|>
    (between' "{" "}" lblock rblock (many pstatement))

let pstatementblock' =
    (between' "{" "}" lblock rblock (many pstatement))
    
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
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier args ty value -> LetFuncDeclr(identifier, args, ty, Value value))

let plet = 
    pipe3
        ((str_ws "let" >>. ws >>. pidentifier) <|> (ws >>. pidentifier))
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier ty value -> LetDeclr(identifier, ty, Value value))
    
let pvar = 
    pipe3
        (str_ws "var" >>. ws >>. pidentifier)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        (ws >>. valueassign >>. ws >>. pexpr'')
        (fun identifier ty value -> VarDeclr(identifier, ty, Value value))

let pfunc =
    pipe4
        (str_ws "func" >>. ws >>. pidentifier)
        (ws >>. parglist)
        ((ws >>. typeassign >>. ws >>. ptypeidentifier |>> TypeName) <|> (ws >>% ImplicitType))
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

let pthrow = str_ws1 "throw" >>. pexpr |>> Throw
let ptry = str_ws "try" >>. pstatementblock |>> fun block -> Try block
let pcatch = pipe2 (str_ws "catch" >>. pdefine) pstatementblock (fun d block -> Catch(d, block))

(* -------- Jump statements -------- *)

let preturn = str_ws1 "return" >>. pexpr |>> fun e -> Return e
let pcontinue = str_ws "continue" |>> fun _ -> Continue

(* -------- Functions -------- *)

let pcall = pidentifier .>>. pparams |>> fun (name, parameters) -> FuncInvocation(name, parameters)

(* -------- Type declaration -------- *)

let pargconstruct = 
    pipe3
        (pidentifier)
        ((ws >>. typeassign >>. ws >>. TYPE) <|> (ws >>% ImplicitType))
        ((ws >>. valueassign >>. ws >>. pexpr |>> DefaultValueArg) <|> (ws >>% NoDefaultValueArg))
        (fun name ty optval -> ArgFieldConstructor(Define(name, ty), optval))

let pargconstructlist = (between' "(" ")" leftp rightp (sepBy pargconstruct comma)) <?>
                        ("constructor parameters")

let ptypeasclass =
    pipe4
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (ws >>. pargconstructlist)
        (pstatementblock')
        (fun identifier gens args members -> TypeAsClass((identifier, gens, args), members))

let ptypeasstruct =
    pipe3
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (ws >>. pargconstructlist)
        (fun identifier gens args -> TypeAsStruct(identifier, gens, args))

let ptypeasalias =
    pipe3
        (str_ws "type" >>. ws >>. ptypeidentifier)
        (ws >>. generictype)
        (valueassign >>. TYPE)
        (fun identifier gens alias -> TypeAsAlias(TypeName identifier, gens, alias))

(* -------- Processor -------- *)

let includefilename = 
    let normalChar = satisfy (fun c -> c <> '"')
    between (str_ws "\"") (str_ws "\"") (manyChars normalChar)

let pinclude = str_ws1 "include" >>. includefilename |>> fun filename -> Include filename

(* -------- Statement implementation -------- *)

let paction = pexpr'' |>> fun e -> AnonymousExpression e

pstatementimpl :=
    attempt (preturn .>> eol)        <|>    // return
    attempt (pcontinue .>> eol)      <|>    // continue
    attempt (pletfunc .>> eol)       <|>    // let ()
    attempt (plet .>> eol)           <|>    // let
    attempt (pvar .>> eol)           <|>    // var
    attempt (passignment .>> eol)    <|>    // a = b
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
pprogimpl := ws >>. (attempt (manyTill (pstatement) (eof <?> "")) |>> Program)
