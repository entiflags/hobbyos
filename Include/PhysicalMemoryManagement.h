#ifndef PHYSICALMEMORYMANAGEMENT_H
#define PHYSICALMEMORYMANAGEMENT_H

constexpr unsigned int PAGE_SIZE = 4096;

void init(unsigned int total_memory, unsigned int kernel_end);
unsigned int allocate_page();
void free_page(unsigned int phys_addr);
unsigned int get_total_memory();
unsigned int get_free_memory();

#endif
