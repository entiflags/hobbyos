#include "IRQ.h"
#include "Utils.h"
#include "VGA.h"
#include "IO.h"
#include "Keyboard.h"

using namespace vga;
using namespace IO;

typedef void (*irq_handler_t)(struct regs*);
static irq_handler_t handlers[16] = {0};

void install_handler(int irq, irq_handler_t handler)
{
    handlers[irq] = handler;
}

extern "C" void pic_remap()
{
    unsigned char master_mask = inb(0x21);
    unsigned char slave_mask = inb(0xA1);

    outb(0x20, 0x11);   // ICW1: initialize master PIC
    outb(0xA0, 0x11);   // ICW1: initialize slave PIC
    outb(0x21, 0x20);   // ICW2: master PIC vector offset
    outb(0xA1, 0x28);   // ICW2: slave PIC vector offset
    outb(0x21, 0x04);   // ICW3: tell master PIC there is a slave at IRQ2
    outb(0xA1, 0x02);   // ICW3: tell slave PIC its cascade identity
    outb(0x21, 0x01);   // ICW4: enable 8086 mode for master
    outb(0xA1, 0x01);   // ICW4: enable 8086 mode for slave

    outb(0x21, master_mask);
    outb(0xA1, slave_mask);
}

extern "C" void irq_install()
{
    pic_remap();

    unsigned short sel = 0x08;
    unsigned char flags = 0x8E;

    idt_set_gate(32, (unsigned long)irq0, sel, flags);
    idt_set_gate(33, (unsigned long)irq1, sel, flags);
    idt_set_gate(34, (unsigned long)irq2, sel, flags);
    idt_set_gate(35, (unsigned long)irq3, sel, flags);
    idt_set_gate(36, (unsigned long)irq4, sel, flags);
    idt_set_gate(37, (unsigned long)irq5, sel, flags);
    idt_set_gate(38, (unsigned long)irq6, sel, flags);
    idt_set_gate(39, (unsigned long)irq7, sel, flags);
    idt_set_gate(40, (unsigned long)irq8, sel, flags);
    idt_set_gate(41, (unsigned long)irq9, sel, flags);
    idt_set_gate(42, (unsigned long)irq10, sel, flags);
    idt_set_gate(43, (unsigned long)irq11, sel, flags);
    idt_set_gate(44, (unsigned long)irq12, sel, flags);
    idt_set_gate(45, (unsigned long)irq13, sel, flags);
    idt_set_gate(46, (unsigned long)irq14, sel, flags);
    idt_set_gate(47, (unsigned long)irq15, sel, flags);

    install_handler(1, handle_keyboard_interrupt);

    asm volatile("sti");
}

extern "C" void irq_handler(struct regs *r)
{
    int irq = r->int_no - 32;

    if (irq < 16 && handlers[irq] != 0)
    {
        handlers[irq](r);
    }
    else
    {
        /*set_color(Color::CYAN, Color::BLACK);
        print("IRQ Received!\n");

        set_color(Color::WHITE, Color::BLACK);
        print("IRQ Number: ");
        char num_str[4];
        itoa(r->int_no - 32, num_str, 10);
        print(num_str);
        print("\n");*/

        if (r->int_no >= 40)
            outb(0xA0, 0x20);
        outb(0x20, 0x20);

        /*set_color(Color::GREEN, Color::BLACK);
        print("Interrupt Handled.\n");*/
    }
}
