section .data

string: db '  23 ', 0
str: db 'here8l', 0

section .bss
puk: resb 6

section .text
 
global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global read_char
global read_word
global parse_uint
global parse_int
global string_equals
global string_copy

;--------------------------------------

exit:
mov rax, 60
syscall

;--------------------------------------

string_length:
   xor rax, rax              ;counter   
.go:
   mov dl, byte[rdi+rax]     ;next symbol by incrementing a pointer
   test dl, dl               ;testing for null-terminator
   jz .end                   ;if it is the end of the string, then end 
   inc rax                   ;incrementing counter
   jmp .go                   
.end:
   ret

;--------------------------------------

print_string:
call string_length        ;count our string's length
mov rdx, rax              ;size to print (bytes)                     
mov rax, 1                ;write
mov rsi, rdi              ;address
mov rdi, 1                ;stdout
syscall
ret

;---------------------------------------

print_char:
mov r11, rsp             ;saving rsp
dec rsp
mov byte[rsp], dil       ;memoring our char
mov rax, 1               ;write
mov rdi,1                ;stdout
mov rdx, 1               ;size to print (bytes) 
mov rsi, rsp             ;address
mov rsp, r11             ;restore rsp
syscall
ret

;----------------------------------------

print_newline:
mov dil, 10              ;code for new line  
call print_char
ret

;----------------------------------------

print_uint:
   mov rax, rdi              ;divident
   mov r10, 10               ;divisor
   xor r9, r9                ;counter
   mov r11, rsp              ;saving rsp                   
   dec rsp                   ;no segfault
.go:
   xor rdx, rdx              ;if it <> 0, code isn't working  
   div r10
   add rdx, 0x30             ;get a code for ostatok
   mov byte[rsp], dl         ;save ostatok in the stack          
   inc r9                    ;count for a digits                           
   test rax, rax             ;if delimoe =0, then we finish to delit'
   jz .go2                   ;finish to delit'
   dec rsp                   ;put in the stack at another order
   jmp .go                   ;else repeat
.go2:
   mov rax, 1                ;write 
   mov rsi, rsp              ;address
   mov rdi, 1                ;stdout
   mov rdx, r9               ;size  
   mov rsp, r11;             ;restore rsp
   syscall
   ret

;---------------------------------------------------

print_int:
   cmp rdi, 0                ;check our number
   jge .uint                 ;if =>0, then do print_uint 
   mov rax, rdi              ;divident
   neg rax                   ;div positive number
   mov r10, 10               ;divisor
   xor r9, r9                ;counter
   mov r11, rsp              ;saving rsp                   
   dec rsp                   ;no segfault
.go:
   xor rdx, rdx              ;if it <> 0, code isn't working  
   div r10
   add rdx, 0x30             ;get a code for ostatok
   mov byte[rsp], dl         ;save ostatok in the stack          
   inc r9                    ;count for a digits                          
   test rax, rax             ;if delimoe =0, then we finish to delit'
   jz .go2                   ;finish to delit'
   dec rsp                   ;put in the stack at another order
   jmp .go                   ;else repeat
.go2:
   dec rsp                   ;it's for a '-'
   inc r9                    ;it's for a '-'
   mov byte[rsp], 45         ;add '-' 
   mov rax, 1                ;write 
   mov rsi, rsp              ;address
   mov rdi, 1                ;stdout
   mov rdx, r9               ;size  
   mov rsp, r11;             ;restore rsp
   syscall
   ret               
.uint:
   call print_uint
   ret

;----------------------------------------------------

read_char:
    mov r9, rsp               ;saving rsp
    dec rsp
    mov rax, 0                ;read
    mov rsi, rsp              ;address
    mov rdi, 0                ;stdin
    mov rdx, 1                ;size
    syscall
    mov al, byte[rsp]  
    mov rsp, r9               ;restore rsp
   ; mov rdi, rax
   ; call print_uint
    ret
  
;-----------------------------------------------------

read_word:
   ;rdi: buffer
   ;rsi: size
   xor r8, r8                ;counter
   mov r11, rsi              ;save size  
   mov r10, rdi              ;save buffer                
.go:
   call read_char
   cmp al, 0x20              ;space?
   jz .word 
   cmp al, 0x9               ;tab?
   jz .word
   cmp al, 0xA               ;new line?
   jz .word
   cmp al, 0                 ;null-terminator?
   jz .word
   mov byte[r10+r8], al      ;save n'st symbol
   inc r8                    ;inc counter
   jmp .go
.word: 
   test r8, r8               ;have we symbols?
   jz .go
   cmp r11, r8               ;is the string too big for buffer or not?
   jz .end
   inc r8         
   mov byte[r10+r8], 0       ;if it is the end of word, then null-terminate
   mov rax, r10              ;return buffer's address
   mov rdx, r8         ;ADDED
   ret   
.end:
   mov rax, 0
   ret
;------------------------------------------------------

parse_uint:          
  ;rdi=pointer
  xor r10,r10
  xor rdx, rdx             ;clear rdx (length)
  xor rax,rax              ;clear rax (number)
  xor r9, r9               ;counter
  mov r8, 10               ;multiplier
.go:
  mov r10b, byte[rdi+r9]   ;n'st sumbol from the string 
  cmp r10b, 48             ;0's code is 48  
  jb .end                  ;if it isn't a digit, then end
  cmp r10b, 57             ;9's code is 57
  ja .end                  ;if it isn't a digit, then end
  sub r10b, 0x30           ;no ascii
  mul r8                   ;next razryad       
  add al, r10b             ;save digit         
  inc r9                   ;inc counter
  jmp .go
.end:
  mov rdx, r9              ;length
  ret
   
;------------------------------------------------------
 
parse_int:
   mov dl, byte[rdi]       ;check the sign
   cmp dl, 45              ; '-'
   jz .neg
   call parse_uint
   ret
.neg:
   inc rdi                ;get from the second symbol
   call parse_uint
   neg rax                ;make the number negative
   inc rdx                ;inc length
   ret
   
;------------------------------------------------------

string_equals:
   xor rax, rax                ;counter
.go:
   mov dl, byte[rdi+rax]       ;n'st sumbol from first string 
   cmp dl, byte[rsi+rax]       ;compare symbols
   jnz .not_eq
   test dl, dl                 ;end? 
   jz .eq  
   inc rax                     ;inc counter
   jmp .go
.not_eq:
   mov rax, 0                
   ret
.eq:
   mov rax, 1
   ret

;-------------------------------------------------------

string_copy:
 ;  rdi                       ;pointer to a string
  ; rsi                       ;pointer to a buffer
   mov r9, rdx                ;save buffer's length
   call string_length          
   cmp rax, r9                ;compare lengths
   jg .end                    ;if the string is bigger than buffer, then end
   xor r9, r9                 ;counter
.go:
   mov dl, [rdi+r9]           ;n'st symbol
   mov [rsi+r9], dl           ;copy n'st symbol
   dec rax                    ;string's length
   test rax, rax              ;end?
   jz .end2
   inc r9
   jmp .go
.end:
   mov rax, 0                 ;if the string is too big
   ret
.end2:
   mov rax, rsi
   ret

      

















