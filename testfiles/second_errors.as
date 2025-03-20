; symbol undefined
jmp &LOOP
sub x, r6

; symbol entry undefined
.entry LOOP

; instruction addressing not allowed
STR: add r1, r0
mov r0, &STR
