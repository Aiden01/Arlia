open Parser
open System.IO
open FParsec
open System

[<EntryPoint>]
let inline main argv =
    Console.Clear()
    let time = System.Diagnostics.Stopwatch.StartNew()
    let AST = (run pprog (File.ReadAllText argv.[0]))
    time.Stop()
    printfn "%A" AST
    File.WriteAllText("AST.txt", AST.ToString())
    printfn "Done.\nTime taken: %.2f seconds." time.Elapsed.TotalSeconds
    0
