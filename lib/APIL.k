`
	+----------+
	|          |
	|   APIL   |
	|		   |
	+----------+
`

define BYTE 1;
define WORD 2;
define DWORD 4;
define FWORD 6;
define QWORD 8;
define TWORD 10;
define DQWORD 16;
define QQWORD 32;
define ZWORD 64;

proc "object"    data structure;
proc "rule"      data ret;
proc "_asm"(asm) extern asm;
proc "breakpoint" _asm("int 3");

object <typesize = BYTE> boolean {  };
object <typesize = BYTE> char {  };
object <typesize = WORD> string {  };
object <typesize = DWORD> integer {  };
object <typesize = QWORD> float {  };
object <typesize = TWORD> real {  };
