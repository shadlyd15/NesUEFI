/*
This file presents all abstractions needed to port LiteNES.
  (The current working implementation uses allegro library.)

To port this project, replace the following functions by your own:
1) nes_hal_init()
    Do essential initialization work, including starting a FPS HZ timer.

2) nes_set_bg_color(c)
    Set the back ground color to be the NES internal color code c.

3) nes_flush_buf(*buf)
    Flush the entire pixel buf's data to frame buffer.

4) nes_flip_display()
    Fill the screen with previously set background color, and
    display all contents in the frame buffer.

5) wait_for_frame()
    Implement it to make the following code is executed FPS times a second:
        while (1) {
            wait_for_frame();
            do_something();
        }

6) int nes__key_state(int b) 
    Query button b's state (1 to be pressed, otherwise 0).
    The correspondence of b and the buttons:
      0 - Power
      1 - A
      2 - B
      3 - SELECT
      4 - START
      5 - UP
      6 - DOWN
      7 - LEFT
      8 - RIGHT
*/

#include <stdbool.h>
#include <stdint.h>


#include "nes_hal.h"
#include "nes_palette.h"
#include "nes_key_hal.h"
#include "nes_gfx_hal.h"


// fill the screen with background color
void nes_set_bg_color(int c){
    nes_set_bg(nes_color_palette(c));
    // nes_flip_display();
}

// flush pixel buffer to frame buffer
void nes_flush_buf(PixelBuf *buf){

}

void nes_draw_pixel(Pixel *p){ 
    uint8_t color = p->c;
    nes_set_pixel(p->x, p->y, nes_color_palette(p->c));
}

// initialization
void nes_hal_init(){

}

// display and empty the current frame buffer
void nes_flip_display(){
    nes_gfx_swap();
}

// query key-press status
int nes_key_state(int key){
    return hal_nes_get_key(key);
}

