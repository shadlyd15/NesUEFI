#include "./driver/timer.h"
#include "./driver/kernel.h"
#include "./driver/bmp_header.h"
#include "./driver/keyboard.h"
#include "./driver/graphics.h"
#include "./driver/sfs.h"
#include "./hal/nes.h"
#include "./hal/nes_key_hal.h"
#include "./emulator/fce.h"
#include "./emulator/ppu.h"
#include "./emulator/cpu.h"
#include "adafruit_gfx.h"
#include "ui_gfx.h"

CHAR16 rom_collection[MAX_FILE_NUMBER][MAX_FILE_NAME_SIZE];

ui_state_t g_ui_state = UI_STATE_SPLASH;

static int total_roms = 0;
static int current_rom_index = 0;

CHAR16* strstr_16(CHAR16 *string, CHAR16 *substring);

void draw_menu(CHAR16 *item_name){
    int x_offset = (kernel.graphics->Mode->Info->HorizontalResolution - 601) / 2;
    int y_offset = (kernel.graphics->Mode->Info->VerticalResolution - 573) / 2 + 495;
    fillRect(x_offset, y_offset, 573, 20, 0x0f4106);
    setCursor(x_offset + 30, y_offset + 6);
    setTextColor(0xFFFFFFFF);
    setTextSize(1);
    write(17);
    print("  ");
    print_16(item_name);
    print("  ");
    write(16);
}

bool load_current_nes_file(){
    bool rc = false;;
    EFI_DEVICE_PATH * dp;
    EFI_FILE_HANDLE root = get_volume(kernel.image_handle, (void*) &dp);

    EFI_FILE_HANDLE FileHandle;

    uefi_call_wrapper(root->Open, 5, root, &FileHandle, rom_collection[current_rom_index], EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

    UINT64 ReadSize = file_size(FileHandle);
    EFI_HANDLE *Buffer = AllocatePool(ReadSize);

    uefi_call_wrapper(FileHandle->Read, 3, FileHandle, &ReadSize, Buffer);

    if (fce_load_rom((char*) Buffer) == 0){
        // Print(L"ROM LOADED\n\r");
        draw_menu(L"ROM LOADED");
        rc = true;
    }
    else{
        draw_menu(L"ROM LOAD ERROR");
    }
    FreePool(Buffer);
    return rc;
}

void render_menu(UINT16 key){
    switch (key){
        case KEY_UP:
        case KEY_RIGHT:{
            current_rom_index++;
            current_rom_index = current_rom_index % total_roms;
            break;
        }

        case KEY_DOWN:
        case KEY_LEFT:{
            current_rom_index--;
            if (current_rom_index < 0)
            {
                current_rom_index = total_roms - 1;
            }
            break;
        }

        case KEY_START:{
            g_ui_state = UI_STATE_LOAD_ROM;
            break;
        }

        default:
            break;
    }
    draw_menu(rom_collection[current_rom_index]);
}

void save_rom_info(EFI_FILE_INFO *info){
    if ((strstr_16(info->FileName, L".nes")) || (strstr_16(info->FileName, L".NES"))){
        StrCpy(rom_collection[total_roms], info->FileName);
        total_roms++;
    }
}

int list_file_browser(){
    EFI_DEVICE_PATH * dp;
    EFI_FILE_HANDLE root = get_volume(kernel.image_handle, (void*) &dp);
    list_directory(root, dp, save_rom_info);
    return total_roms;
}

ui_state_t ui_manage_states(){
    switch (g_ui_state){
        case UI_STATE_SPLASH:{
            render_splash_screen();
            g_ui_state = UI_STATE_MENU;
            break;
        }

        case UI_STATE_MENU:{
            break;
        }

        case UI_STATE_LOAD_ROM:{
            if (load_current_nes_file()){
                fce_init();
                graphics_clear_framebuffer(kernel.graphics);
                g_ui_state = UI_STATE_PLAY;
            }
            else{
                g_ui_state = UI_STATE_MENU;
            }
            break;
        }

        case UI_STATE_PLAY:{

            fce_run();
            break;
        }
    }
    return g_ui_state;
}

void render_splash_screen(){

    EFI_DEVICE_PATH * dp;
    EFI_FILE_HANDLE root = get_volume(kernel.image_handle, (void*) &dp);

    CHAR16 *FileName = L"splash.bmp";
    EFI_FILE_HANDLE FileHandle;
    graphics_clear_framebuffer(kernel.graphics);
    /*open the file */
    uefi_call_wrapper(root->Open, 5, root, &FileHandle, FileName, EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY | EFI_FILE_HIDDEN | EFI_FILE_SYSTEM);

    /*read from the file */
    UINT64 ReadSize = file_size(FileHandle);
    EFI_HANDLE *Buffer = AllocatePool(ReadSize);

    uefi_call_wrapper(FileHandle->Read, 3, FileHandle, &ReadSize, Buffer);
    BMP_IMAGE_HEADER *bmp_header = (BMP_IMAGE_HEADER*) Buffer;
    int x_offset = (kernel.graphics->Mode->Info->HorizontalResolution - bmp_header->PixelHeight) / 2;
    int y_offset = (kernel.graphics->Mode->Info->VerticalResolution - bmp_header->PixelWidth) / 2;

    graphics_draw_bitmap(kernel.graphics, x_offset, y_offset, (void **) Buffer);
    FreePool(Buffer);
}

void ui_key_handler(uint32_t key){
    if(g_ui_state == UI_STATE_MENU){
        render_menu(key);
    }
    else if(g_ui_state == UI_STATE_PLAY){
        if(key == KEY_REBOOT){
            g_ui_state = UI_STATE_SPLASH;
        }
    }
}

CHAR16* strstr_16(CHAR16 *string, CHAR16 *substring){
    // https://opensource.apple.com/source/tcl/tcl-10/tcl/compat/strstr.c.auto.html
    // Modified for CHAR16
    /*First scan quickly through the two strings looking for a
     *single-character match.  When it's found, then compare the
     *rest of the substring.
     */
    CHAR16 *a, *b;
    b = substring;
    if (*b == 0){
        return string;
    }
    for (; *string != 0; string += 1){
        if (*string != *b){
            continue;
        }
        a = string;
        while (1){
            if (*b == 0){
                return string;
            }
            if (*a++ != *b++){
                break;
            }
        }
        b = substring;
    }
    return NULL;
}