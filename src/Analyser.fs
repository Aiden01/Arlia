module Analyser
open Parser
open AST

let analyse (program: Program) =
    match program with
    | AST.Program(stmts) ->
        for stmt in stmts do
            match stmt with
            | AST.Statement.LetDeclr(id, ty, ex) -> 0
            | AST.Statement.VarDeclr(id, ty, ex) -> 0
            | AST.Statement.AnonymousExpression(ex) -> 0
            | AST.Statement.Assignment(init) -> 0
            | AST.Statement.Catch(define, block) -> 0
            | AST.Statement.Continue -> 0
            | AST.Statement.While(ex, block) -> 0
            | AST.Statement.DoWhile(block, ex) -> 0
            | AST.Statement.For(init, to', block) -> 0
            | AST.Statement.ForEach(define, in', block) -> 0
            | AST.Statement.ForStep(init, to', step, block) -> 0
            | AST.Statement.FuncDefinition(id, args, ty, block) -> 0
            | AST.Statement.FuncInvocation(id, parameters) -> 0
            | AST.Statement.If(ex, block) -> 0
            | AST.Statement.IfElse(ex, block1, block2) -> 0
            | AST.Statement.Import(file) -> 0
            | AST.Statement.Include(file) -> 0
            | AST.Statement.LetFuncDeclr(id, args, ty, ex) -> 0
            | AST.Statement.MatchStmt(ex, cases) -> 0
            | AST.Statement.Module(id, block) -> 0
            | AST.Statement.Return(ex) -> 0
            | AST.Statement.Throw(ex) -> 0
            | AST.Statement.Try(block) -> 0
            | AST.Statement.TypeAsAlias(ty1, generictype, ty2) -> 0
            | AST.Statement.TypeAsClass(constructor, stmts) -> 0
            | AST.Statement.TypeAsStruct(constructor) -> 0
            | AST.Statement.TypeAsUnion(ty, generictype) -> 0
            | _ -> 0



(*

let var (name: string) (ty: Type) (value: Expr) =
    if System.Char.IsUpper(name.[0]) then printfn "Upper case!"
    else printfn "name: %s" name
    match ty with
    | Type.TypeName(type') -> printfn "type: %s" type'
    | _ -> printfn "type: ?"
    match value with
    | Expr.Literal(l) -> printfn "value: %A: " l
    0

let statement (a: Statement) =
    match a with
    | VarDeclr(name, ty, value) -> var

*)

