module Errors

let showErr msg =
    System.Console.ForegroundColor <- System.ConsoleColor.Red
    printfn "Error: %s" msg
    System.Console.ForegroundColor <- System.ConsoleColor.Gray



let invalidIdentifier'var_iscapitalized (id: string) =
    (""" A variable / constant cannot be capitalized, and must start with a lowercase letter : 
        """ ^ "'" ^ id ^ "' -> '" ^ id.[0].ToString().ToLower() ^ id.Substring 1 ^ "'")

let invalidIdentifier'alreadyExist (id: string) =
    (""" Already used identifier: '""" ^ id ^ """'""")

