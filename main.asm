global _start

%define pc r13
%define w r14
%define rstack  r15
;---------------------------------------------------------
section .bss
              resq 1023
rstack_start: resq 1
dict:         resq 65536  ; memory for words
mem:          resq 65536  ; memory for user
state:        resq 1
input_buf:    resq 1024

%include "commands.inc"
;---------------------------------------------------------
section .data
error:          db "Error: unknown word.", 10, 0
program_stub:   dq 0
xt_interpreter: dq .interpreter
.interpreter:   dq interpreter_loop
xt_compiler:    dq .compiler
.compiler:	dq compiler_loop
here: 		dq dict
memory: 	dq mem
last_word: 	dq last

;---------------------------------------------------------
section .text

_start:
	mov rstack, rstack_start
	mov pc, xt_interpreter
	jmp next
    
next:   
	mov w, pc
	add pc, 8
	mov w, [w]
	jmp [w]

;----------------------------------------------------------
interpreter_loop:
	cmp byte[state], 0  ; check the state
	jz .inter
	mov pc, xt_compiler
	jmp next
    .inter:
	mov rdi, input_buf  ;place to save word
	mov rsi, 1024       ;size of buf
	call read_word
	test rax, rax       ;if the string is too big for buffer
	jz .exit
	test rdx, rdx       ;rdx = str's length
	jz .exit
	mov rdi, rax        ;rdi = pointer to a key
	
	mov rsi, [last_word];rsi = pointer to the last word in a dict
	push rdi	    ;save pointer to a key
	call find_word
	pop rdi
	test rax, rax
	jz .no_word
	mov rdi, rax        ;rdi = addr
	call cfa
	mov [program_stub], rax		;magic
	mov pc, program_stub		;magic
	jmp next
.exit:
	mov pc, xt_interpreter
	jmp next

.no_word:
	;mov rdi, rax
	mov rdi, input_buf
	call parse_int	        ;check if word is a number
	test rdx, rdx
	jz .error		; if no number
	push rax
	mov pc, xt_interpreter
	jmp next
.error:
	mov rdi, error
	call string_length
	mov rdx, rax              ;size to print (bytes)                     
	mov rax, 1                ;write
	mov rsi, error            ;address
	mov rdi, 2                ;stderr
	syscall
	mov pc, xt_interpreter
	jmp next
	
;-----------------------------------------------------------
compiler_loop:
	cmp byte[state], 0  ; check the state
	jnz .compiler
	mov pc, xt_interpreter
	jmp next
   .compiler:
	mov rdi, input_buf
	mov rsi, 1024
	call read_word
	test rax, rax
	jz .exit
	test rdx, rdx
	jz .exit
	
	mov rsi, [last_word]     
	mov rdi, rax       ;rdi = pointer to a key
	push rdi	   ;save pointer to a key	
	call find_word
	pop rdi 	
	test rax, rax 	   ;any word?
	jz .no_word
	
	mov rdi, rax       ; rdi = addr
	call cfa	
	sub rax, 1         ;rax = xt, rax - 1 = state
	cmp byte[rax], 1   ;immediate?
	jz .immediate
	
	inc rax            ;rax = xt
	mov r8, [here]          
	mov qword[r8], rax      
	add qword[here], 8	       
	mov pc, xt_interpreter
	jmp next
.immediate:
        inc rax	
	mov [program_stub], rax		
	mov pc, program_stub		
	jmp next
.exit:
	mov pc, xt_interpreter
	jmp next
.no_word:
	;mov rdi, rax
	mov rdi, input_buf
	call parse_int	        ;check if word is a number
	test rdx, rdx
	jz .error		; if no number
	
	sub qword[here], 8      ;---------------
	mov r9, [here]          ;check prev word
	cmp r9, xt_branch       ;---------------
	jz .br
	cmp r9, xt_branch0
	jz .br
	add qword[here], 8      ;return to free mem
	mov r8, [here]
	mov qword[r8], xt_lit
	add qword[here], 8
	mov r8, [here]   
	mov qword[r8], rax      ;rax = number   
	add qword[here], 8	;free mem
	mov pc, xt_interpreter
	jmp next
.br:
	add qword[here], 8      ;return to free mem
	mov r8, [here]
	mov qword[r8], rax    ;rax = number
	add qword[here], 8	;free mem
	mov pc, xt_interpreter
	jmp next
.error:
	mov rdi, error
	call string_length
	mov rdx, rax              ;size to print (bytes)                     
	mov rax, 1                ;write
	mov rsi, error            ;address
	mov rdi, 2                ;stderr
	syscall
	mov pc, xt_interpreter
	jmp next
	


	


