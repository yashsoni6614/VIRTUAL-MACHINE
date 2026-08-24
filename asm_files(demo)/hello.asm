; hello.asm
; Smoke-test program: prints a greeting and halts.
; Demonstrates: .ORIG/.END, TRAP PUTS, TRAP HALT, .STRINGZ

.ORIG x3000

LEA R0, GREETING    ; R0 = address of the string
PUTS                ; TRAP x22 - print null-terminated string at address in R0
HALT                ; TRAP x25 - stop execution

GREETING .STRINGZ "Hello from my LC-3 VM!\n"

.END