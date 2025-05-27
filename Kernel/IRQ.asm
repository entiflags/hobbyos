section .text
extern irq_handler

%macro IRQ 1
global irq%1
irq%1:
    cli
    push byte 0
    push byte %1 + 32
    jmp irq_common
%endmacro

irq_common:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    call irq_handler
    pop esp
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

IRQ 0
IRQ 1
IRQ 2
IRQ 3
IRQ 4
IRQ 5
IRQ 6
IRQ 7
IRQ 8
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15
