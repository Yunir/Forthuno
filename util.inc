section .data
  program_stub: dq 0
  xt_interpreter: dq interpreter_loop
  last_word: dq link
  warning_message: db "Warning: we do not have such word", 0

section .bss
  resq 1023
  rstack_start: resq 1
  input_buf: resb 1024

section .text
; Searches word in dictionary
; rax <= address of found word or zero(0)
find_word:
  xor eax, eax            ; rax - answer: true or false
  pop rdi                 ; rdi - user's word
  mov rsi, [last_word]    ; rsi - pointer to the last word

  .loop:
    push rdi                ; caller-saved
    push rsi                ; caller-saved
    add rsi, link_size      ; rsi - jump over the link to word
    call string_equals      ; rax - find succeed? true(1) : false(0)
    pop rdi                 ; caller-saved
    pop rsi                 ; caller-saved

    test rax, rax
    jnz .found              ; check to successful search
    mov rsi, [rsi]          ; rsi - pointer to the next word
    test rsi, rsi           ; is it the last one?
    jnz .loop

    xor eax, eax
    ret

  .found:
    mov rax, rsi
    ret

; code from address - jump pointer to execution_point place
; rdi - command address
; rax - address of xt_COMMAND
cfa:
	xor eax, eax
  pop rdi
	add rdi, link_size
  push rdi
  call string_length        ; rax - length of string
  pop rdi
  add rax, 1                ; rax - length with null-terminator
  add rdi, rax
	mov rax, rdi
  ret

; The inner interpreter. These three lines
; fetch the next instruction and start its
; execution
next:
  mov w, [pc]
  add pc, 8
  jmp [w]