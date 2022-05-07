#ifndef SFS_H
#define SFS_H

#include <efi.h>
#include <efilib.h>


#define MAX_FILE_INFO_SIZE 1024
#define MAX_FILE_NUMBER 1024
#define MAX_FILE_NAME_SIZE 64

EFI_FILE_HANDLE get_volume(EFI_HANDLE image, EFI_DEVICE_PATH * Dp);
EFI_STATUS EFIAPI list_directory(
    IN EFI_FILE_HANDLE dir,
    IN EFI_DEVICE_PATH * dir_device_path,
    void (*cb)(EFI_FILE_INFO * info)
);

UINT64 file_size(EFI_FILE_HANDLE file_handle);

#endif
