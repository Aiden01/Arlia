module Analyser
open Parser
open AST
open Errors

// This variable will change to false if there are errors
let mutable canCompile = true

(* =+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+==+=+=+= *)

type Scope = { mutable scopeName: string; mutable identifiers: string list }

let analyseCommonIdentifier (scope: Scope ref) (id: Identifier) =
    if System.Char.IsUpper id.[0] then
        Errors.showErr (Errors.invalidIdentifier'var_iscapitalized id)
        canCompile <- false
    if scope.contents.identifiers |> List.exists ((=) id) then
        Errors.showErr (Errors.invalidIdentifier'alreadyExist id)
        canCompile <- false
    else scope.contents.identifiers <- [id] |> List.append scope.contents.identifiers

let analyseGenericType (gen: GenericType) =
    match gen with
    | GenericType(types) -> printf ""
    | NoGenericType -> printf ""

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
    match init with Assign(id, ex) -> printf ""

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

let analyseType (ty: Type) =
    match ty with
    | TypeName(typename) -> printf ""
    | TupleType(tuple) -> printf ""
    | TypeFuncDef(typename) -> printf ""
    | Type.GenericType(generic) -> printf ""
    | ImplicitType -> printf ""

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
    | TypeConstructor(ty, params) -> printf ""
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
    | LetDeclr(id, ty, ex) ->
        analyseCommonIdentifier scope id
        analyseType ty
        analyseExpression ex
    | VarDeclr(id, ty, ex) ->
        analyseCommonIdentifier scope id
        analyseType ty
        analyseExpression ex
    | AnonymousExpression(ex) -> 
        analyseExpression ex
    | Assignment(init) ->
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
        analyseCommonIdentifier scope id
        // analyseArgs args
        analyseType ty
        analyseBlock (analyseStatement scope) block
    | FuncInvocation(id, parameters) ->
        analyseCommonIdentifier scope id
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
        analyseCommonIdentifier scope id
        // analyseArgs args
        analyseType ty
        analyseExpression ex
    | MatchStmt(ex, cases) ->
        analyseExpression ex
        analyseCases cases
    | Module(id, block) ->
        analyseCommonIdentifier scope (id.ToString())
        analyseBlock (analyseStatement scope) block
    | Return(ex) ->
        analyseExpression ex
    | Throw(ex) ->
        analyseExpression ex
    | Try(block) ->
        analyseBlock (analyseStatement scope) block
    | TypeAsAlias(ty1, generictype, ty2) ->
        analyseType ty1
        analyseGenericType generictype
        analyseType ty2
    | TypeAsClass(constructor, stmts) -> printf ""
    | TypeAsStruct(constructor) -> printf ""
    | TypeAsUnion(ty, generictype) ->
        analyseType ty
        analyseGenericType generictype
    
let analyse (program: Program) =
    let scope = ref { scopeName = "Program"; identifiers = [] }
    match program with
    | AST.Program(stmts) ->
        for stmt in stmts do
            analyseStatement scope stmt
