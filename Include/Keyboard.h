#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>

void init_keyboard();
void handle_keyboard_interrupt(struct regs* r);
char get_last_key();

#endif
