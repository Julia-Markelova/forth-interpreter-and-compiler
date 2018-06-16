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
error:          db "Error : unknown word.", 10, 0
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
	
	mov rsi, last       ;rsi = pointer to the last word in a dict
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
	mov rdi, rax
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
	mov rdi, input_buf
	mov rsi, 1024
	call read_word
	test rax, rax
	jz .exit
	test rdx, rdx
	jz .exit
	mov rsi, last
	mov rdi, rax       ;rdi = pointer to a key
	push rdi
	call find_word
	pop rdi
	test rax, rax 
	jz .no_word
	mov rdi, rax       ; rdi = addr
	call cfa	
	mov r9, rax        ; r9 = xt
	mov r8, r9
	sub r8, 1
	cmp r8, 1
	jz .immediate
	mov r8, [here]           ;------- -------;
	mov r8, [r8]             ;[[here]] <- xt ; 
	mov qword[here], r8      ;---------------;
	mov qword[here], r9
	add qword[here], 8	        ;magic
	mov pc, xt_interpreter
	jmp next
.immediate:	
	mov [program_stub], rax		;magic
	mov pc, program_stub		;magic
	jmp next
.exit:
	mov pc, xt_interpreter
	jmp next
.no_word:
	mov rdi, rax
	mov rdi, input_buf
	call parse_int	        ;check if word is a number
	test rdx, rdx
	jz .error		; if no number
	mov qword[here], r9
	sub r9, 8
	cmp qword[r9], xt_branch
	jz .br
	cmp qword[r9], xt_branch0
	jz .br
	mov qword[here], xt_lit
	add qword[here], 8
	mov qword[here], rax
	add qword[here], 8
	mov pc, xt_interpreter
	jmp next
.br:
	mov r8, qword[here]
	mov r8, [r8]
	mov qword[here], r8
	mov qword[here], rax
	add qword[here], 8
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
	


	


