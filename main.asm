global _start

%define pc r13
%define w r14
%define rstack  r15

section .bss
              resq 1023
rstack_start: resq 1
input_buf:    resq 1024

section .data
error: db "Error : unknown word.", 10, 0

program_stub: dq 0
xt_interpreter: dq .interpreter
.interpreter: dq interpreter_loop

section .text

%include "commands.inc"

_start:
	mov rstack, rstack_start
	mov pc, xt_interpreter
	jmp next

next:   
	mov w, pc
	add pc, 8
	mov w, [w]
	jmp [w]

interpreter_loop:
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
	;mov rdi, rax
	;mov rdi, input_buf
	;call parse_int	;check if word is a number
	;test rdx, rdx
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
	
	


