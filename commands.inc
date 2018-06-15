%include "util.inc"
%include "macro.inc"
;1
   native drop, "drop", 0 ;ok
	pop rax
	jmp next
;2
   native swap, "swap", 0   ;ok
	pop rax
	pop rdi
	push rax
	push rdi
	jmp next
;3
   native rot, "rot", 0  ;abc bca ;ok
	pop rax
	pop rdi
	pop r9
	push rdi
	push rax
	push r9
	jmp next
;4
   native dup, "dup", 0  ;ok
	pop rax
	push rax
	push rax
	jmp next
;5
   native not, "not", 0	 ;ok
	pop rax
	test rax, rax
	jz .one
	push 0
	jmp next
	.one:
	push 1
	jmp next
;6
   native and, "and", 0    ; ok
	pop rax
	pop rdi	
	and rax, rdi
	push rax
	jmp next
;7
   native or, "or", 0     ;ok
	pop rax
	pop rdi
	or rax, rdi
	push rax
	jmp next
;8
   native land, "land", 0   ;ok
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
   native lor, "lor", 0    ;ok
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
 native count, "count" , 0       ;str -- len ----COLON------------------------------------------------------------------------
	pop rdi
	call string_length
	push rax
	jmp next
;11
   native to_ret, ">r", 0     ; ok
	pop rax
	mov [rstack], rax
	jmp next
;12
   native from_ret, "r>" , 0 	;ok
	push qword[rstack]
	add rstack, 8
	jmp next
;13
  native word, "word" , 0    ;addr -- len     ; ERROR+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	pop rdi
	mov rsi, 1024
	call read_word
	mov rdi, rax
	;call string_length
	push rdx
	jmp next
;14
   native r_fetch, "r@", 0   	;ok
	mov rax, [rstack]
	push rax
	jmp next
;15
   native plus, "+", 0     ;ok
	pop rax
	pop rdi
	add rax, rdi
	push rax
	jmp next
;16
   native minus, "-", 0    ;ok
	pop rax
	pop rdi
	sub rax, rdi	
	push rax
	jmp next
;17
     native multiply, "*", 0 ;ok
	pop rax
	pop rdi
	imul rdi
	push rax
	jmp next
;18 
    native divide, "/", 0     ;ok
	pop rax ;delim
	pop rdi ;delitel'
	idiv rdi
	push rax
	jmp next
 ;19
    native mod, "%", 0
	pop rax ;delim         ;ok
	pop rdi ;delitel'
	idiv rdi
	push rdx
	jmp next
 ;20 
   native equals, "=" , 0        ;ok
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
     native lt, "<", 0        ;ok
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
    native gt, ">", 0 	;ok
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
    native dot, ".", 0       ;ok
      pop rdi
      call print_int
      call print_newline
      jmp next 
;24 
     native find_word, "find_word", 0    ;ok???-------------------------------------------------------
       call find_word
       jmp next
;25 
   native cfa, "cfa", 0           ;ok???------------------------------------------------------------
       call cfa
       jmp next
;26
   native bye, "bye", 0		;ok
      call exit
;27 
  native fetch, "@", 0                ;ERROR+++++++++++++++++++++++++++++++++++++++++++++++++++
	pop rax
	push qword[rax]
	jmp next
;28 
   native write, "!", 0             ;ERROR+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	pop rax  ;addr
	pop rdi  ;value
	mov [rax], rdi
	jmp next
;29
   native read_char, "c@", 0
	xor rax, rax
	pop rdi
	mov al, byte[rdi]
	push rax
	jmp next
;30
   native write_char, "c!", 0
	pop rax ; addr
	pop rdx ; char
	mov [rax], dl
	jmp next
;31
   native emit, "emit", 0   ;print char      ;ok
	pop rdi
	call print_char
	jmp next
;32	
   native number, "number", 0 ;str -len num  
	pop rdi
	call parse_int
	push rdx
	push rax
	jmp next
;33
    native show, ".S", 0	;ERROR +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	mov rcx, rsp
     .go:
	;cmp rcx, hui
	je next
	mov rdi, [rcx]
	push rcx
	call print_int
	call print_newline
	pop rcx
	add rcx, 8
	jmp .go 
;34
   native lit, "lit", 0
	push qword [pc]
	add pc, 8
	jmp next	

;35
   native branch0, "0branch", 0
        pop rax
        test rax, rax
        jnz .skip
        mov pc, [pc]
        jmp next
     .skip:
        add pc, 8
        jmp next   
;36
   native branch, "branch", 0
        mov pc, [pc]
        jmp next


;  native docol, "docol", 0
;  native exit, "exit", 0	
;  native init, "init", 0
;  native last_word, "last_word", 0
;  native syscall, "syscall", 0
;  native mem, "mem", 0
;  native inbuf, "inbuf", 0
;  native state, "state", 0

;------------------------------------------------------

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
   lea rsi, [rsi + rax + 1 + 1]	;addr of the next word
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
	add rax, 10   ;+ null-term + dq + db
	ret

;----------------------------------------------------


