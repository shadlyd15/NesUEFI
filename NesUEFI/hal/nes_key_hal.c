#include <string.h>
#include "nes.h"
#include "nes_key_hal.h"
#include "../emulator/key.h"
#include "../driver/keyboard.h"


bool _key_state[10];

void hal_nes_key_clear(){
  memset(_key_state, 0x00, sizeof(_key_state));
}

void hal_nes_ctrl_key_clear(){
  _key_state[NES_KEY_UP] = false;
  _key_state[NES_KEY_LEFT] = false;
  _key_state[NES_KEY_DOWN] = false;
  _key_state[NES_KEY_RIGHT] = false;
  _key_state[NES_KEY_SELECT] = false;
  _key_state[NES_KEY_START] = false;
  // _key_state[NES_KEY_B] = false; // Otherwise Mario will not jump long
  _key_state[NES_KEY_A] = false;
  _key_state[NES_KEY_REBOOT] = false;
}

void hal_nes_set_key(uint32_t key){
  hal_nes_key_clear();
  switch(key){
    case KEY_UP:
      _key_state[NES_KEY_UP] = true;
      break;

    case KEY_LEFT:
      _key_state[NES_KEY_LEFT] = true;
      break;

    case KEY_DOWN:
      _key_state[NES_KEY_DOWN] = true;
      break;

    case KEY_RIGHT:
      _key_state[NES_KEY_RIGHT] = true;
      break;

    case KEY_SELECT:
      _key_state[NES_KEY_SELECT] = true;
      break;

    case KEY_START:
      _key_state[NES_KEY_START] = true;
      break;

    case KEY_B:
      _key_state[NES_KEY_B] = true;
      break;

    case KEY_A:
      _key_state[NES_KEY_A] = true;
      break;

    case KEY_REBOOT:
      _key_state[NES_KEY_REBOOT] = true;
      break;

    default:
      break;
  }
}

bool hal_nes_get_key(uint16_t key){
  if(!key) return true; // Power default on
  return _key_state[key];
}