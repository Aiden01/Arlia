(* AST of the Arlia language *)

module AST

open System

type Identifier = string
type VarName    = Identifier
type LetName    = Identifier
type FuncName   = Identifier
type TypeName   = Identifier

type InferredType =
    | Implicit
    | Explicit of TypeName

type Literal =
    | Int of int
    | Float of float
    | Char of char
    | String of string
    | Bool of bool
    | Identifier of Identifier

type Wildcard = obj

type Expr =
    | Literal of Literal
    | ListValue of Expr list
    | TupleValue of Expr list
    | To of Expr * Expr
    | Variable of VarName
    | Invoke of Literal * Expr list option
    | LetInvoke of LetName * Param list option
    | InfixOp of Expr * string * Expr
    | PrefixOp of string * Expr
    | PostfixOp of Expr * string
    | TernaryOp of Expr * Expr * Expr
    | TypeConstructor of TypeName * Expr list
    | Constructor of TypeName * Param list
and Param = Param of Expr

type Define = Define of Identifier * InferredType option
type Init = Assign of Identifier * Expr

type Arg = Arg of Define * Expr option

type Condition = Expr
type Iterator = Expr

type To = To of Expr
type Step = Step of Expr
type Each = Each of Expr
type In = In of Expr

type Statement =
    | VarDeclr of LetName * InferredType * Expr
    | LetDeclr of LetName * InferredType * Expr
    | FuncDefinition of FuncName * Arg list option * InferredType option * Block
    | FuncInvoke of Literal * Expr list option
    | Assignment of Init
    | Action of Expr
    | If of Expr * Block option
    | IfElse of Expr * Block option * Block option
    | Match of Expr * Case list
    | For of Init * To * Block option
    | ForStep of Init * To * Step * Block option
    | ForEach of Define * In * Block option
    | While of Expr * Block option
    | DoWhile of Block option * Expr
    | Throw of Expr
    | Catch of Define * Block option
    | Try of Block option
    | Continue
    | Return of Expr

    | TypeAsStruct of TypeName * Arg list
and Case = 
    | Case of Literal
    | Wildcard of Wildcard
and Block = Statement list

type TypeMemberAccess = 
    | Public
    | Private

type ReturnType = TypeName option
type MemberInfo = MemberInfo of TypeMemberAccess * Identifier * ReturnType
type Member =
    | VarField of MemberInfo * Expr
    | LetField of MemberInfo * Param list option * Expr
    | Method of MemberInfo * Param list * Block
    
//type TypeDefinition = 
//    | TypeAsStruct of TypeName * ConstructorValues
//    | TypeAsClass of TypeName * Arg list option * Block
//    | TypeAsEnum of TypeName * EnumValue list
//    | TypeAsAlias of TypeName * TypeName 
//    | TypeAsTuple of TypeName * TypeName list
//and EnumValue = EnumValue of Identifier * Value option
//and ConstructorValues = Arg list option * Member list option

type Program = Program of Statement list
