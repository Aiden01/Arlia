module Parser
open AST
open FParsec
open System
open FParsec
open System.Diagnostics

(* Spaces and comments *)

let max = System.Int32.MaxValue
let pcomment = pstring "`" >>. skipCharsTillString "`" true max
let pspaces = spaces >>. many (spaces >>. pcomment >>. spaces)

let ws = pspaces >>. many (pspaces >>. pcomment >>. pspaces) |>> (fun _ -> ())
let ws1 = spaces1
let str_ws s = pstring s .>> ws
let str_ws1 s = pstring s .>> ws1

(* Literals *)

type Lit = NumberLiteralOptions
let numberFormat = Lit.AllowMinusSign
               ||| Lit.AllowFraction
               ||| Lit.AllowHexadecimal
               ||| Lit.AllowInfinity
               ||| Lit.AllowNaN
let pnumber: Parser<Literal, unit> =
    numberLiteral numberFormat "number"
    |>> fun nl ->
        if nl.IsInteger then Literal(int, nl.String)
        else Literal(float, nl.String)
let ptrue = str_ws "true" |>> fun _ -> Literal(true)
let pfalse = str_ws "false" |>> fun _ -> Literal(false)
let pbool = ptrue <|> pfalse
let pstringliteral =
    let normalChar = satisfy (fun c -> c <> '\\' && c <> '"')
    let unescape c = match c with
                     | 'n' -> '\n'
                     | 'r' -> '\r'
                     | 't' -> '\t'
                     | c   -> c
    let escapedChar = pstring "\\" >>. (anyOf "\\nrt\"" |>> unescape)
    between (pstring "\"") (pstring "\"")
            (manyChars (normalChar <|> escapedChar))
    |>> fun s -> Literal(s)

let pliteral = pnumber <|> pbool <|> pstringliteral

(* Expressions *)

let pexpr, pexprimpl = createParserForwardedToRef ()

let reserved = [ 
                    "var"; "func"; "let"; "type"; "is"; "isnt";
                    "namespace"; "module"; "while"; "for"; "each";
                    "return"; "continue"; "if"; "elif"; "else";
                    "extern"; "match"; "to"; "in"; "do"; "step";
                    "throw"; "try"; "catch"; "new"; "true"; "false"; 
                    "with"; "it"; "public"; "private"; "None"
               ]
let pidentifierraw =
    let isIdentifierFirstChar c = isLetter c
    let isIdentifierChar c = isLetter c || isDigit c
    many1Satisfy2L isIdentifierFirstChar isIdentifierChar "identifier"
let pidentifier =
    pidentifierraw
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("Bad identifier: '" + s + "' is as keyword") // "keyword"
        else preturn s
let pidentifier_ws = pidentifier .>> ws
//let pvar = pidentifier |>> fun x -> Variable(x)
let pvar = str_ws1 "var" >>. pidentifier |>> fun x -> Variable(x)

let pargref = str_ws1 "&" |>> fun _ -> RefArg
let pargtype = (opt pargref)
               |>> function Some x -> x | None -> ValueArg
let parg = pargtype .>>. pexpr |>> fun (by, e) -> Arg(by, e)
let pinvoke =
    pidentifier_ws .>>.
    between (str_ws "(") (str_ws ")") (many parg)
    |>> fun (name, args) -> FunctionInvoke(name, args)

let pvalue = (pliteral |>> fun x -> Value(x)) <|>
                              attempt pinvoke <|>
                              attempt pvar

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

(* Statement blocks *)

let pstatement, pstatementimpl = createParserForwardedToRef ()
let psinglestatement = pstatement |>> fun statement -> [statement]
let pstatementblock =
    psinglestatement <|>
    between (str_ws "{") (str_ws "}") (many pstatement)

let pdefine_var = pipe2 (pidentifier .>> ws1) (pidentifier)
                    (fun name ty -> Define(name, Some ty))
let pdefine_let = pipe2 (pidentifier .>> ws1) (pidentifier)
                    (fun name ty -> Define(name, Some ty))

let pref = str_ws "&" |>> fun _ -> ByRef
let pby = (opt pref) |>> function Some x -> x | None -> ByValue
let pparam =
    pipe3 pby pidentifier_ws pidentifier_ws
        (fun by name ty -> Param(by, name, Some ty))
let pparamlist = str_ws "(" >>. sepBy pparam (str_ws ",") .>> str_ws ")"

let pvardefinition =
    pipe2 (str_ws "var" >>. pdefine_var) pexpr
        (fun def expr -> VarDefinition(def, expr))

let pletdefinition =
    pipe3 (str_ws "let" >>. pdefine_let) pparamlist pexpr
        (fun def ps expr -> LetDefinition(def, Some ps, expr))

let pdefine = pipe2 (pidentifier .>> ws1) (pidentifier)
                    (fun name ty -> Define(name, Some ty))

let pdefinition_var = pdefine_var |>> fun d expr -> VarDefinition(d, expr)
let pdefinition_let = pdefine_let |>> fun d pl expr -> LetDefinition(d, Some pl, expr)

let passign = pipe3 pidentifier_ws (str_ws "=") pexpr                   // a = b
                    (fun var _ expr -> Assign(var, expr))
let pmut = pipe3 pidentifier_ws (str_ws "<-") pexpr                     // a <- b
                    (fun var _ expr -> Assign(var, expr))

let passignment = attempt passign <|> attempt pmut |>> fun c -> Assignment(c)

(* Selection statements *)

let pif =
    pipe2 (str_ws "if" >>. pexpr') pstatementblock
        (fun e block -> If(e, Some block))

let pifelse = 
    pipe3 (str_ws "if" >>. pexpr') pstatementblock (str_ws "else" >>. pstatementblock)
        (fun e t f -> IfElse(e, Some t, Some f))

(*  Iteration statements *)

let pforargs =
    let pinit = attempt passign
    pipe3
        (pinit .>> str_ws "to")
        (pexpr .>> str_ws "step")
        (pexpr)
        (fun from until steps -> from, until, steps)

let pfor =
    pipe2 (str_ws "for" >>. between (str_ws "(") (str_ws ")") pforargs)
        pstatementblock
            (fun (init, until, iter) block -> For(init, until, Some iter, Some block))

let pforeachargs =
    pipe3 pdefine (str_ws1 "in") pexpr
        (fun define _ collection -> define, collection)

let pforeach =
    pipe2 (str_ws "for each" >>. pforeachargs) pstatementblock
        (fun (define, collection) block -> ForEach(define, collection, Some block))

let pwhile =
    pipe2 (str_ws "while" >>. pexpr') pstatementblock
        (fun e block -> While(e, Some block))

let pdowhile =
    pipe2
        (str_ws "do" >>. pstatementblock)
        (str_ws "while" >>. pexpr')
        (fun block e -> DoWhile(Some block, e))

(* Jump statements *)

let preturn = str_ws1 "return" >>. pexpr |>> fun e -> Return(e)
let pcontinue = str_ws "continue" |>> fun _ -> Continue

(* Statement implementation *)

let paction = pexpr |>> fun e -> Action(e)

pstatementimpl :=
    attempt (preturn .>> str_ws ";") <|>
    attempt (pcontinue .>> str_ws ";") <|>
    attempt (pvardefinition .>> str_ws ";") <|>
    attempt (pletdefinition .>> str_ws ";") <|>
    attempt (passignment .>> str_ws ";") <|>
    attempt pifelse <|> attempt pif <|>
    attempt pfor <|> attempt pforeach <|>
    attempt pwhile <|> attempt pdowhile

