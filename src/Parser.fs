module Parser
open AST
open FParsec
open System
open System.Reflection.Emit

(* -------- Spaces and comments -------- *)

let max = Int32.MaxValue
let pcomment = (pstring "`" >>. skipCharsTillString "`" true max) <?> ""
let pspaces = spaces >>. many (spaces >>. pcomment >>. spaces)

let ws = pspaces >>. many (pspaces >>. pcomment >>. pspaces) |>> (fun _ -> ())
let ws1 = spaces1
let str_ws s = pstring s .>> ws
let str_ws1 s = pstring s .>> ws1

(* -------- Utils -------- *)

let comma = (str_ws ",")
let (<|>) a b = (attempt a) <|> b
let between' a b a' b' = between ((ws >>. str_ws a) <?> a') ((ws >>. str_ws b) <?> b')
let eol = str_ws ";" <?> "end of line"

(* -------- Identifiers -------- *)

let reserved = [ 
                    "var"; "func"; "let"; "type"; "is"; "isnt";
                    "module"; "while"; "for"; "each"; "return";
                    "continue"; "if"; "elif"; "else"; "extern";
                    "match"; "to"; "in"; "do"; "step"; "throw"; 
                    "try"; "catch"; "new"; "true"; "false";
                    "with"; "it"; "public"; "private"; "then";
                    "lambda"; "λ"
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

let pliteral = (pnumber <|> pbool <|> str_wsliteral <|> pcharliteral <|> pid |>> Literal) <?> "literal"

let plist = between' "[" "]" "start hook" "end hook" (sepBy pexpr comma) |>> ListValue <?> "list"
let pvalue' = pliteral <|> plist
let ptuple = between' "(" ")" "start parenthesis" "end parenthesis" (sepBy pvalue' comma) |>> TupleValue <?> "tuple"
let pvalue'' = pliteral <|> ptuple
let plist' = between' "[" "]" "start hook" "end hook" (sepBy pvalue'' comma) |>> ListValue <?> "list"
let pvalue''' = pliteral <|> pvalue'' <|> plist'

(* -------- Expressions -------- *)

let ptype = ptypeidentifier

let lambda = ((str_ws "lambda") <|> (str_ws "λ")) <?> "lambda expression"
let plambdaargs = many1 (pidentifier .>> ws)

let pdefine =
    pipe2
        (pidentifier .>> ws1)
        ((ws >>. str_ws ":" >>. ws >>. ptype |>> TypeName) <|> (ws >>% ImplicitType))
        (fun name ty -> Define(Identifier.Identifier(name), Some ty))

let passign = pipe3 pidentifier_ws (str_ws "=") pexpr (fun var _ expr -> Assign(Identifier.VariableName(var), expr))

let pparams = between' "(" ")" "start parenthesis" "end parenthesis" (sepBy pexpr comma)

let pinvoke = pidentifier_ws .>>. pparams |>> fun (name, parameters) -> Invoke(Identifier(name), Some parameters)

let pternary' = 
    pipe3
        (str_ws "if" >>. pexpr)
        (str_ws "then" >>. ws >>. pexpr)
        (str_ws "else" >>. ws >>. pexpr)
        (fun cnd t f -> TernaryOp(Condition(cnd), IfTrue(t), IfFalse(f)))

let plambda =
    pipe2
        (ws >>. lambda >>. ws >>. plambdaargs)
        (ws >>. str_ws "->" >>. ws >>. pexpr)
        (fun args expr -> Lambda(args, expr))

let pvalue = pinvoke <|> pvalue''' <|> pternary' <|> plambda

type Assoc = Associativity

let opp = OperatorPrecedenceParser<Expr, unit, unit> ()
pexprimpl := opp.ExpressionParser <?> "expression"
let term = pvalue .>> ws <|> between (str_ws "(") (str_ws ")") pexpr
opp.TermParser <- term
let inops   = [ "+"; "-"; "*"; "/"; "%"; "**"; "^+"; "^-"; "^*"; "^/"; "^%"; "&&"; "||"; "&"; "|"; "^"; "=="; "!="; "<="; ">="; "<"; ">"; "?"; "to" ]
let preops  = [ "-"; "++"; "--" ]
let postops = [ "++"; "--" ]

for op in inops do opp.AddOperator(InfixOperator(op, ws, 1, Assoc.Left, fun x y -> InfixOp(x, op, y)))
for op in preops do opp.AddOperator(PrefixOperator(op, ws, 1, true, fun x -> PrefixOp(op, x)))
for op in postops do opp.AddOperator(PostfixOperator(op, ws, 1, true, fun x -> PostfixOp(x, op)))

opp.AddOperator(InfixOperator(".", ws, 1, Assoc.Left, fun l r -> Dot(l, ".", r)))

let pexpr' = between' "(" ")" "start parenthesis" "end parenthesis" pexpr

let newObjConstructor, newObjConstructorimpl = createParserForwardedToRef ()

let psupexpr = pexpr <|> newObjConstructor

newObjConstructorimpl := 
    (str_ws "new" >>. ws >>. ptypeidentifier >>= fun typeName ->
    between' "(" ")" "start parenthesis" "end parenthesis" (sepBy psupexpr comma) |>> fun typeValues ->
    TypeConstructor(Identifier.TypeName(typeName), typeValues)) <?> "constructor calling"

let passignment = attempt passign |>> fun c -> Assignment(c)

let pexpr'' = pexpr <|> newObjConstructor <|> pvalue <|> pternary' <|> plambda

(* -------- Statements -------- *)

let pstatement, pstatementimpl = createParserForwardedToRef ()
let psinglestatement = pstatement |>> fun statement -> [statement]

let pstatementblock =
    psinglestatement <|>
    between' "{" "}" "start block" "end block" (many pstatement)

let parg = 
    pipe3
        (pidentifier)
        ((ws >>. str_ws ":" >>. ws >>. ptypeidentifier |>> TypeName) <|> (ws >>% ImplicitType))
        ((ws >>. str_ws "=" >>. ws >>. pexpr |>> Expression) <|> (ws >>% Nothing))
        (fun name ty optval -> Arg(Define(Identifier.Identifier(name), Some ty), Some (DefaultValueArg(optval))))

let parglist = str_ws "(" >>. sepBy parg (str_ws ",") .>> str_ws ")" <?> "parameters"

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

let pfunc =
    pipe4
        (str_ws "func" >>. ws >>. pidentifier)
        (ws >>. parglist)
        ((ws >>. str_ws ":" >>. ws >>. ptype |>> TypeName) <|> (ws >>% ImplicitType))
        (pstatementblock)
        (fun identifier args ty block -> FuncDefinition(Identifier.FunctionName(identifier), Some args, Some ty, Block(block)))

(* -------- Selection statements -------- *)

let pif =
    pipe2
        (str_ws "if" >>. pexpr')
        (pstatementblock)
        (fun e block -> If(e, Some (Block(block))))

let pifelse =
    pipe3
        (str_ws "if" >>. pexpr')
        (pstatementblock)
        (str_ws "else" >>. pstatementblock)
        (fun e t f -> IfElse(e, Some (Block(t)), Some (Block(f))))

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
    pipe2 (str_ws "for" >>. between' "(" ")" "start parenthesis" "end parenthesis" pforstepargs) pstatementblock
          (fun (init, until, iter) block -> ForStep(init, To(until), Step(iter), Some (Block(block))))

let pfor =
    pipe2 (str_ws "for" >>. between' "(" ")" "start parenthesis" "end parenthesis" pforargs) pstatementblock
          (fun (init, until) block -> For(init, To(until), Some (Block(block))))

let pwhile =
    pipe2 (str_ws "while" >>. pexpr') pstatementblock
          (fun e block -> While(e, Some (Block(block))))

let pdowhile =
    pipe2 (str_ws "do" >>. pstatementblock)
          (str_ws "while" >>. pexpr')
          (fun block e -> DoWhile(Some (Block(block)), e))

(* -------- Exceptions -------- *)

let pthrow = str_ws1 "throw" >>. pexpr |>> Throw
let ptry = str_ws "try" >>. pstatementblock |>> fun block -> Try(Some (Block(block)))
let pcatch = pipe2 (str_ws "catch" >>. pdefine) pstatementblock (fun d block -> Catch(d, Some (Block(block))))

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

let pargconstruct = 
    pipe3
        (pidentifier)
        ((ws >>. str_ws ":" >>. ws >>. ptypeidentifier |>> TypeName) <|> (ws >>% ImplicitType))
        ((ws >>. str_ws "=" >>. ws >>. pexpr |>> Expression) <|> (ws >>% Nothing))
        (fun name ty optval -> ArgFieldConstructor(Define(Identifier.Identifier(name), Some ty), Some (DefaultValueArg(optval))))

let pargconstructlist = str_ws "(" >>. sepBy pargconstruct (str_ws ",") .>> str_ws ")"

let ptypeclass =
    pipe3
        (str_ws "type" >>. ws >>. pidentifier)
        (ws >>. pargconstructlist)
        (pstatementblock)
        (fun identifier args block -> TypeAsClass(Constructor(Define(Identifier.TypeName(identifier), None), Some args), Block(block)))

let ptypeasstruct =
    pipe2
        (str_ws "type" >>. ws >>. pidentifier)
        (ws >>. pargconstructlist)
        (fun identifier args -> TypeAsStruct(Constructor(Define(Identifier.TypeName(identifier), None), Some args)))

let ptypeasalias =
    pipe2
        (str_ws "type" >>. ws >>. pidentifier)
        (str_ws "=" >>. ptypeidentifier)
        (fun identifier alias -> TypeAsAlias(Identifier.TypeName(identifier), Identifier.TypeName(alias)))

(* -------- Statement implementation -------- *)

let paction = pexpr'' |>> fun e -> AnonymousExpression(e)

pstatementimpl :=
    attempt (paction .>> eol)        <|>
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
    attempt (pfunc)                  <|>
    attempt (pthrow .>> eol)         <|>
    attempt (ptry)                   <|>
    attempt (pcatch)                 <|>
    attempt (ptypeclass)             <|>
    attempt (ptypeasstruct .>> eol)  <|>
    attempt (ptypeasalias .>> eol)

(* -------- Program -------- *)

let pprog, pprogimpl = createParserForwardedToRef ()
pprogimpl := (attempt (manyTill (pstatement) (eof <?> "")) |>> Program)
