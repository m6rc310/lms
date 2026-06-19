BITS 64
SECTION .text

GLOBAL sysasm.syscall

sysasm.syscall:
  mov rax,[rsp+0x38]
  mov rdi,[rsp+0x30]
  mov rsi,[rsp+0x28]
  mov rdx,[rsp+0x20]
  mov rcx,[rsp+0x18]
  mov r8,[rsp+0x10]
  mov r9,[rsp+0x08]
  syscall
  mov [rsp+0x38],rax
  ret

