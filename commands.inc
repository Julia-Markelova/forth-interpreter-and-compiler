%include "util.inc"
%include "macro.inc"
;1
   native drop, "drop", 0 
	pop rax
	jmp next
;2
   native swap, "swap", 0   
	pop rax
	pop rdi
	push rax
	push rdi
	jmp next
;3
   native rot, "rot", 0  
	pop rax
	pop rdi
	pop r9
	push rdi
	push rax
	push r9
	jmp next
;4
   native dup, "dup", 0  
	pop rax
	push rax
	push rax
	jmp next
;5
   native not, "not", 0	
	pop rax
	test rax, rax
	jz .one
	push 0
	jmp next
	.one:
	push 1
	jmp next
;6
   native and, "and", 0    
	pop rax
	pop rdi	
	and rax, rdi
	push rax
	jmp next
;7
   native or, "or", 0    
	pop rax
	pop rdi
	or rax, rdi
	push rax
	jmp next
;8
   native land, "land", 0   
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
   native lor, "lor", 0    
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
 native count, "count" , 0       ;str -- len 
	pop rdi
	call string_length
	push rax
	jmp next
;11
   native to_ret, ">r", 0     
	pop rax
	mov [rstack], rax
	jmp next
;12
   native from_ret, "r>" , 0 	
	push qword[rstack]
	add rstack, 8
	jmp next
;13
  native word, "word" , 0    ;addr -- len     
	pop rdi
	mov rsi, 1024
	call read_word
	mov rdi, rax
	;call string_length
	push rdx
	jmp next
;14
   native r_fetch, "r@", 0   	
	mov rax, [rstack]
	push rax
	jmp next
;15
   native plus, "+", 0     
	pop rax
	pop rdi
	add rax, rdi
	push rax
	jmp next
;16
   native minus, "-", 0   
	pop rax
	pop rdi
	sub rax, rdi	
	push rax
	jmp next
;17
     native multiply, "*", 0 
	pop rax
	pop rdi
	imul rdi
	push rax
	jmp next
;18 
    native divide, "/", 0     
	pop rax ;delim
	pop rdi ;delitel'
	idiv rdi
	push rax
	jmp next
 ;19
    native mod, "%", 0
	pop rax ;delim         
	pop rdi ;delitel'
	idiv rdi
	push rdx
	jmp next
 ;20 
   native equals, "=" , 0       
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
     native lt, "<", 0       
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
    native gt, ">", 0 	
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
    native dot, ".", 0       
      pop rdi
      call print_int
      call print_newline
      jmp next 
;24 
     native find_word, "find_word", 0   
       call find_word
       jmp next
;25 
   native cfa, "cfa", 0          
       call cfa
       jmp next
;26
   native bye, "bye", 0		;ok
      call exit
;27 
  native fetch, "@", 0                
	pop rax
	push qword[rax]
	jmp next
;28 
   native write, "!", 0            
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
   native emit, "emit", 0   ;print char      
	pop rdi
	call print_char
	call print_newline
	jmp next
;32	
   native number, "number", 0 ;str -len num  
	pop rdi
	call parse_int
	push rdx
	push rax
	jmp next
;33
    native show, ".S", 0	
	mov rcx, rsp
     .go:
	cmp rcx, qword[stack_end]
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

;37 
    native exit, "exit", 0
	mov pc, [rstack]
	add rstack, 8
	jmp next
;38
   native semicolon, ";", 1
        mov r8, [here]
	mov qword[r8], xt_exit
        add qword[here], 8
	mov byte[state], 0
	jmp next
;39
    native docol, "docol", 0
	sub rstack, 8
	mov [rstack], pc
	add w, 8
	mov pc, w
	jmp next
;40
    native colon, ":", 0
	mov rdi, input_buf
	mov rsi, 1024
	call read_word 		    ;rax = ptr, rdx = len
	mov rdi, rax 		    ;rax = ptr to word         ----------;
	mov rsi, [here]		    ;rsi = buf to save			 ;
	push rsi	  						 ;
	mov rdx, 1024		    ;rdx = buf size			 ; save word
	call string_copy	    ;save func name in mem  rdx = len    ;
	add qword[here], rdx        ;len				 ;
	add qword[here], 1 	    ;null-term  	       ----------;

	mov r8, [here]
	mov r9, [last_word]         ;save lw	               ----------;
	mov qword[r8], r9           ;[here] <- lw			 ;
	pop rsi								 ;
	mov qword[last_word], rsi    ;new lw				 ; save ptr to prev and state
	add qword[here], 9	    ;lw + state		       ----------;

	mov r8, [here]		    
	mov qword[r8], docol_impl
	add qword[here], 8	    ;free mem
	mov qword[state], 1         ;update state
	jmp next
	
;41
    native state, "state", 0
	mov rdi, [state]
	call print_int
	call print_newline
	jmp next 

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
	add rax, 10   ;+ null-term + dq + db
	ret

;----------------------------------------------------


