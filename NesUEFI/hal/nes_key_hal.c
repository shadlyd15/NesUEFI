#include <string.h>
#include "nes.h"
#include "nes_key_hal.h"
#include "../emulator/key.h"
#include "../driver/keyboard.h"

int hal_nes_get_key(uint16_t key){
    switch (key)
    {
        case 0: // On / Off
            return 1;
        case 1: // A
            return get_key(37);//K
        case 2: // B
            return get_key(36);;//J
        case 3: // SELECT
            return get_key(22);//U
        case 4: // START
            return get_key(23);//I
        case 5: // UP
            return get_key(17);//W
        case 6: // DOWN
            return get_key(31);//S
        case 7: // LEFT
            return get_key(30);//A
        case 8: // RIGHT
            return get_key(32);//D
        default:
            return 1;
    }
}