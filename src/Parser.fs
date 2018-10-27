module Parser
open AST
open FParsec
open System
open System.Reflection.Emit

(* -------- Spaces and comments -------- *)

let max = Int32.MaxValue
let pcomment = pstring "`" >>. skipCharsTillString "`" true max
let pspaces = spaces >>. many (spaces >>. pcomment >>. spaces)

let ws = pspaces >>. many (pspaces >>. pcomment >>. pspaces) |>> (fun _ -> ())
let ws1 = spaces1
let str_ws s = pstring s .>> ws
let str_ws1 s = pstring s .>> ws1

(* -------- Utils -------- *)

let comma = (str_ws ",")
let (<|>) a b = (attempt a) <|> b
let between' a b = between (ws >>. str_ws a) (ws >>. str_ws b)
let eol = str_ws ";"

(* -------- Identifiers -------- *)

let reserved = [ 
                    "var"; "func"; "let"; "type"; "is"; "isnt";
                    "module"; "while"; "for"; "each"; "return";
                    "continue"; "if"; "elif"; "else"; "extern";
                    "match"; "to"; "in"; "do"; "step"; "throw"; 
                    "try"; "catch"; "new"; "true"; "false";
                    "with"; "it"; "public"; "private"
               ]

let pidentifierraw =
    let inline isIdentifierFirstChar c = isLetter c
    let inline isIdentifierChar c = isLetter c || isDigit c
    many1Satisfy2L isIdentifierFirstChar isIdentifierChar "identifier"

let pidentifier =
    pidentifierraw
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("Bad identifier: '" + s + "' is as keyword")
        else preturn s

let ptypeidentifier = 
    pidentifierraw
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("Bad type identifier: '" + s + "' is a keyword")
        else preturn s

let pidentifier_ws = pidentifier .>> ws

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

let pexpr , pexprimpl = createParserForwardedToRef ()

let str_wsliteral =
    let normalChar = satisfy (fun c -> c <> '\\' && c <> '"')
    let escapedChar = str_ws "\\" >>. (anyOf "\\nrt\"" |>> unescape)
    between (str_ws "\"") (str_ws "\"")
            (manyChars (normalChar <|> escapedChar))
    |>> fun s -> Literal.String(s)

let pcharliteral   =
    let normalChar = satisfy (fun c -> c <> '\\' && c <> '"')
    let escapedChar = str_ws "\\" >>. (anyOf "\\nrt\"" |>> unescape)
    between (str_ws "'") (str_ws "'")
        (normalChar <|> escapedChar)
    |>> fun c -> Literal.Char(c)

let pid = pidentifier |>> Literal.Identifier

let pliteral = pnumber <|> pbool <|> str_wsliteral <|> pcharliteral <|> pid |>> Literal

let plist = between' "[" "]" (sepBy pexpr comma)            |>> ListValue
let pvalue' = pliteral <|> plist
let ptuple = between' "(" ")" (sepBy pvalue' comma)         |>> TupleValue
let pvalue'' = pliteral <|> ptuple
let plist' = between' "[" "]" (sepBy pvalue'' comma)        |>> ListValue
let pvalue''' = pliteral <|> pvalue'' <|> plist'

(* -------- Expressions -------- *)

let ptype = ptypeidentifier

let pdefine =
    pipe2
        (pidentifier .>> ws1)
        ((ws >>. str_ws ":" >>. ws >>. ptype |>> TypeName) <|> (ws >>% ImplicitType))
        (fun name ty -> Define(Identifier.Identifier(name), Some ty))

let passign = pipe3 pidentifier_ws (str_ws "=") pexpr (fun var _ expr -> Assign(Identifier.VariableName(var), expr))

let pparams = between' "(" ")" (sepBy pexpr comma)

let pinvoke = pidentifier_ws .>>. pparams |>> fun (name, parameters) -> Invoke(Identifier(name), Some parameters)

let pvalue = pinvoke <|> pvalue'''

type Assoc = Associativity

let opp = OperatorPrecedenceParser<Expr, unit, unit> ()
pexprimpl := opp.ExpressionParser
let term = pvalue .>> ws <|> between (str_ws "(") (str_ws ")") pexpr
opp.TermParser <- term
let inops   = [ "+"; "-"; "*"; "/"; "%"; "**"; "^+"; "^-"; "^*"; "^/"; "^%"; "&&"; "||"; "&"; "|"; "^"; "=="; "!="; "<="; ">="; "<"; ">"; "?"; "." ]
let preops  = [ "-"; "++"; "--" ]
let postops = [ "++"; "--" ]

for op in inops do opp.AddOperator(InfixOperator(op, ws, 1, Assoc.Left, fun x y -> InfixOp(x, op, y)))
for op in preops do opp.AddOperator(PrefixOperator(op, ws, 1, true, fun x -> PrefixOp(op, x)))
for op in postops do opp.AddOperator(PostfixOperator(op, ws, 1, true, fun x -> PostfixOp(x, op)))

let pexpr' = between (str_ws "(") (str_ws ")") pexpr

(* -------- Statements -------- *)

let pstatement, pstatementimpl = createParserForwardedToRef ()
let psinglestatement = pstatement |>> fun statement -> [statement]

let pstatementblock =
    psinglestatement <|>
    between' "{" "}" (many pstatement)

let newObjConstructor, newObjConstructorimpl = createParserForwardedToRef ()

let psupexpr = pexpr <|> newObjConstructor

newObjConstructorimpl := 
    (str_ws "new" >>. ws >>. ptypeidentifier >>= fun typeName ->
    between' "(" ")" (sepBy psupexpr comma) |>> fun typeValues ->
    TypeConstructor(Identifier.TypeName(typeName), typeValues))

let passignment = attempt passign |>> fun c -> Assignment(c)

let pexpr'' = pexpr <|> newObjConstructor <|> pvalue

let parg = 
    pipe3
        (pidentifier)
        ((ws >>. str_ws ":" >>. ws >>. ptypeidentifier |>> TypeName) <|> (ws >>% ImplicitType))
        ((ws >>. str_ws "=" >>. ws >>. pexpr |>> Expression) <|> (ws >>% Nothing))
        (fun name ty optval -> Arg(Define(Identifier.Identifier(name), Some ty), Some (DefaultValueArg(optval))))

let parglist = str_ws "(" >>. sepBy parg (str_ws ",") .>> str_ws ")"

let pletfunc =
    pipe4
        (str_ws "let" >>. ws >>. pidentifier)
        (ws >>. parglist)
        ((ws >>. str_ws ":" >>. ws >>. ptype |>> TypeName) <|> (ws >>% ImplicitType))
        (ws >>. str_ws "=" >>. ws >>. pexpr'')
        (fun identifier args ty value -> LetFuncDeclr(Identifier.LetName(identifier), args, Some ty, Value(value)))

let plet = 
    pipe3
        (str_ws "let" >>. ws >>. pidentifier)
        ((ws >>. str_ws ":" >>. ws >>. ptype |>> TypeName) <|> (ws >>% ImplicitType))
        (ws >>. str_ws "=" >>. ws >>. pexpr'')
        (fun identifier ty value -> LetDeclr(Identifier.Identifier(identifier), ty, Value(value)))
    
let pvar = 
    pipe3
        (str_ws "var" >>. ws >>. pidentifier)
        ((ws >>. str_ws ":" >>. ws >>. ptype |>> TypeName) <|> (ws >>% ImplicitType))
        (ws >>. str_ws "=" >>. ws >>. pexpr'')
        (fun identifier ty value -> VarDeclr(Identifier.VariableName(identifier), ty, Value(value)))

(* -------- Selection statements -------- *)

let pif = pipe2 (str_ws "if" >>. pexpr') pstatementblock
                (fun e block -> If(e, Some block))

let pifelse = pipe3 (str_ws "if" >>. pexpr') pstatementblock (str_ws "else" >>. pstatementblock)
                    (fun e t f -> IfElse(e, Some t, Some f))

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
    pipe2 (str_ws "for" >>. between' "(" ")" pforstepargs) pstatementblock
          (fun (init, until, iter) block -> ForStep(init, To(until), Step(iter), Some block))

let pfor =
    pipe2 (str_ws "for" >>. between' "(" ")" pforargs) pstatementblock
          (fun (init, until) block -> For(init, To(until), Some block))

let pwhile =
    pipe2 (str_ws "while" >>. pexpr') pstatementblock
          (fun e block -> While(e, Some block))

let pdowhile =
    pipe2 (str_ws "do" >>. pstatementblock)
          (str_ws "while" >>. pexpr')
          (fun block e -> DoWhile(Some block, e))

(* -------- Exceptions -------- *)

let pthrow = str_ws1 "throw" >>. pexpr |>> Throw
let ptry = str_ws "try" >>. pstatementblock |>> fun block -> Try(Some block)
let pcatch = pipe2 (str_ws "catch" >>. pdefine) pstatementblock (fun d block -> Catch(d, Some block))

(* -------- Jump statements -------- *)

let preturn = str_ws1 "return" >>. pexpr |>> fun e -> Return(e)
let pcontinue = str_ws "continue" |>> fun _ -> Continue

(* -------- Functions -------- *)

let pcall = pidentifier .>>. (pparams) |>> fun (name, parameters) -> FuncInvoke(Identifier(name), Some parameters)

// Access

let ppublic = str_ws "public"   |>> fun _ -> Public
let pprivate = str_ws "private" |>> fun _ -> Private
let paccess = opt (ppublic <|> pprivate)

(* -------- Type declaration -------- *)



(* -------- Statement implementation -------- *)

let paction = pexpr |>> fun e -> Action(e)

pstatementimpl :=
    attempt (preturn .>> eol)        <|>
    attempt (pcontinue .>> eol)      <|>
    attempt (pletfunc .>> eol)       <|>
    attempt (plet .>> eol)           <|>
    attempt (pvar .>> eol)           <|>
    attempt (passignment .>> eol)    <|>
    attempt (pifelse)                <|>
    attempt (pif)                    <|>
    attempt (pfor)                   <|>
    attempt (pforstep)               <|>
    attempt (pwhile)                 <|>
    attempt (pdowhile)               <|>
    attempt (pcall .>> eol)          <|>
    attempt (pthrow .>> eol)         <|>
    attempt (ptry)                   <|>
    attempt (pcatch)

(* -------- Program -------- *)

let pprog, pprogimpl = createParserForwardedToRef ()
pprogimpl := many pstatement |>> Program
