%include "util.inc"
%include "macro.inc"

   native drop, "drop"
   native swap, "swap"
   native rot, "rot"
   native dup, "dup"
   native not, "not"
   native and, "and"
   native or, "or"
   native land, "land"
   native lor, "lor"

   native show, ".S"
   native exit, "exit"
   native to_ret, ">r"
   native from_ret, "r>"
   native r_fetch, "r@"
   native emit, "emit"
   native word, "word"
   native number, "number"
   native branch, "branch"
   native branch0, "branch0"
   native fetch, "@"
   native write, "!"
   native write_char, "c!"
   native plus, "+"
   native minus, "-"
   native multiply, "*"
   native divide, "/"
   native mod, "%"
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

   native dot, "."
      pop rdi
      call print_int
      call print_newline
      jmp next 

   native find_word, "find_word"
       call find_word
       jmp next

native bye, "bye"
      call exit

   native cfa, "cfa"
       call cfa
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

