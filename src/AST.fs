module AST

type Identifier = string

type Type =
    | TypeName of string
    | TupleType of Type list
    | TypeFuncDef of Type list
    | GenericType of Type list
    | ImplicitType
    
type Literal =
    | Int of int
    | Float of float
    | Char of char
    | String of string
    | Bool of bool
    | Identifier of string

type Expr =
    | Literal of Literal
    | ListValue of Expr list
    | TupleValue of Expr list
    | ToExpr of Expr * Expr
    | Variable of Identifier
    | Invoke of Literal * Expr list
    | InfixOp of Expr * string * Expr
    | Dot of Expr * string * Expr
    | PrefixOp of string * Expr
    | PostfixOp of Expr * string
    | TernaryOp of TernaryCondition * TernaryTrue * TernaryFalse
    | TypeConstructor of Type * Expr list
    | Constructor of Identifier * Param list
    | Expression of Expr
    | Match of Expr * Case list * Case
    | Value of Expr
    | Lambda of string list * Expr
    | Extern of string * string * Expr list
and TernaryCondition = Condition of Expr
and TernaryTrue = IfTrue of Expr
and TernaryFalse = IfFalse of Expr
and Case = 
    | Case of Expr * Expr
    | Wildcard of Expr
and Param = Param of Expr

type Define = Define of Identifier * Type
type Init = Assign of Identifier * Expr

type DefaultValueArg =
    | DefaultValueArg of Expr
    | NoDefaultValueArg

type Arg = Define * DefaultValueArg

type Arguments = Arg list

type Parameters = Expr list

type To = To of Expr
type Step = Step of Expr
type Each = Each of Expr
type In = In of Expr

type TypeMemberAccess =
    | Public
    | Private

type ArgConstructor = ArgFieldConstructor of TypeMemberAccess * Define * DefaultValueArg

type GenericType =
    | GenericType of Type list
    | NoGenericType

type Constructor = Identifier * GenericType * ArgConstructor list

type Statement =
    | VarDeclr of Identifier * Type * Expr
    | LetDeclr of Identifier * Type * Expr
    | LetFuncDeclr of Identifier * Arguments * Type * Expr
    | FuncDefinition of Identifier * Arguments * Type * Block
    | FuncInvocation of Identifier * Parameters
    | Assignment of Init
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
    | Catch of Define * Block
    | Try of Block
    | Continue
    | Return of Expr
    | TypeAsAlias of Type * GenericType * Type
    | TypeAsStruct of Constructor
    | TypeAsClass of Constructor * Statement list //* Member list
    | TypeAsUnion of Type * GenericType 
    | Include of string
    | Import of string
    | Module of Expr * Block
and Block = Statement list
and Member = TypeMemberAccess * Statement

type Program = Program of Statement list
