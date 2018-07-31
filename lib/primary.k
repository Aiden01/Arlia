`
    +----------------------------------------------+
    | This file is the basic standard import file. |
    +----------------------------------------------+

`

define MAX      (real.MaxValue);
define MIN      (real.MinValue);
define NOLIMITE (-1);

` A virtual object is an object that has a null size and that doesn't exist in the resulting code, but accepting any values `
object <typesize = 0> virtual;

rule unsigned[] : real = (0 to MAX);
rule voidable : virtual;
rule eof : virtual = -1;

structure sFile {
    var unused : integer;
}

data FILE : sFile;

ifdef WIN32 {
    define GETCHAR  USESTD("getchar", "msvcrt", "win32a.inc");
    define PUTCHAR  USESTD("putchar", "msvcrt", "win32a.inc");
    define GETCH    USESTD("getch", "msvcrt", "win32a.inc");
    define PRINTF   USESTD("printf", "msvcrt", "win32a.inc");
    define SCANF    USESTD("scanf", "msvcrt", "win32a.inc");
    define MALLOC   USESTD("malloc", "msvcrt", "win32a.inc");
    define RAND     USESTD("rand", "msvcrt", "win32a.inc");
    define SRAND    USESTD("rand", "msvcrt", "win32a.inc");
    define TIME     USESTD("time", "msvcrt", "win32a.inc");
    define CLOCK    USESTD("clock", "msvcrt", "win32a.inc");
    define FGET     USESTD("fget", "msvcrt", "win32a.inc");
    define FPUTS    USESTD("fputs", "msvcrt", "win32a.inc");
    define FPRINTF  USESTD("fprintf", "msvcrt", "win32a.inc");
    define FSCANF   USESTD("fscanf", "msvcrt", "win32a.inc");
    define FREAD    USESTD("fread", "msvcrt", "win32a.inc");
    define FWRITE   USESTD("fwrite", "msvcrt", "win32a.inc");
    define FCLOSE   USESTD("fclose", "msvcrt", "win32a.inc");
    define FFLUSH   USESTD("fflush", "msvcrt", "win32a.inc");
    define FSEEK    USESTD("fseek", "msvcrt", "win32a.inc");
    define FEOF     USESTD("feof", "msvcrt", "win32a.inc");
    define FGETC    USESTD("fgetc", "msvcrt", "win32a.inc");
    define SSCANF   USESTD("sscanf", "msvcrt", "win32a.inc");
    define VFPRINTF USESTD("vfprintf", "msvcrt", "win32a.inc");
    define VPRINTF  USESTD("vprintf", "msvcrt", "win32a.inc");
    define FGETS    USESTD("fgets", "msvcrt", "win32a.inc");
    define GETS     USESTD("gets", "msvcrt", "win32a.inc");
    define GETC     USESTD("getc", "msvcrt", "win32a.inc");
    define PUTC     USESTD("putc", "msvcrt", "win32a.inc");
    define PUTS     USESTD("puts", "msvcrt", "win32a.inc");
    define FPUTC    USESTD("fputc", "msvcrt", "win32a.inc");
    define SPRINTF  USESTD("sprintf", "msvcrt", "win32a.inc");
    define UNGETC   USESTD("ungetc", "msvcrt", "win32a.inc");
}
