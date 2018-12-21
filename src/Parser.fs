module Parser

open FParsec
open AST

// Indentation module from 'https://github.com/stephan-tolksdorf/fparsec/wiki/Parsing-indentation-based-syntax-with-FParsec'
module IndentationParserWithoutBacktracking =
    type LastParsedIndentation() =
        [<DefaultValue>]
        val mutable Value: int32
        [<DefaultValue>]
        val mutable EndIndex: int64

    let tabStopDistance = 8 // must be a power of 2

    type UserState = 
        {Indentation: int
         // We put LastParsedIndentation into the UserState so that we 
         // can conveniently use a separate instance for each stream.
         // The members of the LastParsedIndentation instance will be mutated
         // directly and hence won't be affected by any stream backtracking. 
         LastParsedIndentation: LastParsedIndentation}
        with
           static member Create() = {Indentation = -1
                                     LastParsedIndentation = LastParsedIndentation(EndIndex = -1L)}

    type CharStream = CharStream<UserState>
    type Parser<'t> = Parser<'t, UserState>

    // If this function is called at the same index in the stream
    // where the function previously stopped, then the previously
    // returned indentation will be returned again. 
    // This way we can avoid backtracking at the end of indented blocks.
    let skipIndentation (stream: CharStream) =    
        let lastParsedIndentation = stream.UserState.LastParsedIndentation
        if lastParsedIndentation.EndIndex = stream.Index then
            lastParsedIndentation.Value
        else
            let mutable indentation = stream.SkipNewlineThenWhitespace(tabStopDistance, false)
            while stream.Peek() = '#' do
                stream.SkipRestOfLine(false) // skip comment
                indentation <- stream.SkipNewlineThenWhitespace(tabStopDistance, false)
            lastParsedIndentation.EndIndex <- stream.Index
            lastParsedIndentation.Value <- indentation
            indentation

    let indentedMany1 (p: Parser<'t>) label : Parser<'t list> =
        fun stream ->
            let oldIndentation = stream.UserState.Indentation
            let indentation = skipIndentation stream
            if indentation <= oldIndentation then 
                Reply(Error, expected (if indentation < 0 then "newline" else "indented " + label))
            else
                stream.UserState <- {stream.UserState with Indentation = indentation}            
                let results = ResizeArray()
                let mutable stateTag = stream.StateTag
                let mutable reply = p stream // parse the first element
                let mutable newIndentation = 0
                while reply.Status = Ok 
                      && (results.Add(reply.Result)
                          newIndentation <- skipIndentation stream
                          newIndentation = indentation)
                   do
                     stateTag <- stream.StateTag
                     reply <- p stream
                if reply.Status = Ok 
                   || (stream.IsEndOfStream && results.Count > 0 && stream.StateTag = stateTag) 
                then
                    if newIndentation < indentation || stream.IsEndOfStream then
                        stream.UserState <- {stream.UserState with Indentation = oldIndentation}
                        Reply(List.ofSeq results)
                    else
                        Reply(Error, messageError "wrong indentation")
                else // p failed
                    Reply(reply.Status, reply.Error) 

open IndentationParserWithoutBacktracking

let isBlank = fun c -> c = ' ' || c = '\t'

let ws = skipManySatisfy isBlank
let str s = pstring s .>> ws
let ws1 = skipMany1SatisfyL isBlank "whitespace"

let comment = // Comments are only tags between '`' : ` my comment `
    (pstring "`" >>. skipCharsTillString "`" true (System.Int32.MaxValue))

let wsBeforeEOL = skipManySatisfy isBlank >>. optional comment
let keyword str = pstring str >>? nextCharSatisfiesNot (fun c -> isLetter c || isDigit c) <?> str

let betweenParentheses p =
    (between (str "(") (str ")") (ws >>. p)) <?> ""

let betweenHooks p =
    (between (str "[") (str "]") (ws >>. p)) <?> ""

// Literals

let reserved =
    [ "var"; "func"; "let"; "type"; "module"; "while"; "for"; "extrude";
      "exposing"; "continue"; "if"; "else"; "elif"; "extern"; "match"; "to";
      "in"; "do"; "step"; "throw"; "try"; "new"; "true"; "false"; "λ"; "with";
      "public"; "infix"; "postfix"; "prefix"; "private"; "then"; "lambda";
      "import"; "include"; "vanaur"; "await"; "sync" ]

let numberFormat =  NumberLiteralOptions.AllowMinusSign     |||
                    NumberLiteralOptions.AllowFraction      |||
                    NumberLiteralOptions.AllowHexadecimal   |||
                    NumberLiteralOptions.AllowOctal         |||
                    NumberLiteralOptions.AllowBinary

let literal_numeric =
    numberLiteral numberFormat "number" |>> fun nl ->
        if nl.IsInteger then Literal (Int(int nl.String))
        else Literal (Float(float nl.String))

let literal_bool =
    (choice [
        (stringReturn "true" (Literal (Bool true)))
        (stringReturn "false" (Literal (Bool false)))
    ]
    .>> ws) <?> "boolean"

let literal_string = 
    (between (pstring "\"") (pstring "\"") (manyChars (satisfy (fun c -> c <> '"')))
    |>> fun s -> Literal (String s)) <?> "string"

let literal_char = 
    (between (pstring "'") (pstring "'") (satisfy (fun c -> c <> '''))
    |>> fun c -> Literal (Char c)) <?> "character"
    
let identifier =
    (many1Satisfy2L isLetter (fun c -> isLetter c || isDigit c) "identifier"
    >>= fun s ->
        if reserved |> List.exists ((=) s) then fail ("'" ^ s ^ "' is not expected here")
        else preturn s)

let literal =
    (literal_numeric  <|>
     literal_bool     <|>
     literal_char     <|>
     literal_string)  <?> "literal"

// Values

let value, valueimpl = createParserForwardedToRef()

let dataname = identifier |>> DataName

let tuple = betweenParentheses (sepBy (ws >>. value) (str ",")) |>> Tuple

let list = betweenHooks (sepBy (ws >>. value) (str ",")) |>> List

let term =
    betweenParentheses value <|>
    value

let infixOperator (p: OperatorPrecedenceParser<_, _, _>) op prec map =
    p.AddOperator(InfixOperator(op, ws, prec, Associativity.Left, map))

let ops =
    // Arithmetic
    [ "+"; "-"; "*"; "/"; "%" ] @
    // Matrix and list
    [ "^"; "^+"; "^-"; "^*"; "^/"; "^%" ] @
    // Logical
    [ "&&"; "||"; "=="; "!="; ">"; "<"; ">="; "<=" ]

let opCorrespondance op =
    match op with
    // Arithmetic operators
    | "+"  -> Sum
    | "-"  -> Sub
    | "*"  -> Mul
    | "/"  -> Div
    | "%"  -> Mod
    // Matrix and list operators
    | "^"  -> Concat
    | "^+" -> ConcatAdd
    | "^-" -> ConcatSub
    | "^*" -> ConcatMul
    | "^/" -> ConcatDiv
    | "^%" -> ConcatMod
    // Logical operators
    | "&&" -> And
    | "||" -> Or
    | "==" -> Equal
    | "!=" -> NotEqual
    | ">"  -> Greater
    | "<"  -> Smaller
    | ">=" -> GreaterEqual
    | "<=" -> SmallerEqual
    | _    -> InfixOp.Custom op

let opParser = new OperatorPrecedenceParser<Expr, unit, UserState>()

for op in ops do
    infixOperator opParser op 1 (fun x y -> InfixOp(x, opCorrespondance op, y))

opParser.TermParser <- term .>> ws

// Arithmetic and logical expression
let operated =
    betweenParentheses opParser.ExpressionParser <|>
    opParser.ExpressionParser

valueimpl :=
    literal <|>
    tuple <|>
    list <|>
    dataname

// Types

let ptype, ptypeimpl = createParserForwardedToRef()

let typename = identifier |>> TypeName

let typetuple =
    betweenParentheses
        (sepBy (ws >>. ptype) (str ","))
        |>> TypeTuple

let typelist =
    betweenHooks
        (ws >>. ptype)
        |>> TypeList

let typearrow =
    (sepBy (ws >>. ptype) (str "->"))
    |>> TypeArrow

ptypeimpl :=
    typename <|>
    typetuple <|>
    typelist <|>
    typearrow // Error !!

// Generic type

let generictype = 
    ((between (str "<") (str ">") (sepBy (ws >>. ptype) (str ",")))
        |>> GenericType) <|>
    (ws >>% NoGenericType)

let type's assignSymbol =
    pipe2
        (ws >>. generictype)
        (ws >>. str assignSymbol >>. ws >>. ptype <|> (ws >>% InferedType))
        (fun gen ty -> Type's(gen, ty))
        
// Instructions and statements

let instruction, instructionRef = createParserForwardedToRef()
let indentedinstructions = indentedMany1 instruction "statement"

let print =
    keyword "print" >>.
    (ws1 >>. identifier .>> wsBeforeEOL |>> Print)

let loop =
    keyword "loop" >>.
    (pipe4
        (ws1 >>. identifier)
        (ws1 >>. pint32)
        (ws1 >>. pint32 .>> wsBeforeEOL)
        (indentedinstructions)
        (fun id min max stmts -> Loop(id, min, max, stmts))
    )

let expression =
    operated <|> value |>> Expression

// Instruction value
let instrval =
    ((indentedinstructions) <|>
    (instruction |>> fun instr -> [instr]))
    |>> Instructions

// For example: ` add :: Integer -> Integer -> Integer `
let pletinterface =
    keyword "let" >>.
    pipe2
        (ws1 >>. identifier)
        (ws >>. str "::" >>. ptype)
        (fun id ty -> LetInterface(id, ty))

let plet =
    keyword "let" >>.
    pipe3
        (ws1 >>. identifier)
        (type's ":")
        (ws >>. str "=" >>.
            ws >>. instrval .>> wsBeforeEOL)
        (fun id tys exp -> Let(id, tys, exp))

instructionRef :=
    print <|>
    loop <|>
    plet <|>
    pletinterface <|>
    expression

let program = indentedinstructions |>> Program

let test str =
    match runParserOnString program (UserState.Create()) "" str with
    | Success(result, _, _)   -> printfn "Success: %A" result
    | Failure(errorMsg, _, _) -> printfn "Failure: %s" errorMsg
