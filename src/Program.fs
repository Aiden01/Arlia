open System.IO
open System
open Analyser

[<EntryPoint>]
let inline main argv =
    Console.Clear()
    let time = System.Diagnostics.Stopwatch.StartNew()
    let program = Parser.parse (File.ReadAllText argv.[0])
    //printfn "%A" program

    //let AST: string = "[" ^ program.ToString()
    //                                .Replace("\n", "")
    //                                .Replace(" ", "")
    //                                .Replace("Int", "Int ")
    //                                .Replace("Float", "Float ")
    //                                .Replace("Char", "Char ")
    //                                .Replace("String", "String ")
    //                                .Replace("Identifier", "Identifier ")
    //                                .Replace("TypeName", "TypeName ")
    //                                .Replace("(", "[")
    //                                .Replace(")", "]")
    //                                .Replace(",", "][")
    //                                .Replace(";", "][") ^ "]"

    //printfn "%s" AST
    
    analyse program

    time.Stop()
    printfn "\nTime taken: %.2f seconds." time.Elapsed.TotalSeconds
    0
