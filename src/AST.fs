module AST

type Identifier = string

type Type =
    | TypeName of Identifier        // Such as `Integer`
    | TypeTuple of Type list        // Such as `(Integer, Float)`
    | TypeList of Type              // Such as `[Integer]`
    | TypeArrow of Type list        // Such as `Integer -> Float`
    | InferedType                   // When the type is to deduce

type GenericType =
    | GenericType of Type list      // Such as `<T1, T2>`
    | NoGenericType                 // When there are no generic types, `<>` is an error

type Type's = GenericType * Type    // Regrouping generic type, and type of data

type Literal =
    | Int of int
    | Float of float
    | Bool of bool
    | Char of char
    | String of string

type Instruction =
    | Loop of Identifier * int * int * Instructions // For test
    | Print of Identifier // For test
    | Let of Identifier * Type's * Value
    | LetInterface of Identifier * Type
    | If of Expr * Instructions
    | IfElse of Expr * Instructions * Instructions
    | IfElif of Expr * Instructions * (Expr * Instructions)
    | IfElifElse of Expr * Instructions * (Expr * Instructions) * Instructions
    | Expression of Value

and Instructions = Instruction list

and Value =
    | Literal of Literal
    | List of Value list
    | Tuple of Value list
    | InfixOp of Value * InfixOp * Value
    | PostfixOp of Value * PostfixOp
    | PrefixOp of PrefixOp * Value
    | Instructions of Instruction list
    | DataName of Identifier
    | Nil // When there are no value, such as in `Arguments`
and Expr = Value

and InfixOp =
    | Sum | Sub | Mul | Div | Pow | Mod
    | Concat | ConcatAdd | ConcatSub | ConcatMul | ConcatDiv | ConcatMod
    | And | Or | Equal | NotEqual | Greater | Smaller | GreaterEqual | SmallerEqual
    | Custom of string

and PostfixOp = Custom of string

and PrefixOp = Custom of string

and Arguments = (Identifier * Value) list

type Program = Program of Instruction list
