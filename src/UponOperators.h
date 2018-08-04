#pragma once

enum Operators {
	Op_Add,        // +
	Op_Sub,        // -
	Op_Mult,       // *
	Op_Div,        // /
	Op_Mod,        // %
	Op_Inc,		   // ++
	Op_Dec,		   // --
	Op_AddEq,      // +=
	Op_SubEq,      // -=
	Op_MultEq,     // *=
	Op_DivEq,      // /=
	Op_ModEq,      // %=
	Op_Concat,     // ^
	Op_ConcatAdd,  // ^+
	Op_ConcatSub,  // ^-
	Op_ConcatMult, // ^*
	Op_ConcatDiv,  // ^/
	Op_ConcatMod,  // ^%
	Op_Hooks	   // [..]
};
