module Analyser
open Parser
open AST
open Errors
open System.Linq.Expressions

// This variable will change to false if there are errors
let mutable canCompile = true

(* =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+= *)

//type Variables = (Identifier * Type * Expr) list
//type Lets = (Identifier * Type * Expr) list
//type LetFuncs = (Identifier * Arguments * Type * Expr) list
//type Functions = (Identifier * Arguments * Type * Block) list
//type Types = Type list

//type Scope = {
//    variables: Variables
//    lets: Lets
//    letFuncs: LetFuncs
//    functions: Functions
//    types: Types
//}

type Scope = {
    mutable scopeName: string
    mutable identifiers: string list
    mutable types: string list
}



type TypeOfCommonIdentifier = Variable' | Let' | LetFunc' | Function' | Module'
let toci's (toci: TypeOfCommonIdentifier): string =
    match toci with
    | Variable' -> "variable"
    | Let' -> "let constant"
    | LetFunc' -> "let function"
    | Function' -> "function"
    | Module' -> "module"
    | _ -> "<unknown>"

type TypeOfTypeUse = Define' | Assign'
let totu's (totu: TypeOfTypeUse): string =
    match totu with
    | Define' -> "type definition"
    | Assign' -> "type assignation"
    | _ -> "<unknown>"

let analyseCommonIdentifier (scope: Scope ref) (id: Identifier) (toci: TypeOfCommonIdentifier) =
    if System.Char.IsUpper id.[0] then
        Errors.showErr (Errors.invalidIdentifier'iscapitalized id (toci's toci))
        canCompile <- false
    if scope.contents.identifiers |> List.exists ((=) id) then
        Errors.showErr (Errors.invalidIdentifier'alreadyExist id)
        canCompile <- false
    else scope.contents.identifiers <- [id] |> List.append scope.contents.identifiers

let analyseTypeName (scope: Scope ref) (ty: string) (totu: TypeOfTypeUse) =
    if System.Char.IsLower ty.[0] then
        Errors.showErr (Errors.invalidTypeIdentifier'islowered ty)
        canCompile <- false
    if (totu = Define') then if scope.contents.types |> List.exists ((=) ty) then
        Errors.showErr (Errors.invalidTypeIdentifier'alreadyExist ty)
        canCompile <- false
                             else scope.contents.types <- [ty] |> List.append scope.contents.types

let analyseArgConstructor (argc: ArgConstructor) =
    match argc with ArgFieldConstructor(access, define, dva) -> printf ""

let analyseTo (to': To) =
    match to' with To(ex) -> printf ""

let analyseStep (step: Step) =
    match step with Step(ex) -> printf ""

let analyseEach (each: Each) =
    match each with Each(ex) -> printf ""

let analyseIn (in': In) =
    match in' with In(ex) -> printf ""

let analyseDefaultValueArg (dva: DefaultValueArg) =
    match dva with
    | DefaultValueArg(ex) -> printf ""
    | NoDefaultValueArg -> printf ""

let analyseInit (init: Init) =
    match init with (id, ex) -> printf ""

let analyseDefine (define: Define) =
    match define with Define(id, ty) -> printf ""

let analyseCase (case: Case) =
    match case with
    | Case(value, ex) -> printf ""
    | Wildcard(ex) -> printf ""

let analyseCases (cases: Case list) =
    for case in cases do
        analyseCase case

let analyseLiteral (lit: AST.Literal) =
    match lit with
    | Bool(a) -> printf ""
    | Int(a) -> printf ""
    | Float(a) -> printf ""
    | Char(a) -> printf ""
    | String(a) -> printf ""
    | Identifier(a) -> printf ""

let analyseType (scope: Scope ref) (ty: Type) (totu: TypeOfTypeUse) =
    match ty with
    | TypeName(typename) -> analyseTypeName scope typename totu
    | TupleType(tuple) -> printf ""
    | TypeFuncDef(typenames, typename) -> printf ""
    | Type.GenericType(generic) -> printf ""
    | ImplicitType -> printf ""

let analyseGenericType (scope: Scope ref) (gen: GenericType) =
    match gen with
    | GenericType(types) -> 
        for ty in types do
            analyseType scope ty Define'
    | NoGenericType -> printf ""

let rec analyseExpression (expr: Expr) =
    match expr with
    | Literal(lit) -> printf ""
    | ListValue(list) -> printf ""
    | TupleValue(tuple) -> printf ""
    | ToExpr(lex, rex) -> printf ""
    | Variable(id) -> printf ""
    | Invoke(id, params) -> printf ""
    | InfixOp(lex, op, rex) -> printf ""
    | Dot(lobj, op, robj) -> printf ""
    | PrefixOp(op, ex) -> printf ""
    | PostfixOp(ex, op) -> printf ""
    | TernaryOp(condition, iftrue, iffalse) -> printf ""
    | TypeConstructor(ty, genty, params) -> printf ""
    | Constructor(id, params) -> printf ""
    | Expression(ex) -> analyseExpression ex
    | Match(ex, cases, wildcard) -> printf ""
    | Value(ex) -> analyseExpression ex
    | Lambda(args, value) -> printf ""
    | Extern(dll, func, params) -> printf ""

let analyseBlock (analyseStatement) (block: Block) =
    for stmt in block do
        analyseStatement stmt

let rec analyseStatement (scope: Scope ref) (statement: Statement) =
    match statement with
    | LetDeclr(id, ty, genty, ex) ->
        analyseCommonIdentifier scope id Let'
        analyseType scope ty Assign'
        analyseExpression ex
    | VarDeclr(id, ty, genty, ex) ->
        analyseCommonIdentifier scope id Variable'
        analyseType scope ty Assign'
        analyseExpression ex
    | AnonymousExpression(ex) -> 
        analyseExpression ex
    | Storage(init) ->
        analyseInit init
    | Catch(define, block) -> 
        analyseDefine define
        analyseBlock (analyseStatement scope) block
    | Continue -> printf ""
    | While(ex, block) ->
        analyseExpression ex
        analyseBlock (analyseStatement scope) block
    | DoWhile(block, ex) ->
        analyseBlock (analyseStatement scope) block
        analyseExpression ex
    | For(init, to', block) ->
        analyseInit init
        analyseTo to'
        analyseBlock (analyseStatement scope) block
    | ForEach(define, in', block) ->
        analyseDefine define
        analyseIn in'
        analyseBlock (analyseStatement scope) block
    | ForStep(init, to', step, block) ->
        analyseInit init
        analyseTo to'
        analyseBlock (analyseStatement scope) block
    | FuncDefinition(id, args, ty, block) ->
        analyseCommonIdentifier scope id Function'
        // analyseArgs args
        analyseType scope ty Assign'
        analyseBlock (analyseStatement scope) block
    | FuncInvocation(id, parameters) ->
        analyseCommonIdentifier scope id Function'
        // analyseParameters parameters
    | If(ex, block) ->
        analyseExpression ex
        analyseBlock (analyseStatement scope) block
    | IfElse(ex, block1, block2) ->
        analyseExpression ex
        analyseBlock (analyseStatement scope) block1
        analyseBlock (analyseStatement scope) block2
    | Import(file) -> printf ""
    | Include(file) -> printf ""
    | LetFuncDeclr(id, args, ty, ex) ->
        analyseCommonIdentifier scope id LetFunc'
        // analyseArgs args
        analyseType scope ty Assign'
        analyseExpression ex
    | MatchStmt(ex, cases) ->
        analyseExpression ex
        analyseCases cases
    | Module(id, block) ->
        analyseCommonIdentifier scope (id.ToString()) Module'
        analyseBlock (analyseStatement scope) block
    | Return(ex) ->
        analyseExpression ex
    | Throw(ex) ->
        analyseExpression ex
    | Try(block) ->
        analyseBlock (analyseStatement scope) block
    | TypeAsAlias(ty1, generictype, ty2) ->
        analyseType scope ty1 Define'
        analyseGenericType scope generictype
        analyseType scope ty2 Assign'
    | TypeAsClass(constructor, stmts) -> printf ""
    | TypeAsStruct(constructor) -> printf ""
    | TypeAsUnion(ty, generictype) ->
        analyseType scope ty Define'
        analyseGenericType scope generictype
    
let analyse (program: Program) =
    let scope = ref { scopeName = "Program"; identifiers = []; types = [] }
    match program with
    | AST.Program(stmts) ->
        if stmts.IsEmpty then canCompile <- false
        else for stmt in stmts do analyseStatement scope stmt
    canCompile
