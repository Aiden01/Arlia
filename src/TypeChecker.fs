module TypeChecker
open AST

type SubstType =
    | Integer
    | Boolean
    | Float
    | Char
    | String
    | Name of string
    | Function of SubstType * SubstType

type TypeSubst = Map<string, SubstType>

type TypeScheme =
    { Values : string list
      Type : SubstType }

type TypeEnv =
    { Schemes : Map<string, TypeScheme> }

type SubstType with

    member this.FreeTypeValues =
        match this with
        | Integer | Boolean -> Set.empty
        | Name n -> Set.singleton n
        | Function(t1, t2) ->
            let v1 = t1.FreeTypeValues
            let v2 = t2.FreeTypeValues
            Set.union v1 v2

    member this.Apply(ts : TypeSubst) =
        match this with
        | Name n ->
            match ts.TryFind n with
            | Some t -> t
            | None -> Name n
        | Function(t1, t2) -> Function(t1.Apply ts, t2.Apply ts)
        | _ -> this

type TypeScheme with
    member this.FreeTypeValues =
        this.Type.FreeTypeValues - (Set.ofList this.Values)
    member this.Apply(s : TypeSubst) =
        { Values = this.Values
          Type =
              let newSubst =
                  List.fold (fun ns i -> Map.remove i ns) s this.Values
              this.Type.Apply newSubst }

type TypeEnv with
    member this.Remove(value : string) = { Schemes = this.Schemes.Remove value }
    member this.FreeTypeValues =
        Seq.fold
            (fun v (nts : System.Collections.Generic.KeyValuePair<string, TypeScheme>) ->
            Set.union v nts.Value.FreeTypeValues) Set.empty this.Schemes
    member this.Apply(ts : TypeSubst) =
        { Schemes = this.Schemes |> Map.map (fun k v -> v.Apply ts) }

let unionMap<'K, 'V when 'K : comparison> (s1 : Map<'K, 'V>) (s2 : Map<'K, 'V>) : Map<'K, 'V> =
    if s1.Count = 0 then s2
    else if s2.Count = 0 then s1
    else
        let keep2 =
            s2
            |> Seq.filter (fun kv -> not (s1.ContainsKey kv.Key))
            |> Array.ofSeq
        Seq.append s1 keep2
        |> Array.ofSeq
        |> Array.map (fun kv -> kv.Key, kv.Value)
        |> Map.ofArray

let singletonMap<'K, 'V when 'K : comparison> (k : 'K) (v : 'V) : Map<'K, 'V> =
    Map.ofSeq [ k, v ]

let composeSubst (s1 : TypeSubst) (s2 : TypeSubst) : TypeSubst =
    let ns2 = Map.map (fun k (v : SubstType) -> v.Apply s1) s2
    unionMap ns2 s1

let generalize (env : TypeEnv) (t : SubstType) =
    { Values = List.ofSeq (t.FreeTypeValues - env.FreeTypeValues)
      Type = t }

let newTyVal =
    let nextIndex = ref 1
    fun n ->
        let nn = sprintf "%s%d" n !nextIndex
        nextIndex := !nextIndex + 1
        Name nn

let instantiate (ts : TypeScheme) =
    let nvars = List.map (fun _ -> newTyVal "a") ts.Values
    let s = Map.ofSeq (Seq.zip ts.Values nvars)
    ts.Type.Apply s

let rec unify (t1 : SubstType) (t2 : SubstType) : TypeSubst =
    match t1, t2 with
    | Function(l1, r1), Function(l2, r2) ->
        let s1 = unify l1 l2
        let s2 = unify (r1.Apply s1) (r2.Apply s1)
        composeSubst s1 s2
    | Name u, t -> valBind u t
    | t, Name u -> valBind u t
    | Integer, Integer -> Map.empty
    | Boolean, Boolean -> Map.empty
    | _ -> failwith "Types do not unify: %A vs %A" t1 t2

and valBind (u : string) (t : SubstType) : TypeSubst =
    match t with
    | Name u -> Map.empty
    | _ when t.FreeTypeValues.Contains u ->
        failwith "Occur check fails: %s vs %A" u t
    | _ -> singletonMap u t

let rec ti (env : TypeEnv) (e : Expr) : TypeSubst * SubstType =
    match e with
    | Identifier n ->
        match env.Schemes.TryFind n with
        | None -> failwith "Unbound value: %s" n
        | Some s ->
            let t = instantiate s
            Map.empty, t
    | Literal l -> tiLit env l

and tiLit (env : TypeEnv) (l : Literal) : TypeSubst * SubstType =
    match l with
    | Literal.Int _ -> Map.empty, SubstType.Integer
    | Literal.Bool _ -> Map.empty, SubstType.Boolean
    | Literal.Float _ -> Map.empty, SubstType.Float
    | Literal.Char _ -> Map.empty, SubstType.Char
    | Literal.String _ -> Map.empty, SubstType.String

let typeInference (env : Map<string, TypeScheme>) (e : Expr) =
    let s, t = ti { Schemes = env } e
    t.Apply s

let test (e: Expr) =
    try
        let t = typeInference Map.empty e
        printfn "%A : %A" e t
    with :? System.ArgumentException -> printfn "Error in type checking"
