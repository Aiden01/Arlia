module Analyser

open AST
open TypeChecker

module Analyse =
    
    /// Check the type of t1 with t2, display an error message if they don't match and
    /// return true if they match and false if not
    let sameLtypeRtype t1 t2 e1 e2 =
        if t1 <> t2 then
            match e1, e2 with
            | AST.Expr.Literal l1, l2 -> printfn "Error: The type `%A`does not match the type `%A`" l1 l2
            | _ -> printfn "Error: The type `%A`does not match the type `%A`" e1 e2
            false
        else true

    let rec expression (e : AST.Expr) =
    match e with
    | AST.Expr.Value v -> test v
    | AST.Expr.Expression v -> test v
    | AST.Expr.Literal _ -> test e
    | AST.Expr.Identifier i -> printfn "To do"
    | AST.Expr.InfixOp(e1, sign, e2) ->

        let t1 = typeInference Map.empty e1
        let t2 = typeInference Map.empty e2
        
        sameLtypeRtype t1 t2 e1 e2 |> ignore

        match sign with
        | "+" 
        | "-"
        | "*"
        | "/"
        | "%"
        | "**"
        | "^+"
        | "^-"
        | "^*"
        | "^/"
        | "^%"
        | "&&"
        | "||"
        | "=="
        | "^"
        | "!="
        | "<="
        | ">="
        | "<"
        | ">" -> printfn ""
    | _ -> printfn "%A" e

let analyse (ast : AST.Program) =
    match ast with
    | AST.Program(stmts) ->
        for stmt in stmts do
            match stmt with
            | AST.Statement.LetDeclr(i, t, g, e) ->
                match e with
                | Value v -> test v
                | _ -> Analyse.expression e
            | AST.Statement.AnonymousExpression e -> Analyse.expression e
            | _ -> printfn "Impossible to type-check on this: `%A`" stmt
