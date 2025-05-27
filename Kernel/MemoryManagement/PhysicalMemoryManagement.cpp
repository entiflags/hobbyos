#include "PhysicalMemoryManagement.h"
#include "Utils.h"

static unsigned char* bitmap = nullptr;
static unsigned int bitmap_size = 0;
static unsigned int total_pages = 0;
static unsigned int free_pages = 0;
static unsigned int bitmap_base_addr = 0;

void init(unsigned int total_memory, unsigned int kernel_end)
{
    total_pages = total_memory / PAGE_SIZE;
    free_pages = total_pages;

    bitmap_size = total_pages / 8;
    if (total_pages % 8) bitmap_size++;

    bitmap_base_addr = (kernel_end + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    bitmap = (unsigned char*)bitmap_base_addr;

    for (unsigned int i = 0; i < bitmap_size; i++)
        bitmap[i] = 0;

    unsigned int kernel_pages = (bitmap_base_addr + bitmap_size - 0x100000) / PAGE_SIZE;
    if ((bitmap_base_addr + bitmap_size - 0x100000) % PAGE_SIZE) kernel_pages++;

    for (unsigned int i = 0; i < kernel_pages; i++)
    {
        bitmap[i / 8] |= (1 << (i % 8));
        free_pages--;
    }
}

unsigned int allocate_page()
{
    if (free_pages == 0) return 0;

    for (unsigned int i = 0; i < bitmap_size; i++)
    {
        if (bitmap[i] != 0xFF)
        {
            for (int j = 0; j < 8; j++)
            {
                if (!(bitmap[i] & (1 << j)))
                {
                    // found a free page
                    bitmap[i] |= (1 << j);
                    free_pages--;
                    return (i * 8 + j) * PAGE_SIZE;
                }
            }
        }
    }
    return 0; // no free pages found
}

void free_page(unsigned int phys_addr)
{
    unsigned int page_index = phys_addr / PAGE_SIZE;
    unsigned int byte_index = page_index / 8;
    unsigned int bit_index = page_index % 8;

    if (byte_index < bitmap_size && (bitmap[byte_index] & (1 << bit_index)))
    {
        bitmap[byte_index] &= ~(1 << bit_index);
        free_pages++;
    }
}

unsigned int get_total_memory()
{
    return total_pages * PAGE_SIZE;
}

unsigned int get_free_memory()
{
    return free_pages * PAGE_SIZE;
}
