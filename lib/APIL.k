` This file constitutes all the macros of the language `

define BYTE             1;
define WORD             2;
define DWORD            4;
define FWORD            6;
define QWORD            8;
define TWORD            10;
define DQWORD           16;
define QQWORD           32;
define ZWORD            64;

define object           typed structure;

define nullchr          (0b);
define nullptr          (0);

define MAXCHAR          ((127)c);
define MINCHAR          ((-128)c);
define MAXINT           ((2147483647)i);
define MININT           ((-2147483648)i);
define MAXFLOAT         (((2 ** 63) - 1)f);
define MINFLOAT         ((-(2 ** 63))f);
define MAXREAL          ((2 ** (2 ** 14) - 1)r);
define MINREAL          ((-(2 ** (2 ** 14)))r);
