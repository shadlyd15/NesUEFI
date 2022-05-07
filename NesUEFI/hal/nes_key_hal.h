#ifndef NES_KEY_HAL_H
#define NES_KEY_HAL_H

#include <efi.h>
#include <stdbool.h>

#include "nes.h"

typedef enum nes_key_t {
	NES_KEY_POWER,
	NES_KEY_A,
	NES_KEY_B,
	NES_KEY_SELECT,
	NES_KEY_START,
	NES_KEY_UP,
	NES_KEY_DOWN,
	NES_KEY_LEFT,
	NES_KEY_RIGHT,
	NES_KEY_REBOOT
} nes_key_t;

void hal_nes_key_clear();
void hal_nes_set_key(uint32_t key);
bool hal_nes_get_key(uint16_t key);
void hal_nes_ctrl_key_clear();

#endif