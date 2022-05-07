#include <efi.h>
#include <efilib.h>
#include "sfs.h"

extern EFI_GUID LoadedImageProtocol;

UINT64 file_size(EFI_FILE_HANDLE file_handle){
  UINT64 ret;
  EFI_FILE_INFO * file_info;
  file_info = LibFileInfo(file_handle);
  ret = file_info->FileSize;
  FreePool(file_info);
  return ret;
}

EFI_STATUS EFIAPI list_directory(
    IN EFI_FILE_HANDLE dir,
    IN EFI_DEVICE_PATH * dir_device_path,
    void (*cb)(EFI_FILE_INFO * info)
) {
    EFI_STATUS rc;
    EFI_FILE_INFO * file_info;
    CHAR16 * file_name;
    UINTN file_infoSize;
    EFI_DEVICE_PATH * dp;

    file_info = AllocatePool(MAX_FILE_INFO_SIZE);
    if (file_info == NULL) {
        return EFI_OUT_OF_RESOURCES;
    }

    for (;;) {
        file_infoSize = MAX_FILE_INFO_SIZE;
        rc = uefi_call_wrapper(dir -> Read, 3, dir, & file_infoSize, (VOID * ) file_info);
        if (EFI_ERROR(rc) || file_infoSize == 0) {
            if (rc == EFI_BUFFER_TOO_SMALL) {
                // Print(L"EFI_FILE_INFO > MAX_FILE_INFO_SIZE. Increase the size\n");
            }
            FreePool(file_info);
            return rc;
        }

        file_name = file_info -> FileName;

        if (StrCmp(file_name, L".") == 0 || StrCmp(file_name, L"..") == 0) {
            continue;
        }

        dp = FileDevicePath(dir_device_path, file_name);
        if (dp == NULL) {
            FreePool(file_info);
            return EFI_OUT_OF_RESOURCES;
        }

        cb(file_info);

        if (file_info -> Attribute & EFI_FILE_DIRECTORY) {

            EFI_FILE_HANDLE new_dir;

            rc = uefi_call_wrapper(dir -> Open, 5, dir, & new_dir, file_name, EFI_FILE_MODE_READ, 0);
            if (rc != EFI_SUCCESS) {
                FreePool(file_info);
                FreePool(dp);
                return rc;
            }
            uefi_call_wrapper(new_dir -> SetPosition, 2, new_dir, 0);

            rc = list_directory(new_dir, dp, cb);
            uefi_call_wrapper(dir -> Close, 1, new_dir);
            if (rc != EFI_SUCCESS) {
                FreePool(file_info);
                FreePool(dp);
                return rc;
            }
        }

        FreePool(dp);
    }
}

EFI_FILE_HANDLE get_volume(EFI_HANDLE image, EFI_DEVICE_PATH * dp)
{
	EFI_STATUS rc;
	EFI_LOADED_IMAGE *loaded_image = NULL;
	EFI_GUID lipGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
	EFI_FILE_IO_INTERFACE *fs;
	EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_FILE_HANDLE root;

	uefi_call_wrapper(BS->HandleProtocol, 3, image, &lipGuid, (void **) &loaded_image);
	uefi_call_wrapper(BS->HandleProtocol, 3, loaded_image->DeviceHandle, &fsGuid, (VOID*)&fs);
	
	rc = uefi_call_wrapper(BS -> OpenProtocol,
		6,
	    loaded_image->DeviceHandle, &
	    gEfiDevicePathProtocolGuid, &
	    dp,
	    NULL,
	    image,
	    EFI_OPEN_PROTOCOL_GET_PROTOCOL
	);
    // Print(L"LocateHandleBuffer : %r\n\r", rc);
    if(rc == EFI_SUCCESS){
        uefi_call_wrapper(fs->OpenVolume, 2, fs, &root);
    }

	return root;
}