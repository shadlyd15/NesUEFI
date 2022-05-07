#include "key.h"
#include "../hal/nes_key_hal.h"

#include <stdbool.h>


static bool nes_keys[9];
// nes_keys[0] = true;  // Power Button

void nes_set_key(int key){

	nes_keys[key] = true;
}

int nes_get_key(int key){
    return hal_nes_get_key(key);
}
