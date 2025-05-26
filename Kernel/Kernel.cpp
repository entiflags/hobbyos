#include "VGA.h"
#include "GDT.h"

extern "C" void kmain()
{
    gdt::init();
    vga::clear();
    vga::set_color(vga::Color::YELLOW, vga::Color::BLUE);
    vga::print("Welcome to HobbyOS!\n");
    vga::set_color(vga::Color::WHITE, vga::Color::BLACK);
    vga::print("Hello World!\n");
    vga::set_cursor(5, 10);
    vga::set_color(vga::Color::GREEN, vga::Color::BLACK);
    vga::print("Testing cursor position");
}
