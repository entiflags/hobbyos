#include <cstddef>
#include <cstdint>
#include "IO.h"

namespace vga {
constexpr std::uintptr_t BUFFER = 0xb8000;  // vga buffer address
constexpr std::size_t WIDTH = 80;           // screen width (columns)
constexpr std::size_t HEIGHT = 25;          // screen height (rows)

// Color definitions
enum class Color : std::uint8_t {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    YELLOW = 14,
    WHITE = 15
};

namespace {
std::size_t term_col = 0;                   // current column position
std::size_t term_row = 0;                   // current row position
Color fg_color = Color::WHITE;              // foreground color
Color bg_color = Color::BLACK;              // background color

// scroll the screen by moving rows up and clearing the last row
void Scroll()
{
    auto* video_mem = static_cast<std::uint16_t*>(const_cast<void*>(reinterpret_cast<const void*>(BUFFER)));
    // copy rows 1 to HEIGHT-1 into rows 0 to HEIGHT-2
    for (std::size_t r = 0; r < HEIGHT - 1; ++r)
    {
        for (std::size_t c = 0; c < WIDTH; ++c)
        {
            video_mem[r * WIDTH + c] = video_mem[(r + 1) * WIDTH + c];
        }
    }
    // clear the last row
    for (std::size_t c = 0; c < WIDTH; ++c) {
        video_mem[(HEIGHT - 1) * WIDTH + c] = (static_cast<std::uint16_t>(bg_color) << 12) |
                                              (static_cast<std::uint16_t>(fg_color) << 8) | ' ';
    }
}

// update hardware cursor position
void UpdateCursor()
{
    std::uint16_t pos = term_row * WIDTH + term_col;

    // send low byte
    IO::outb(0x3D4, 0x0F);
    IO::outb(0x3D5, static_cast<std::uint8_t>(pos & 0xFF));
    // send high byte
    IO::outb(0x3D4, 0x0E);
    IO::outb(0x3D5, static_cast<std::uint8_t>((pos >> 8) & 0xFF));
}

} // anonymous namespace

// set text color
void set_color(Color fg, Color bg)
{
    fg_color = fg;
    bg_color = bg;
}

// clear the screen
void clear()
{
    auto* video_mem = static_cast<std::uint16_t*>(const_cast<void*>(reinterpret_cast<const void*>(BUFFER)));
    for (std::size_t r = 0; r < HEIGHT; ++r)
    {
        for (std::size_t c = 0; c < WIDTH; ++c)
        {
            video_mem[r * WIDTH + c] = (static_cast<std::uint16_t>(bg_color) << 12) |
                                       (static_cast<std::uint16_t>(fg_color) << 8) | ' ';
        }
    }
    term_row = 0;
    term_col = 0;
    UpdateCursor();
}

// set cursor position
void set_cursor(std::size_t row, std::size_t col)
{
    if (row < HEIGHT && col < WIDTH)
    {
        term_row = row;
        term_col = col;
        UpdateCursor();
    }
}

// output a single char to the vga buffer
void putc(char c)
{
    if (c == '\n')
    {
        term_col = 0;
        ++term_row;
        if (term_row == HEIGHT) {
            Scroll();
            term_row = HEIGHT - 1;
        }
    } else {
        auto* location = static_cast<std::uint16_t*>(
            const_cast<void*>(
                reinterpret_cast<const void*>(
                    BUFFER + (term_row * WIDTH + term_col) * 2
                    )
                )
            );
        *location = (static_cast<std::uint16_t>(bg_color) << 12) |
                    (static_cast<std::uint16_t>(fg_color) << 8) | c;
        ++term_col;
        if (term_col == WIDTH)
        {
            term_col = 0;
            ++term_row;
            if (term_row == HEIGHT) {
                Scroll();
                term_row = HEIGHT - 1;
            }
        }
    }
    UpdateCursor();
}

// output a string to the vga buffer
void print(const char* str)
{
    while (*str)
        putc(*str++);
}
} // namespace vga

// kernel entry point
extern "C" void kmain()
{
    vga::clear();
    vga::set_color(vga::Color::YELLOW, vga::Color::BLUE);
    vga::print("Welcome to HobbyOS!\n");
    vga::set_color(vga::Color::WHITE, vga::Color::BLACK);
    vga::print("Hello World!\n");
    vga::set_cursor(5, 10);
    vga::set_color(vga::Color::GREEN, vga::Color::BLACK);
    vga::print("Testing cursor position");
}
