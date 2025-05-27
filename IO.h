#ifndef IO_H
#define IO_H

#include <cstdint>

namespace IO {
// write a byte to the specified port
inline void outb(std::uint16_t port, std::uint8_t value)
{
	asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

// read a byte from the specified port
inline std::uint8_t inb(std::uint16_t port)
{
	std::uint8_t value;
	asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
	return value;
}
}

#endif // IO_H
