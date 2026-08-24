; echo.asm
; Reads keystrokes one at a time and echoes them back until Enter is pressed.
; Demonstrates: TRAP GETC, TRAP OUT, conditional branching (BR), looping.

.ORIG x3000

LEA R0, PROMPT
PUTS                ; print the instructions once

LOOP    GETC                ; TRAP x20 - blocking read, char goes into R0
        OUT                 ; TRAP x21 - echo the char in R0 back to the screen

        LD R1, ENTER_KEY    ; load the ASCII code for Enter (carriage return)
        NOT R1, R1
        ADD R1, R1, #1      ; R1 = -ENTER_KEY  (two's complement negation)
        ADD R2, R0, R1      ; R2 = (char just read) - ENTER_KEY

        BRnp LOOP           ; if R2 != 0 (char wasn't Enter), loop again
                             ; falls through only when char == Enter

LEA R0, DONE
PUTS
HALT

PROMPT     .STRINGZ "Type characters, press Enter to stop:\n"
DONE       .STRINGZ "\nDone!\n"
ENTER_KEY  .FILL x000A       ; ASCII line feed

.END