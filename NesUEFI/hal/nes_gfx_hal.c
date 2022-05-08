#include "nes_gfx_hal.h"
#include "../driver/graphics.h"
#include "../ui/adafruit_gfx.h"
#include <string.h>

// UINT32 _nes_screen_buffer_prev[(NES_SCREEN_WIDTH) * (NES_SCREEN_HEIGHT) + 1];
UINT32 _nes_screen_buffer_current[(NES_SCREEN_WIDTH) * (NES_SCREEN_HEIGHT) + 1];


void nes_set_pixel(int x, int y, UINT32 nes_colour){
	if(x>-1){
		_nes_screen_buffer_current[x + y * NES_SCREEN_WIDTH] = nes_colour;
	}
}

void nes_set_bg(int colour){
	for (int i = 0; i < NES_SCREEN_WIDTH*NES_SCREEN_HEIGHT; ++i){
		nes_set_pixel(i%NES_SCREEN_WIDTH, i / NES_SCREEN_WIDTH , colour);
	}
}

void nes_gfx_swap(){
	for (int i = 0; i < NES_SCREEN_WIDTH*NES_SCREEN_HEIGHT; ++i){
		// if((_nes_screen_buffer_current[i] != _nes_screen_buffer_prev[i]) ){
			int x_offset = (kernel.graphics->Mode->Info->HorizontalResolution - NES_SCREEN_WIDTH*NES_SCREEN_ZOOM)/2 +1;
			int y_offset = (kernel.graphics->Mode->Info->VerticalResolution - NES_SCREEN_HEIGHT*NES_SCREEN_ZOOM)/2;
			// _nes_screen_buffer_prev[i] = _nes_screen_buffer_current[i];
	   		fillRect( 	x_offset + (i%NES_SCREEN_WIDTH)*NES_SCREEN_ZOOM, 
	   					y_offset + (i/NES_SCREEN_WIDTH)*NES_SCREEN_ZOOM, 
	   					NES_SCREEN_ZOOM, NES_SCREEN_ZOOM, _nes_screen_buffer_current[i]);
	   	// }
	}
}