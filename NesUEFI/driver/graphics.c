#include <efi.h>
#include <efilib.h>
#include <string.h>
#include "graphics.h"
#include "bmp_header.h"

extern EFI_GUID GraphicsOutputProtocol;
#define be32_to_cpu(x) __builtin_bswap32(x)

#if defined(__x86_64__) || defined(__aarch64__)
	UINT64 * _graphics_buffer;
#elif defined(__i386__) || defined(__arm__)
	UINT32 * _graphics_buffer;
#else
#error Arch Not Supported
#endif

EFI_STATUS graphics_init(EFI_GRAPHICS_OUTPUT_PROTOCOL ** gop){
	EFI_STATUS rc;
	EFI_GRAPHICS_OUTPUT_PROTOCOL * protocol;
	Print(L"Initializing GOP ...\n");
	// rc = uefi_call_wrapper(LibLocateProtocol, 2, &GraphicsOutputProtocol, (void **)&protocol);
	rc = LibLocateProtocol(&GraphicsOutputProtocol, (void **)&protocol);
	if (EFI_ERROR(rc)) {
		Print(L"Could not locate GOP: %r\n", rc);
		return rc;
	}

	if (!protocol) {
		Print(L"LocateProtocol(GOP, &gop) returned %r but GOP is NULL\n", rc);
		return EFI_UNSUPPORTED;
	}

	* gop = protocol;
	return EFI_SUCCESS;
}

void graphics_print_modes(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop){
	int i, imax;
	EFI_STATUS rc;

	if (gop->Mode) {
		imax = gop->Mode->MaxMode;
		Print(L"GOP reports MaxMode %d\n", imax);
	} else {
		Print(L"gop->Mode is NULL\n");
		imax = 1;
	}

	for (i = 0; i < imax; i++) {
		EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;
		UINTN SizeOfInfo;
		rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo,
					&info);
		if (EFI_ERROR(rc) && rc == EFI_NOT_STARTED) {
			Print(L"gop->QueryMode() returned %r\n", rc);
			Print(L"Trying to start GOP with SetMode().\n");
			rc = uefi_call_wrapper(gop->SetMode, 2, gop,
				gop->Mode ? gop->Mode->Mode : 0);
			rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i,
				&SizeOfInfo, &info);
		}

		if (EFI_ERROR(rc)) {
			Print(L"%d: Bad response from QueryMode: %r (%d)\n",
			      i, rc, rc);
			continue;
		}
		Print(L"%c%d: %dx%d ",
		      (gop->Mode &&
		       CompareMem(info,gop->Mode->Info,sizeof(*info)) == 0
		       ) ? '*' : ' ',
		      i, info->HorizontalResolution, info->VerticalResolution);
		switch(info->PixelFormat) {
			case PixelRedGreenBlueReserved8BitPerColor:
				Print(L"RGBR");
				break;
			case PixelBlueGreenRedReserved8BitPerColor:
				Print(L"BGRR");
				break;
			case PixelBitMask:
				Print(L"R:%08x G:%08x B:%08x X:%08x",
					info->PixelInformation.RedMask,
					info->PixelInformation.GreenMask,
					info->PixelInformation.BlueMask,
					info->PixelInformation.ReservedMask);
				break;
			case PixelBltOnly:
				Print(L"(blt only)");
				break;
			default:
				Print(L"(Invalid pixel format)");
				break;
		}
		Print(L" pitch %d\n", info->PixelsPerScanLine);
	}
}

EFI_STATUS graphics_set_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop){
	int i, imax;
	EFI_STATUS rc;
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *info;

	Print(L"Setting Graphics Mode ...\n");

	if (gop->Mode) {
		imax = gop->Mode->MaxMode;
	} else {
		Print(L"gop->Mode is NULL\n");
		return EFI_UNSUPPORTED;
	}

	for (i = 0; i < imax; i++) {
		UINTN SizeOfInfo;
		rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i, &SizeOfInfo,
					&info);
		if (EFI_ERROR(rc) && rc == EFI_NOT_STARTED) {
			Print(L"gop->QueryMode() returned %r\n", rc);
			Print(L"Trying to start GOP with SetMode().\n");
			rc = uefi_call_wrapper(gop->SetMode, 2, gop,
				gop->Mode ? gop->Mode->Mode : 0);
			rc = uefi_call_wrapper(gop->QueryMode, 4, gop, i,
				&SizeOfInfo, &info);
		}

		if (EFI_ERROR(rc)) {
			Print(L"%d: Bad response from QueryMode: %r (%d)\n",
			      i, rc, rc);
			continue;
		}

		if (CompareMem(info, gop->Mode->Info, sizeof (*info)))
			continue;

		UINT32 pixels = info->VerticalResolution * info->PixelsPerScanLine;

		#if defined(__x86_64__) || defined(__aarch64__)
			UINT32 graphics_buffer_size = pixels * sizeof(UINT64);
		#elif defined(__i386__) || defined(__arm__)
			UINT32 graphics_buffer_size = pixels * sizeof(UINT32);
		#else
		#error Arch Not Supported
		#endif

		
		Print(L"Vertical Resolution : %ld\n", info->VerticalResolution);
		Print(L"Horizontal Resolution : %ld\n", info->HorizontalResolution);
		Print(L"Pixel Format : %ld\n", info->PixelFormat);
		Print(L"Pixels PerScanLine : %ld\n", info->PixelsPerScanLine);
		return EFI_SUCCESS;
	}
	Print(L"Never found the active video mode?\n");
	return EFI_UNSUPPORTED;
}

EFI_STATUS graphics_set_pixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 x, UINT32 y, UINT32 colour){
	EFI_STATUS rc;
	UINT32 PixelsPerScanLine = gop->Mode->Info->PixelsPerScanLine;
	#if defined(__x86_64__) || defined(__aarch64__)
    	*(UINT64*)((UINT64)gop->Mode->FrameBufferBase + (x*4) + (y * PixelsPerScanLine * 4)) = colour;
#elif defined(__i386__) || defined(__arm__)
	   	*(UINT32*)((UINT32)gop->Mode->FrameBufferBase + (x*4) + (y * PixelsPerScanLine * 4)) = colour;
	#else
	#error Arch Not Supported
	#endif
    rc = EFI_SUCCESS;

	return rc;
}


void graphics_clear_framebuffer(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop){
	if(gop){
		memset((VOID *)gop->Mode->FrameBufferBase, 0x00, gop->Mode->FrameBufferSize);
	}
}

EFI_STATUS graphics_draw_bitmap( EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, 
					  int x_offset,
					  int y_offset,
		              EFI_HANDLE *bmp_buffer){
    BMP_IMAGE_HEADER *BmpHeader = (BMP_IMAGE_HEADER *) bmp_buffer;
    EFI_STATUS rc = EFI_SUCCESS;
    UINT8  *bitmap_data;

    bitmap_data = (UINT8*)bmp_buffer + BmpHeader->ImageOffset;

    for (unsigned int y = 0; y < BmpHeader->PixelHeight; ++y){
        for (unsigned int x = 0; x < BmpHeader->PixelWidth; ++x){
        	unsigned int* pixPtr = (unsigned int *)bitmap_data + (BmpHeader->PixelHeight - 1 - y) * BmpHeader->PixelWidth + x;
        	graphics_set_pixel(gop, x_offset + x, y_offset + y, (UINT32)*pixPtr);
        }
    }

    return rc;
}
 


