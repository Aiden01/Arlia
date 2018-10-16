(* AST of the Arlia language *)

module AST

open System

type Identifier = string
type VarName = Identifier
type LetName = Identifier
type FuncName = Identifier
type TypeName = Identifier

type IsConst = bool

type Value = obj
type Literal = Literal of Value
type TupleValue = Value list

type Wildcard = obj

type ArgType =
    | ValueArg
    | RefArg
    | FuncArg

type ParamType =
    | ByValue
    | ByRef
    | ByFunc

type Expr =
    | Value of Literal
    | Variable of VarName
    | FunctionInvoke of FuncName * Arg list
    | LetInvoke of LetName * Arg list option
    | InfixOp of Expr * string * Expr
    | PrefixOp of string * Expr
    | PostfixOp of Expr * string
    | TernaryOp of Expr * Expr * Expr
and Arg = Arg of ArgType

type Define = Define of VarName * TypeName option
type Init = Assign of Identifier * Expr
type Condition = Expr
type Iterator = Expr

type Statement =
    | Definition of Define
    | Assignment of Init
    | Action of Expr
    | If of Expr * Block option
    | Elif of Expr * Block option
    | Else of Expr * Block option
    | Match of Expr * Case list
    | For of Define * Init option * Expr * Expr option  // For / For each / For step
    | While of Expr * Block option
    | DoWhile of Block option * Expr
    | Throw of Expr
    | Catch of Define * Block option
    | Try of Block option
    | Continue
    | Return of Expr
and Case = 
    | Case of Literal
    | Wildcard of Wildcard
and Block = Statement list

type TypeMemberAccess = 
    | Public
    | Private

type ReturnType = TypeName option
type MemberInfo = MemberInfo of TypeMemberAccess * ReturnType * Identifier
type Param = Param of ParamType * VarName * TypeName option
type Member =
    | VarField of MemberInfo * Expr
    | LetField of MemberInfo * Param list option * Expr
    | Method of MemberInfo * Param list * Block
    
type TypeDefinition = 
    | TypeAsStruct of TypeName * ConstructorValues * ConstructorValues
    | TypeAsEnum of TypeName * EnumValue list
    | TypeAsAlias of TypeName * TypeName 
    | TypeAsTuple of TypeName * TypeName list
and EnumValue = EnumValue of Identifier * Value option
and ConstructorValues = TypeMemberAccess option * Param list option * Member list option

type Import = Import of Identifier list
