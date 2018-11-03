﻿open System.IO
open System
open Analyser

[<EntryPoint>]
let inline main argv =
    Console.Clear()
    let time = System.Diagnostics.Stopwatch.StartNew()
    let program = Parser.parse (File.ReadAllText argv.[0])
    //printfn "%A" program
    analyse program |> printfn "%A"
    time.Stop()
    printfn "Done.\nTime taken: %.2f seconds." time.Elapsed.TotalSeconds
    0
