open System.IO
open System
open Analyser

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

let welcome = 
 """     ___       ______    __       __       ___      
    /   \     |   _  \  |  |     |  |     /   \         
   /  ^  \    |  |_)  | |  |     |  |    /  ^  \        Version 0.01
  /  /_\  \   |      /  |  |     |  |   /  /_\  \       By vanaur
 /  _____  \  |  |\  \  |  `---. |  |  /  _____  \      Type :? for help
/__/     \__\ | _| \__\ |______| |__| /__/     \__\     
____________________________________________________    
 """


let inline runOnFile filename = 
    if System.IO.File.Exists filename then
        let time = System.Diagnostics.Stopwatch.StartNew()
        let program = Parser.parse (File.ReadAllText filename) filename
        if analyse program = false then printfn "\nCan't compile. Sorry."
        time.Stop()
        printfn "\n\nTime taken: %.2f seconds." time.Elapsed.TotalSeconds
    else
        printfn "%s" ("Can't load file: '" ^ filename ^ "'")
    0

type ActionOnInput =
    | Quit

let inline analyseInput input =
    match input with
    | ":q" -> Quit.ToString()
    | _ -> input

let inline runOnCLI a =
    while true do
        printf "Arlia> "
        let input = Console.ReadLine()
        if analyseInput input = "Quit" then
            printfn "Bye bye"
            System.Environment.Exit(0)
        analyse ((Parser.parse (input ^ ";\n")) "input")
    0

[<EntryPoint>]
let inline main argv =
    Console.Clear()
    printfn "%s" welcome
    if argv.Length = 1 then runOnFile argv.[0]
    else runOnCLI 0
    0
