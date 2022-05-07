#include <efi.h>
#include <efilib.h>
#include <string.h>
#include <stdbool.h>
#include "kernel.h"
#include "keyboard.h"

EFI_STATUS keyboard_poll(){
	if(!kernel.keyboard_event_handler) return EFI_INVALID_PARAMETER;

	EFI_INPUT_KEY efi_input_key;
	EFI_STATUS rc;
	while(1){
		rc = uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &efi_input_key);
		if(rc == EFI_SUCCESS){
			if(efi_input_key.UnicodeChar){
				kernel.keyboard_event_handler(efi_input_key.UnicodeChar);
			} else{
				kernel.keyboard_event_handler(efi_input_key.ScanCode);
			}
		}
	
		else {
			return rc;
		}
	}
    return rc;
}

EFI_STATUS keyboard_init(void (*handler)(UINT32 key)){
	if(!handler) return EFI_INVALID_PARAMETER;
	kernel.keyboard_event_handler = handler;
	return EFI_SUCCESS;
}
