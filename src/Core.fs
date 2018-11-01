module Core.ASM

type Register =
    | General of General
    | Segment of Segment
and General =
    | AL  | CL  | DL  | BL  | AH  | CH  | DH  | BH
    | AX  | CX  | DX  | BX  | SP  | BP  | SI  | DI
    | EAX | ECX | EDX | EBX | ESP | EBP | ESI | EDI
and Segment =
    | ES  | CS  | SS  | DS  | FS  | GS

type Memory = Memory of Register

type Instruction =

    (* ------ mov ------ *)

    /// Mov register to register
    | MovRR of Register * Register // mov reg, reg
    /// Mov register to memory
    | MovRM of Memory * Register   // mov [reg], reg
    /// Mov memory to register
    | MovMR of Register * Memory   // mov reg, [reg]
    /// Mov int to register
    | MovIR of Register * int      // mov reg, int
    /// Mov int to memory
    | MovIM of Memory * int        // mov [reg], int
    /// Mov value to register
    | MovVR of Register * string   // mov reg, [val]
    /// Mov value to memory
    | MovVM of Memory * string     // mov [reg], [val]

    (* ------ push ------ *)

    /// Push register
    | PushR of Register             // push reg
    /// Push int
    | PushI of int                  // push int
    /// Push value
    | PushV of string               // push [val]
    /// Push memory
    | PushM of Memory               // push [reg]

    (* ------ pop ------ *)

    /// Pop register
    | PopR of Register              // pop reg
    /// Pop int
    | PopI of int                   // pop int
    /// Pop value
    | PopV of string                // pop [val]
    /// Pop memory
    | PopM of Memory                // pop [reg]

    (* ------ add ------ *)

    /// Add register to register
    | AddRR of Register * Register  // add reg, reg
    /// Add memory to register
    | AddMR of Memory * Register    // add reg, [reg]
    /// Add register to memory
    | AddRM of Register * Memory    // add [reg], reg
    /// Add int to register
    | AddIR of int * Register       // add reg, int
    /// Add value to register
    | AddVR of string * Register    // add [val], reg

    (* ------ inc ------ *)

    /// Inc register by one
    | IncR of Register              /// inc reg
    /// Inc memory by one
    | IncM of Memory                /// inc byte [reg]
    
    (* ------ sub ------ *)

    /// Sub register to register
    | SubRR of Register * Register  // sub reg, reg
    /// Sub memory to register
    | SubMR of Memory * Register    // sub reg, [reg]
    /// Sub register to memory
    | SubRM of Register * Memory    // sub [reg], reg
    /// Sub int to register
    | SubIR of int * Register       // sub reg, int
    /// Sub value to register
    | SubVR of string * Register    // sub [val], reg

    (* ------ dec ------ *)

    /// Dec register by one
    | DecR of Register              /// dec reg
    /// Dec memory by one
    | DecM of Memory                /// dec byte [reg]

    (* ------ imul ------ *)

    /// Imul register to register
    | ImulRR of Register * Register  // imul reg, reg
    /// Imul memory to register
    | ImulMR of Memory * Register    // imul reg, [reg]
    /// Imul register to memory
    | ImulRM of Register * Memory    // imul [reg], reg
    /// Imul int to register
    | ImulIR of int * Register       // imul reg, int
    /// Imul value to register
    | ImulVR of string * Register    // imul [val], reg

    (* ------ div ------ *)

    /// Div register to register
    | DivRR of Register * Register  // div reg, reg
    /// Div memory to register
    | DivMR of Memory * Register    // div reg, [reg]
    /// Div register to memory
    | DivRM of Register * Memory    // div [reg], reg
    /// Div int to register
    | DivIR of int * Register       // div reg, int
    /// Div value to register
    | DivVR of string * Register    // div [val], reg
