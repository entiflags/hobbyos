#include "IDT.h"
#include "IRQ.h"
#include "VGA.h"
#include "Utils.h"

using namespace vga;

struct idt_entry idt[256];
struct idt_ptr idtp;

extern "C" void idt_load();

extern "C" void isr0();
extern "C" void isr1();
extern "C" void isr2();
extern "C" void isr3();
extern "C" void isr4();
extern "C" void isr5();
extern "C" void isr6();
extern "C" void isr7();
extern "C" void isr8();
extern "C" void isr9();
extern "C" void isr10();
extern "C" void isr11();
extern "C" void isr12();
extern "C" void isr13();
extern "C" void isr14();
extern "C" void isr15();
extern "C" void isr16();
extern "C" void isr17();
extern "C" void isr18();
extern "C" void isr19();
extern "C" void isr20();
extern "C" void isr21();
extern "C" void isr22();
extern "C" void isr23();
extern "C" void isr24();
extern "C" void isr25();
extern "C" void isr26();
extern "C" void isr27();
extern "C" void isr28();
extern "C" void isr29();
extern "C" void isr30();
extern "C" void isr31();

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags)
{
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install()
{
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (unsigned int)&idt;
    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    isr_install();
    irq_install();
    idt_load();
}

void isr_install()
{
    unsigned short sel = 0x08;
    unsigned char flags = 0x8E;

    idt_set_gate(0, (unsigned long)isr0, sel, flags);
    idt_set_gate(1, (unsigned long)isr1, sel, flags);
    idt_set_gate(2, (unsigned long)isr2, sel, flags);
    idt_set_gate(3, (unsigned long)isr3, sel, flags);
    idt_set_gate(4, (unsigned long)isr4, sel, flags);
    idt_set_gate(5, (unsigned long)isr5, sel, flags);
    idt_set_gate(6, (unsigned long)isr6, sel, flags);
    idt_set_gate(7, (unsigned long)isr7, sel, flags);
    idt_set_gate(8, (unsigned long)isr8, sel, flags);
    idt_set_gate(9, (unsigned long)isr9, sel, flags);
    idt_set_gate(10, (unsigned long)isr10, sel, flags);
    idt_set_gate(11, (unsigned long)isr11, sel, flags);
    idt_set_gate(12, (unsigned long)isr12, sel, flags);
    idt_set_gate(13, (unsigned long)isr13, sel, flags);
    idt_set_gate(14, (unsigned long)isr14, sel, flags);
    idt_set_gate(15, (unsigned long)isr15, sel, flags);
    idt_set_gate(16, (unsigned long)isr16, sel, flags);
    idt_set_gate(17, (unsigned long)isr17, sel, flags);
    idt_set_gate(18, (unsigned long)isr18, sel, flags);
    idt_set_gate(19, (unsigned long)isr19, sel, flags);
    idt_set_gate(20, (unsigned long)isr20, sel, flags);
    idt_set_gate(21, (unsigned long)isr21, sel, flags);
    idt_set_gate(22, (unsigned long)isr22, sel, flags);
    idt_set_gate(23, (unsigned long)isr23, sel, flags);
    idt_set_gate(24, (unsigned long)isr24, sel, flags);
    idt_set_gate(25, (unsigned long)isr25, sel, flags);
    idt_set_gate(26, (unsigned long)isr26, sel, flags);
    idt_set_gate(27, (unsigned long)isr27, sel, flags);
    idt_set_gate(28, (unsigned long)isr28, sel, flags);
    idt_set_gate(29, (unsigned long)isr29, sel, flags);
    idt_set_gate(30, (unsigned long)isr30, sel, flags);
    idt_set_gate(31, (unsigned long)isr31, sel, flags);
}

void isr_handler(struct regs *r)
{
    clear();
    set_color(Color::RED, Color::BLACK);
    print("Interrupt Occured!\n");

    const char* interrupt_desc[] = {
        "Divide by Zero",
        "Debug",
        "Non-Maskable Interrupt",
        "Breakpoint",
        "Overflow",
        "Bound Range Exceeded",
        "Invalid Opcode",
        "Device Not Available",
        "Double Fault",
        "Coprocessor Segment Overrun",
        "Invalid TSS",
        "Segment Not Present",
        "Stack-Segment Fault",
        "General Protection Fault",
        "Page Fault",
        "Reserved",
        "x87 FPU Error",
        "Alignment Check",
        "Machine Check",
        "SIMD Floating-Point Exception",
        "Virtualization Exception",
        "Control Protection Exception",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Hypervisor Injection Exception",
        "VMM Communication Exception",
        "Security Exception",
        "Reserved"
    };

    set_color(Color::WHITE, Color::BLACK);
    print("Interrupt Number: ");
    char num_str[4];
    itoa(r->int_no, num_str, 10);
    print(num_str);
    print("\n");

    if (r->int_no < 32)
    {
        print("Description: ");
        print(interrupt_desc[r->int_no]);
        print("\n");
    }

    if (r->err_code != 0)
    {
        set_color(Color::YELLOW, Color::BLACK);
        print("Error Code: ");
        char err_str[16];
        itoa(r->err_code, err_str, 16);
        print("0x");
        print(err_str);
        print("\n");
    }

    set_color(Color::RED, Color::BLACK);
    print("System Halted.\n");
    for (;;)
        asm("hlt");
}
