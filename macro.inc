%define pc r15
%define w r14
%define rstack r13
%define stack r12
;%define here r11         ; pointer to free block of forth memory
%define link 0

%assign link_size 8
%assign word_size 8

%macro create_link 0
  %%link: dq link        ; %% creates new local (example: @1234.link)
  %define link %%link
%endmacro

%macro native 3
  section .data
    w_ %+ %2:            ; w_COMMAND label
	    create_link        ; create link for dictionary search
	    db %1, 0           ; put name of COMMAND with null-terminator
      db %3              ; Immediate flag (1) ; not Immediate (0, 2)
	  xt_ %+ %2:           ; xt_COMMAND label
      dq %2 %+ _impl     ; COMMAND_impl address
  section .text
	 %2 %+ _impl:           ; COMMAND_impl label
%endmacro

%macro colon 3
  section .data
	 w_ %+ %2:
	  create_link
	  db %1, 0
    db %3                ; Immediate flag
	 xt_ %+ %2:
	  dq docol_impl             ; The `docol` address −− one level of indirection
%endmacro

%macro native 2
  native %1, %2, 0
%endmacro

%macro colon 2
  colon %1, %2, 0
%endmacro

%macro branchif0 1
  dq xt_branchifz
  dq %1
%endmacro

%macro branch 1
  dq xt_branch
  dq %1
%endmacro

%macro rpush 1
  sub rstack, 8
  mov qword [rstack], %1
%endmacro

%macro rpop 1
  mov %1, qword [rstack]
  add rstack, 8
%endmacro

%macro const 2
%defstr %%__cnst_str %1
native %%__cnst_str, %1
    push qword %2
    jmp next
%endmacro
