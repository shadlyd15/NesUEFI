#ifndef GRAPHICS_H
#define GRAPHICS_H 

#include <efi.h>
#include <efilib.h>

EFI_STATUS graphics_init(EFI_GRAPHICS_OUTPUT_PROTOCOL **gop);
void graphics_print_modes(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);
EFI_STATUS graphics_set_mode(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);
EFI_STATUS graphics_set_pixel(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, UINT32 x, UINT32 y, UINT32 colour);
EFI_STATUS graphics_flush_buffer(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);
EFI_STATUS graphics_draw_bitmap( EFI_GRAPHICS_OUTPUT_PROTOCOL *gop, int x_offset, int y_offset,EFI_HANDLE *bmp_buffer);
void graphics_clear_framebuffer(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);


#endif