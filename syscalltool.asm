BITS 64
SECTION .text

EXTERN main

GLOBAL syscall
syscall:
  mov rax,rdi
  mov rdi,rsi
  mov rsi,rdx
  mov rdx,rcx
  syscall
  ret

GLOBAL _start
_start:
  mov rdi,[rsp]				; argc
  lea rsi,[rsp+8]			; argv
  lea rax,[rsi+rdi*8+8]		; environment
  push rax
  push rsi
  push rdi
  call main
  mov rdi,0
  mov rax,60
  syscall

