; count.asm
; Prints the digits 0 through 9, one per line.
; Demonstrates: ADD (immediate + register), LD, LEA, BR, looping with a
; counter and a fixed limit - i.e. instructions actually composed into
; real control flow, not just single trap calls in isolation.

.ORIG x3000

AND R1, R1, #0      ; R1 = 0   -> our loop counter
LD R2, LIMIT        ; R2 = 10  -> stop when counter reaches this
LD R3, ASCII_ZERO   ; R3 = '0' -> used to convert 0-9 into a printable digit
LD R4, NEWLINE

LOOP
    ADD R0, R1, R3      ; R0 = counter + '0'  -> ASCII digit for current count
    OUT                 ; TRAP x21 - print the digit

    ADD R0, R4, #0      ; R0 = newline char
    OUT                 ; print newline

    ADD R1, R1, #1      ; counter++
    NOT R5, R2
    ADD R5, R5, #1      ; R5 = -LIMIT
    ADD R5, R1, R5       ; R5 = counter - LIMIT

    BRn LOOP            ; loop again while (counter - LIMIT) is still negative

HALT

LIMIT       .FILL #10
ASCII_ZERO  .FILL x0030
NEWLINE     .FILL x000A

.END