module AST

type Identifier = string

type Type =
    | Integer
    | Boolean
    | Float
    | Char
    | String
    | Identifier of string
    | Function of Type * Type
    | TypeName of string
    | TupleType of Type list
    | ArrowType of Type list * Type
    | GenericType of Type list
    | ImplicitType
    | EnumType

type GenericType =
    | GenericType of Type list
    | NoGenericType

type Literal =
    | Int of int
    | Float of float
    | Char of char
    | String of string
    | Bool of bool
    | Identifier of string

type Expr =
    | Literal of Literal
    | Identifier of string
    | ListValue of Expr list
    | TupleValue of Expr list
    | ToExpr of Expr * Expr
    | ToStepExpr of Expr * Expr * Expr
    | Variable of Identifier
    | Invoke of Literal * Param list
    | InfixOp of Expr * string * Expr
    | Dot of Expr * string * Expr
    | PrefixOp of string * Expr
    | PostfixOp of Expr * string
    | TernaryOp of TernaryCondition * TernaryTrue * TernaryFalse
    | TypeConstructor of Type * GenericType * Expr list
    | Constructor of Identifier * Param list
    | Expression of Expr
    | Match of Expr * Case list
    | Value of Expr
    | Lambda of string list * Expr
    | Extern of string * string * Param list
and TernaryCondition = Condition of Expr
and TernaryTrue = IfTrue of Expr
and TernaryFalse = IfFalse of Expr
and Case =
    | Pattern of Identifier * Identifier list * Expr
    | Case of Expr * Expr
    | Wildcard of Expr
and Param =
    | Param of Expr
    | Ref of Identifier

type Define = Define of Identifier * Type
type Init = Identifier * Expr

type DefaultValueArg =
    | DefaultValueArg of Expr
    | NoDefaultValueArg

type Arg = Define * DefaultValueArg

type Arguments = Arg list

type Parameters = Param list

type To = To of Expr
type Step = Step of Expr
type In = In of Expr

type TypeMemberAccess =
    | Public
    | Private

type ArgConstructor = ArgFieldConstructor of TypeMemberAccess * Define * DefaultValueArg

type Constructor = Identifier * GenericType * ArgConstructor list

type LetFuncId =
    | Identifier' of Identifier
    | CustomOp of string // Always infix

type Statement =
    | VarDeclr of Identifier * Type * GenericType * Expr
    | LetDeclr of Identifier * Type * GenericType * Expr // * codeLine: string
    | LetFuncDeclr of Identifier * Arguments * Type * Expr
    | FuncDefinition of LetFuncId * Arguments * Type * Block
    | FuncInvocation of Identifier * Parameters
    | Storage of Init
    | AnonymousExpression of Expr
    | If of Expr * Block
    | IfElse of Expr * Block * Block
    | MatchStmt of Expr * Case list
    | For of Init * To * Block
    | ForStep of Init * To * Step * Block
    | ForEach of Define * In * Block
    | While of Expr * Block
    | DoWhile of Block * Expr
    | Throw of Expr
    | Try of Expr * Case list
    | Continue
    | Return of Expr
    | TypeAsAlias of Type * GenericType * Type
    | TypeAsStruct of Constructor
    | TypeAsClass of Constructor * Member list
    | TypeAsSum of Type * GenericType * SumConstructors
    | Include of string
    | Import of string
    | Module of ModuleName * Block
    | Extern of string * string * Param list
and Block = Statement list
and Member = TypeMemberAccess * Statement
and SumConstructor =
    | SumConstructor of Identifier * Type list
    | SumType of Identifier
and SumConstructors = SumConstructors of SumConstructor list
and ModuleName = string

type Program = Program of Statement list
