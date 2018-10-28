(* AST of the Arlia language *)

module AST

open System

type Optional<'a> =
    | Some of 'a
    | None

type Identifier =
    | Identifier of string
    | VariableName of string
    | LetName of string
    | FunctionName of string

    | TypeName of string
    | ImplicitType 

type Type = Identifier

type Literal =
    | Int of int
    | Float of float
    | Char of char
    | String of string
    | Bool of bool
    | Identifier of string

type Wildcard = obj

type Expr =
    | Literal of Literal
    | ListValue of Expr list
    | TupleValue of Expr list
    | ToExpr of Expr * Expr
    | Variable of Identifier
    | Invoke of Literal * Expr list Optional
    | LetInvoke of Identifier * Param list Optional
    | InfixOp of Expr * string * Expr
    | Dot of Expr * string * Expr
    | PrefixOp of string * Expr
    | PostfixOp of Expr * string
    | TernaryOp of TernaryCondition * TernaryTrue * TernaryFalse
    | TypeConstructor of Identifier * Expr list
    | Constructor of Identifier * Param list
    | Expression of Expr
    | MatchExpr of Case list
    | Value of Expr
    | Lambda of String list * Expr
    | Nothing
and TernaryCondition = Condition of Expr
and TernaryTrue = IfTrue of Expr
and TernaryFalse = IfFalse of Expr
and Case = 
    | Case of Expr * Expr
    | Wildcard of Wildcard
and Param = Param of Expr

type Define = Define of Identifier * Type Optional
type Init = Assign of Identifier * Expr
type DefaultValueArg = DefaultValueArg of Expr

type Arg = Arg of Define * DefaultValueArg Optional

type Iterator = Expr

type To = To of Expr
type Step = Step of Expr
type Each = Each of Expr
type In = In of Expr

type ArgConstructor = ArgFieldConstructor of Define * DefaultValueArg Optional

type Constructor = Constructor of Define * ArgConstructor list Optional

type Statement =
    | VarDeclr of Identifier * Type * Expr
    | LetDeclr of Identifier * Type * Expr
    | LetFuncDeclr of Identifier * Arg list * Type Optional * Expr
    | FuncDefinition of Identifier * Arg list Optional * Type Optional * Block
    | FuncInvoke of Literal * Expr list Optional
    | Assignment of Init
    | AnonymousExpression of Expr
    | If of Expr * Block Optional
    | IfElse of Expr * Block Optional * Block Optional
    | MatchStmt of Expr * Case
    | For of Init * To * Block Optional
    | ForStep of Init * To * Step * Block Optional
    | ForEach of Define * In * Block Optional
    | While of Expr * Block Optional
    | DoWhile of Block Optional * Expr
    | Throw of Expr
    | Catch of Define * Block Optional
    | Try of Block Optional
    | Continue
    | Return of Expr
    | TypeAsAlias of Type * Type
    | TypeAsStruct of Constructor
    | TypeAsClass of Constructor * Block
and Block = Block of Statement list

type TypeMemberAccess =
    | Public of Statement
    | Private of Statement

type ReturnType = Identifier Optional
type MemberInfo = MemberInfo of TypeMemberAccess * Identifier * ReturnType
type Member =
    | VarField of MemberInfo * Expr
    | LetField of MemberInfo * Param list Optional * Expr
    | Method of MemberInfo * Param list * Block

type Program = Program of Statement list
