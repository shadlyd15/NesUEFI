#ifndef NES_GFX_HAL_H
#define NES_GFX_HAL_H

#include "nes.h"
#include "../driver/kernel.h"

void nes_set_pixel(int x, int y, UINT32 nes_colour);
void nes_set_bg(int colour);
void nes_gfx_swap();

#endif
