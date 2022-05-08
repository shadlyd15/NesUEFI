#ifndef NES_KEY_HAL_H
#define NES_KEY_HAL_H

#include <efi.h>
#include <stdbool.h>

#include "nes.h"

int hal_nes_get_key(uint16_t key);

#endif