
# Compile and run GNU-EFI applications under linux

## Requirements :

Download gnu-efi from here : [https://sourceforge.net/projects/gnu-efi/](https://sourceforge.net/projects/gnu-efi/)
- **gcc-3.0 or newer**
```bash
	gcc --version
```
- A version of **objcopy** that supports EFI applications : 
```bash 
	objcopy --help 
```
Check if supported target contains elf64-x86-64 elf32-i386 elf32-x86-64

- **mtools** : Utilities to access MS-DOS disks from Linux

## Compile and install GNU-EFI Library:
Go to gnu-efi source folder
```bash
	make
	make install
```
## Compile GNU-EFI application :
Use this makefile to easily compile and run gnu-efi application. It will recursively compile all c files in the sub-directories. 
Change the following macros to locate the libraries and headers installed on the previous step.

```makefile
	IMAGE		= uefi_app 
	TARGET		= main.efi 

	INCDIR		= /usr/local/include
	LIBDIR		= /usr/local/lib
	EFILIB		= /usr/local/lib
```
## Run on qemu :
### Locate OVMF  in Makefile:
To run the compiled application in qemu we need OVMF for UEFI emulation. OVMF is a port of Intel's tianocore firmware to the qemu virtual machine. Download it from here.
```makefile
	OVMF_DIR		= ../OVMF
```
Change the OVMF directory in the makefile.

### Create Image :
```bash
	make img
```
It will do the following tasks : 
```bash
	# Create a new image file that will contain the GNU-EFI application. 
	dd if=/dev/zero of=$(IMAGE).img bs=512 count=93750
	# mformat to format it with FAT16.
	mformat -i $(IMAGE).img -h 32 -t 32 -n 64 -c 1 ::
	# Create directory
	mmd -i $(IMAGE).img ::/EFI
	mmd -i $(IMAGE).img ::/EFI/BOOT
	# Copy image
	mcopy -i $(IMAGE).img $(TARGET) ::/EFI/BOOT
	# Copy the nsh script to load efi application automatically from fs0
	mcopy -i $(IMAGE).img startup.nsh ::
```
### Run :
```bash
	make run
```
## Running on real hardware :
- Format a pendrive with FAT32 file system.
- Create /efi/boot/ folders in the pendrive
- Rename main.efi to bootx64.efi for 64 bit and bootia32.efi for 32 bit application.
- For 64 bit application the pendrive should look like this : **/efi/boot/bootx64.efi** 
- For 32 bit application the pendrive should look like this : **/efi/boot/bootia32.efi** 

# Further Reading : 
- https://wiki.osdev.org/UEFI
- https://wiki.osdev.org/GNU-EFI
- https://wiki.osdev.org/UEFI_App_Bare_Bones