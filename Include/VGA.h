#ifndef VGA_H
#define VGA_H

#include <cstddef>
#include <cstdint>

namespace vga {
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

void set_color(Color fg, Color bg);
void clear();
void set_cursor(std::size_t row, std::size_t col);
void putc(char c);
void print(const char* str);
}

#endif // VGA_H
