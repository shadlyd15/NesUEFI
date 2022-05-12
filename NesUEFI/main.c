#include <efi.h>
#include <efilib.h>
#include <efidevp.h>
#include <string.h>

#include "./driver/graphics.h"
#include "./driver/kernel.h"
#include "./driver/timer.h"
#include "./driver/keyboard.h"
#include "./driver/sfs.h"
#include "./hal/nes.h"
#include "./hal/nes_key_hal.h"
#include "./hal/nes_gfx_hal.h"
#include "./emulator/fce.h"
#include "./emulator/ppu.h"
#include "./emulator/cpu.h"
#include "./ui/ui_gfx.h"
#include "./ui/adafruit_gfx.h"


Kernel kernel;

extern EFI_GUID GraphicsOutputProtocol;
EFI_GRAPHICS_OUTPUT_PROTOCOL * gop;

EFI_HANDLE * handles;
UINTN number_of_handles;


EFI_STATUS
efi_main (EFI_HANDLE image_handle, EFI_SYSTEM_TABLE *systab){
	EFI_GUID sfspGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_STATUS rc;

    kernel.image_handle = image_handle;
	InitializeLib(image_handle, systab);

	Print(L"Initializing NesUEFI ...\n");

	rc = timer_init(TICK_PER_SECOND);
	Print(L"Timer Intialized : %r\n", rc);
	ST->BootServices->SetWatchdogTimer(0, 0, 0, NULL);
	rc = graphics_init(&gop);
	if (rc == EFI_SUCCESS){
		Print(L"Graphics Protocol Intialized : %r\n", rc);
		// graphics_print_modes(gop);
		rc = graphics_set_mode(gop);
		if (rc == EFI_SUCCESS){
			Print(L"Graphics Mode Set : %r\n", rc);
			Print(L"\a");
			kernel.graphics = gop;

			adafruit_gfx_init(kernel.graphics->Mode->Info->HorizontalResolution, kernel.graphics->Mode->Info->VerticalResolution);
			list_file_browser();
			keyboard_init(ui_key_handler);
		}
	}

	#if 1
	unsigned long last_frame = 0;
	while(rc == EFI_SUCCESS){
		keyboard_poll();
        ui_state_t state = ui_manage_states();
		const unsigned long now = (unsigned long)timer_ticks();
        if ((now - last_frame) > TICK_PER_SECOND/NES_FPS*1.0){
            last_frame = now;
            if(state == UI_STATE_PLAY){
                nes_gfx_swap();
            }
        }
	}
	#endif

	Print(L"EFI EXIT : %r\n", rc);
	return EFI_SUCCESS;
}


