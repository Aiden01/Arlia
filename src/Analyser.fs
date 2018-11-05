module Analyser
open Parser
open AST

let invalidIdentifier'var_iscapitalized (id: string) =
    (""" A variable / constant cannot be capitalized, and must be started with a lowercase letter : 
     """ ^ "'" ^ id ^ "' -> '" ^ id.[0].ToString().ToLower() ^ id.Substring 1 ^ "'")

let letDeclr_analyse (id: Identifier) (ty: Type) (ex: Expr) = 0    
let varDeclr_analyse (id: Identifier) (ty: Type) (ex: Expr) = 0
let anonymousExpression_analyse (ex: Expr) = 0
let assignment_analyse (init: Init) = 0
let catch_analyse (define: Define) (block: Block) = 0
let continue_analyse () = 0
let while_analyse (ex: Expr) (block: Block) = 0
let doWhile_analyse (block: Block) (ex: Expr) = 0
let for_analyse (init: Init) (to': To) (block: Block) = 0
let forEach_analyse (define: Define) (in': In) (block: Block) = 0
let forStep_analyse (init: Init) (to': To) (step: Step) (block: Block) = 0
let funcDefinition_analyse (id: Identifier) (args: Arguments) (ty: Type) (block: Block) = 0
let funcInvocation_analyse (id: Identifier) (parameters: Parameters) = 0
let if_analyse (ex: Expr) (block: Block) = 0
let ifElse_analyse (ex: Expr) (block1: Block) (block2: Block) = 0
let import_analyse (file: string) = 0
let include_analyse (file: string) = 0
let letFuncDeclr_analyse (id: Identifier) (args: Arguments) (ty: Type) (ex: Expr) = 0
let matchStmt_analyse (ex: Expr) (cases: Case) = 0
let module_analyse (id: Expr) (block: Block) = 0
let return_analyse (ex: Expr) = 0
let throw_analyse (ex: Expr) = 0
let try_analyse (block: Block) = 0
let typeAsAlias_analyse (ty1: Type) (genericType: GenericType) (ty2: Type) = 0
let typeAsClass_analyse (constructor: Constructor) (stmts: Block) = 0
let typeAsStruct_analyse (constructor: Constructor) = 0
let typeAsUnion_analyse (ty: Type) (genericType: GenericType) = 0

let analyse (program: Program) =
    match program with
    | AST.Program(stmts) -> 
        for stmt in stmts do
            match stmt with
            | AST.Statement.LetDeclr(id, ty, ex) -> letDeclr_analyse id ty ex
            | AST.Statement.VarDeclr(id, ty, ex) -> varDeclr_analyse id ty ex
            | AST.Statement.AnonymousExpression(ex) -> anonymousExpression_analyse ex
            | AST.Statement.Assignment(init) -> assignment_analyse init
            | AST.Statement.Catch(define, block) -> catch_analyse define block
            | AST.Statement.Continue -> continue_analyse ()
            | AST.Statement.While(ex, block) -> while_analyse ex block
            | AST.Statement.DoWhile(block, ex) -> doWhile_analyse block ex
            | AST.Statement.For(init, to', block) -> for_analyse init to' block
            | AST.Statement.ForEach(define, in', block) -> forEach_analyse define in' block
            | AST.Statement.ForStep(init, to', step, block) -> forStep_analyse init to' step block
            | AST.Statement.FuncDefinition(id, args, ty, block) -> funcDefinition_analyse id args ty block
            | AST.Statement.FuncInvocation(id, parameters) -> funcInvocation_analyse id parameters
            | AST.Statement.If(ex, block) -> if_analyse ex block
            | AST.Statement.IfElse(ex, block1, block2) -> ifElse_analyse ex block1 block2
            | AST.Statement.Import(file) -> import_analyse file
            | AST.Statement.Include(file) -> include_analyse file
            | AST.Statement.LetFuncDeclr(id, args, ty, ex) -> letFuncDeclr_analyse id args ty ex
            | AST.Statement.MatchStmt(ex, cases) -> matchStmt_analyse ex cases
            | AST.Statement.Module(id, block) -> module_analyse id block
            | AST.Statement.Return(ex) -> return_analyse ex
            | AST.Statement.Throw(ex) -> throw_analyse ex
            | AST.Statement.Try(block) -> try_analyse block
            | AST.Statement.TypeAsAlias(ty1, generictype, ty2) -> typeAsAlias_analyse ty1 generictype ty2
            | AST.Statement.TypeAsClass(constructor, stmts) -> typeAsClass_analyse constructor stmts
            | AST.Statement.TypeAsStruct(constructor) -> typeAsStruct_analyse constructor
            | AST.Statement.TypeAsUnion(ty, generictype) -> typeAsUnion_analyse ty generictype

