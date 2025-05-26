#ifndef GDT_H
#define GDT_H

#include <cstdint>

namespace gdt {

struct GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed));

struct GDTPointer {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void init();
void set_gate(int32_t num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

} // namespace gdt

#endif // GDT_H
