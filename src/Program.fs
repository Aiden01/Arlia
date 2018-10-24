open Parser
open System.IO
open FParsec
open System

[<EntryPoint>]
let main argv =
    let sourceCode = File.ReadAllText argv.[0]
    Console.Clear()
    let time = System.Diagnostics.Stopwatch.StartNew()
    let AST = (run pstatement sourceCode)
    time.Stop()
    printfn "%A" AST
    File.WriteAllText("AST.txt", AST.ToString())
    printfn "\nTime taken: %.2f seconds." time.Elapsed.TotalSeconds
    0
