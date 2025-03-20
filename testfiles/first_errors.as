; missing comma
add r1 #40

; extra comma
mov a,, r0
mov , a, r5

; trailing text
sub r2, r3 error

; symbol illegal name
1pp: stop

; symbol reserved name
add: stop

; symbol illegal
Hello:

; symbol macro name
mcro m_mc
aaa
mcroend
m_mc: stop

; symbol name too long
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa: stop

; symbol redefinition
LIST: add r1, r2
LIST: sub r4, r7

; illegal string
STR: .string 123
.string "acf
.string gj4"

; missing operand
lea STR, 

; illegal number
.data as

; number out of range
.data 20000000

;invalid instruction
num r0