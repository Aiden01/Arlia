; Example of a generated code:
; func main() {
;     @dio:display.writeln("Hello, world!");
; }

format PE
entry __main
include 'INCLUDE\win32a.inc'

section '.idata' data readable import
        library kernel32, 'kernel32.dll', \
			msvcrt, 'msvcrt.dll'
        import kernel32, ExitProcess, 'ExitProcess'
        import msvcrt, puts, 'puts'

section '.data' data readable
	GV0 db "Hello, world!", 0

section '.code' code executable

____dio___display__write:
    push ebp
    mov ebp, esp
	push edi
    call [puts]
    pop ebp
    ret

__main:
    push ebp
    mov ebp, esp
    mov dword edi, GV0
    call ____dio___display__write
	push 0
	call [ExitProcess]
