#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <efi.h>
#include <stdbool.h>

bool get_key(int scancode);
void keyboard_poll();
EFI_STATUS keyboard_init(void (*handler)(UINT32 key));

#endif