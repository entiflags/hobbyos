MB_MAGIC equ 0x1BADB002
MB_FLAGS equ (1<<0)|(1<<1)
MB_CHSUM equ -(MB_MAGIC+MB_FLAGS)

section .multiboot
align 4
dd MB_MAGIC
dd MB_FLAGS
dd MB_CHSUM

section .text
extern kmain
global _start

_start:
    mov esp, stackTop
    push eax
    push ebx
    call kmain
    cli
.loop:
    hlt
    jmp .loop

section .bss
align 16
stackBottom:
resb 4096
stackTop:
