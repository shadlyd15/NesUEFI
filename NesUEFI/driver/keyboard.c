#include <efi.h>
#include <efilib.h>
#include <string.h>
#include <stdbool.h>
#include "kernel.h"
#include "keyboard.h"

static char key_state[128],key_pressed[128];

static inline UINT8 inportb(UINT16 port) {
    UINT8 r;
    asm("inb %1, %0" : "=a" (r) : "dN" (port));
    return r;
}

EFI_STATUS keyboard_init(void (*handler)(UINT32 key)){
	if(!handler) return EFI_INVALID_PARAMETER;
	kernel.keyboard_event_handler = handler;
	memset(key_state, 0x00, 128);
	memset(key_pressed, 0x00, 128);
	return EFI_SUCCESS;
}

bool get_key(int scancode) { 
    bool res; 
    res = key_state[scancode] | key_pressed[scancode]; 
    key_pressed[scancode]=0; 
    return res; 
} 

void keyboard_poll() {
    UINT8 scancode = (UINT8) inportb(0x60);
    if (scancode & 0x80){
        scancode &= 0x7f; 
        if(key_state[scancode] == 1){	
        	kernel.keyboard_event_handler(scancode);
        }
        key_state[scancode]=0; 
    }    
    else { 
        key_state[scancode]=1; 
        key_pressed[scancode]=1; 
    }
}
