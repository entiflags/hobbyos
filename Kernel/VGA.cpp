#include "VGA.h"
#include "IO.h"

using namespace std;
using namespace IO;

namespace vga {
constexpr uintptr_t BUFFER = 0xb8000;  // vga buffer address
constexpr size_t WIDTH = 80;           // screen width (columns)
constexpr size_t HEIGHT = 25;          // screen height (rows)

namespace {
size_t term_col = 0;                   // current column position
size_t term_row = 0;                   // current row position
Color fg_color = Color::WHITE;              // foreground color
Color bg_color = Color::BLACK;              // background color

// scroll the screen by moving rows up and clearing the last row
void Scroll()
{
    auto* video_mem = static_cast<uint16_t*>(const_cast<void*>(reinterpret_cast<const void*>(BUFFER)));
    // copy rows 1 to HEIGHT-1 into rows 0 to HEIGHT-2
    for (size_t r = 0; r < HEIGHT - 1; ++r)
    {
        for (size_t c = 0; c < WIDTH; ++c)
        {
            video_mem[r * WIDTH + c] = video_mem[(r + 1) * WIDTH + c];
        }
    }
    // clear the last row
    for (size_t c = 0; c < WIDTH; ++c) {
        video_mem[(HEIGHT - 1) * WIDTH + c] = (static_cast<uint16_t>(bg_color) << 12) |
                                              (static_cast<uint16_t>(fg_color) << 8) | ' ';
    }
}

// update hardware cursor position
void UpdateCursor()
{
    uint16_t pos = term_row * WIDTH + term_col;

    // send low byte
    outb(0x3D4, 0x0F);
    outb(0x3D5, static_cast<uint8_t>(pos & 0xFF));
    // send high byte
    outb(0x3D4, 0x0E);
    outb(0x3D5, static_cast<uint8_t>((pos >> 8) & 0xFF));
}
} // anonymous namespace

void set_color(Color fg, Color bg)
{
    fg_color = fg;
    bg_color = bg;
}

void clear()
{
    auto* video_mem = static_cast<uint16_t*>(const_cast<void*>(reinterpret_cast<const void*>(BUFFER)));
    for (size_t r = 0; r < HEIGHT; ++r)
    {
        for (size_t c = 0; c < WIDTH; ++c)
        {
            video_mem[r * WIDTH + c] = (static_cast<uint16_t>(bg_color) << 12) |
                                       (static_cast<uint16_t>(fg_color) << 8) | ' ';
        }
    }
    term_row = 0;
    term_col = 0;
    UpdateCursor();
}

void set_cursor(size_t row, size_t col)
{
    if (row < HEIGHT && col < WIDTH)
    {
        term_row = row;
        term_col = col;
        UpdateCursor();
    }
}

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
        auto* location = static_cast<uint16_t*>(
            const_cast<void*>(
                reinterpret_cast<const void*>(
                    BUFFER + (term_row * WIDTH + term_col) * 2
                    )
                )
            );
        *location = (static_cast<uint16_t>(bg_color) << 12) |
                    (static_cast<uint16_t>(fg_color) << 8) | c;
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

void print(const char* str)
{
    while (*str)
        putc(*str++);
}

void print_number(unsigned int num)
{
	if (num == 0)
	{
		print("0");
		return;
	}

	char buffer[32];
	int i = 0;

	while (num > 0)
	{
		buffer[i++] = '0' + (num % 10);
		num /= 10;
	}

	char final_buffer[32];
	int j = 0;
	for (int k = i - 1; k >= 0; k--)
	{
		final_buffer[j++] = buffer[k];
	}
	final_buffer[j] = '\0';

	print(final_buffer);
}
} // namespace vga
