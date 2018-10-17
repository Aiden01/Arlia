open Parser
open System.IO
open FParsec
open System

//File.ReadAllText argv.[0]

[<EntryPoint>]
let main argv =
    let sourceCode = File.ReadAllText argv.[0]
    Console.Clear()
    let r = printfn "%A" (run pstatement sourceCode)
    0
