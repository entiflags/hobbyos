#include "VGA.h"
#include "GDT.h"
#include "IDT.h"
#include "Keyboard.h"
#include "PhysicalMemoryManagement.h"

using namespace gdt;
using namespace vga;

struct multiboot_info
{
    unsigned int mem_lower;
    unsigned int mem_upper;
};

extern "C" void kmain(multiboot_info* mboot_ptr)
{
    init_gdt();
    idt_install();
    init_keyboard();
    clear();
    set_color(Color::YELLOW, Color::BLUE);
    print("Welcome to HobbyOS!\n");
    set_color(Color::WHITE, Color::BLACK);
    print("Hello World!\n");

    unsigned int total_memory = (mboot_ptr->mem_lower + mboot_ptr->mem_upper) * 1024;
    unsigned int kernel_end = 0x200000;
    init(total_memory, kernel_end);

    set_color(Color::CYAN, Color::BLACK);
    print("Total Memory: ");
    print_number(get_total_memory() / 1024); // In KB
    print(" KB\nFree Memory: ");
    print_number(get_free_memory() / 1024); // In KB
    print(" KB\n");


    asm volatile("sti");

    while (1)
    {
        asm volatile("hlt");
    }
}
