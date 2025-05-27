#include "VGA.h"
#include "GDT.h"
#include "IDT.h"
#include "Keyboard.h"

using namespace gdt;
using namespace vga;

extern "C" void kmain()
{
    init_gdt();
    idt_install();
    init_keyboard();
    clear();
    set_color(Color::YELLOW, Color::BLUE);
    print("Welcome to HobbyOS!\n");
    set_color(Color::WHITE, Color::BLACK);
    print("Hello World!\n");
    set_cursor(5, 10);
    set_color(Color::GREEN, Color::BLACK);
    print("Testing cursor position\n\n");

    asm volatile("sti");

    while (1)
    {
        asm volatile("hlt");
    }
}
