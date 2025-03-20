;line too long
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa

;extra comma
mcro ,mc
	add W
mcroend

;macro redefinition
mcro  m_error
	add W
	mov r1, r3
mcroend
mcro  m_error
	add W
	mov r1, r3
mcroend

;macro illegal name
mcro  1m
	add W
	mov r1, r3
mcroend
mcro  mov
	add W
	mov r1, r3
mcroend
mcro  r1
	add W
mcroend

;macro name too long
mcro aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa


;missing macro name
mcro  
	add W
	mov r1, r3
mcroend

;trailing text
mcro  m
	add W
	mov r1, r3
mcroend error
mcro  mmm error
	add W
	mov r1, r3
mcroend