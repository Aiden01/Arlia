module Errors

let showErr msg dsperrm =
    System.Console.ForegroundColor <- System.ConsoleColor.Red
    System.Console.WriteLine (if dsperrm then "Error: " ^ msg else msg)
    System.Console.ResetColor()

let invalidIdentifier'iscapitalized (id: string) (keywordty: string) =
    (""" The name of a '""" ^ keywordty ^ """' must be different from that of a type,
            and must necessarily begin with a lowercase letter : 
            """ ^ "'" ^ id ^ "' -> '" ^ id.[0].ToString().ToLower() ^ id.Substring 1 ^ "'")

let invalidTypeIdentifier'islowered (id: string) =
    (""" The name of a type must necessarily begin with an uppercase letter : 
            """ ^ "'" ^ id ^ "' -> '" ^ id.[0].ToString().ToUpper() ^ id.Substring 1 ^ "'")

let invalidIdentifier'alreadyExist (id: string) =
    (""" Already defined identifier in this scope: '""" ^ id ^ """'""")

let invalidTypeIdentifier'alreadyExist (id: string) =
    (""" Already defined type in this scope: '""" ^ id ^ """'""")
