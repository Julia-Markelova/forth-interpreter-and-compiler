%include "util.inc"
%include "macro.inc"
;1
   native drop, "drop"
	pop rax
	mov rdi, 42
	call print_char
	jmp next
;2
   native swap, "swap"   ;ok
	pop rax
	pop rdi
	push rax
	push rdi
	jmp next
;3
   native rot, "rot"  ;abc bca
	pop rax
	pop rdi
	pop r9
	push rdi
	push r9
	push rax
	jmp next
;4
   native dup, "dup"
	pop rax
	push rax
	push rax
	jmp next
;5
   native not, "not"	
	pop rax
	test rax, rax
	jz .one
	push 0
	jmp next
	.one:
	push 1
	jmp next
;6
   native and, "and"
	pop rax
	pop rdi	
	and rax, rdi
	push rax
	jmp next
;7
   native or, "or"
	pop rax
	pop rdi
	or rax, rdi
	push rax
	jmp next
;8
   native land, "land"
	pop rax
	pop rdi
	test rax, rax
	jz .zero
	push rdi
	jmp next
	.zero:
	push rax
	jmp next
;9
   native lor, "lor"
	pop rax
	pop rdi
	test rax, rax
	jz .zero
	push rax
	jmp next
	.zero:
	push rdi
	jmp next
;10  
 native count, "count"        ;str -- len
	pop rdi
	call string_length
	push rax
	jmp next
;11
   native to_ret, ">r"     ;???????????????
	pop rax
	mov [rstack], rax
	jmp next
;12
   native from_ret, "r>"  ;????????????????
	push qword[rstack]
	add rstack, 8
	jmp next
;13
  native word, "word"     ;addr -- len
	pop rdi
	mov rsi, 1024
	call read_word
	mov rdi, rax
	call string_length
	push rax
	jmp next
;14
   native r_fetch, "r@"   ;rs - ds
	mov rax, [rstack]
	push rax
	jmp next
;15
   native plus, "+"
	pop rax
	pop rdi
	add rax, rdi
	push rax
	jmp next
;16
   native minus, "-"
	pop rax
	pop rdi
	sub rax, rdi	
	push rax
	jmp next
;17
     native multiply, "*"
	pop rax
	pop rdi
	imul rdi
	push rax
	jmp next
;18 
    native divide, "/"
	pop rax ;delim
	pop rdi ;delitel'
	idiv rdi
	push rax
	jmp next
 ;19
    native mod, "%"
	pop rax ;delim
	pop rdi ;delitel'
	idiv rdi
	push rdx
	jmp next
 ;20 
   native equals, "="
      pop rsi
      pop rdi
      cmp rdi, rsi
      je .equals
      push 0
      jmp next
   .equals:
      push 1
      jmp next
;21 
     native lt, "<"
      pop rsi
      pop rdi
      cmp rsi, rdi
      jg .greather
      push 0
      jmp next
   .greather:
      push 1
      jmp next
;22 
    native gt, ">"
      pop rsi
      pop rdi
      cmp rdi, rsi
      jg .greather
      push 0
      jmp next
   .greather:
      push 1
      jmp next
;23 
    native dot, "."       ;ok
      pop rdi
      call print_int
      call print_newline
      jmp next 
;24 
     native find_word, "find_word"
       call find_word
       jmp next
;25 
   native cfa, "cfa"
       call cfa
       jmp next
;26
   native bye, "bye"
      call exit
;27
  native fetch, "@"
	pop rax
	push qword[rax]
	jmp next
;28 
   native write, "!"
	pop rax  ;addr
	pop rdi  ;value
	mov [rax], rdi
	jmp next
;29
   native read_char, "c@"
	xor rax, rax
	pop rdi
	mov al, byte[rdi]
	push rax
	jmp next
;30
   native write_char, "c!"
	pop rax ; addr
	pop rdx ; char
	mov [rax], dl
	jmp next
;31
   native emit, "emit"   ;print char
	pop rdi
	call print_char
	jmp next
;32	
   native number, "number" ;str -len num
	pop rdi
	call parse_int
	push rdx
	push rax
	jmp next
;33

;  native docol, "docol"
;  native show, ".S"
;  native exit, "exit"	
;  native branch, "branch"
;  native branch0, "branch0"
;  native init, "init"

;  native last_word, "last_word"
;  native syscall, "syscall"
;  native lit, "lit"
;  native mem, "mem"
;  native inbuf, "inbuf"
;  native state, "state"

;------------------------------------------------------

;rdi pointer to a key

;rdi pointer to a key
;rsi pointer to the last word in a dictionary
find_word:
.go:
   push rdi
   push rsi 
   call string_equals
   pop rsi
   pop rdi
   cmp rax, 1                    ;if strings are equals, rax=1 
   jz .end    
   push rdi
   mov rdi, rsi            
   call string_length
   pop rdi
   lea rsi, [rsi + rax + 1]	;addr of the next word
   mov rsi, [rsi]
   test rsi, rsi  		;end of a dict?
   jz .no_word
   jmp .go    
.end:
   mov rax, rsi 
   ret
.no_word:
   mov rax, 0
   ret

;----------------------------------------------------
;rdi - addr
cfa:
	;lea rdi, [rdi + 8]
	push rdi  ; save addr
	call string_length 
	pop rdi
	add rax, rdi
	add rax, 9   ;+ null-term + dq
	ret

;----------------------------------------------------

;   find_word_func:
;   .loop:
;       push rdi
;       lea rdi, [rdi + 8]
;       call string_equals
;       pop rdi
;       cmp rax, 0
;       je .find
;       mov rdi, [rdi]
;       cmp rdi, 0
;       je .fail 
;       jmp .loop
;       .find:
;           mov rax, rdi
;           ret
;       .fail:
;           mov rax, 0
;           ret




   ;cfa_func:
    ;   lea rdi, [rdi + 8]
     ;  push rdi
      ; call string_length
       ;pop rdi
       ;lea rax, [rdi + rax + 1]
       ;ret

