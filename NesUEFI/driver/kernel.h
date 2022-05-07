#ifndef BOOT_H
#define BOOT_H

#include <efi.h>
#include <efilib.h>

typedef struct {
    EFI_HANDLE image_handle;
    EFI_SYSTEM_TABLE *systab;

	EFI_GRAPHICS_OUTPUT_PROTOCOL * graphics;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL * fs;

	void (*keyboard_event_handler)(UINT32 key);
	unsigned long long ticks;
} Kernel;

extern Kernel kernel;
#endif