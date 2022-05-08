# NesUEFI - A bootable NES Emulator running on UEFI

![alt text](/splash.bmp "splash")

**NesUEFI** is a **NES Emulator running without an operating system** directly on **Unified Extensible Firmware Interface (UEFI).** Simply place **NesUEFI** and nes roms in a pendrive and play.  **NesUEFI file browser**  will show the available roms. The core drivers (Graphics, File System, Timer) are written in c using **GNU-EFI** and the emulator part is a port of [LiteNES](https://github.com/NJU-ProjectN/LiteNES). Adafruit GFX Graphics Library is ported for GNU-EFI ecosystem in C for high level graphics operations and text rendering. 

# Compile NesUEFI
## Requirements

- **GNU-EFI** Library. Download it from [here](https://sourceforge.net/projects/gnu-efi/).
- **gcc-3.0 or newer**
- A version of **objcopy** that supports EFI applications
- **qemu** (To run without real hardware)
- **mtools** : Utilities to access MS-DOS disks from Linux (to run on qemu)
- **OVMF** (To run on qemu)


## Compile and install GNU-EFI Library
Go to gnu-efi source folder
```bash
make
make install
```
[Here](compile_guide.md) is a guide you can follow for more details on how to compile and run gnu-efi applications.
 
## Compile NesUEFI
Change the following macros in makefile to locate GNU-EFI **libraries** and **headers** installed on the previous step.

```makefile
INCDIR		= /usr/local/include
LIBDIR		= /usr/local/lib
EFILIB		= /usr/local/lib
```

# NesUEFI on qemu :

### Locate OVMF  in Makefile:
To run the compiled application in qemu we need OVMF for UEFI emulation. OVMF is a port of Intel's tianocore firmware to the qemu virtual machine.
```makefile
OVMF_DIR		= ../OVMF
```
Change **OVMF_DIR** directory in the makefile.

### Create Image
Add your roms here in the makefile to write it to your image. For example : test_1.nes, test_2.nes
```makefile
mcopy -i $(IMAGE).img splash.bmp ::
# Add your roms here
mcopy -i $(IMAGE).img test_1.nes ::
mcopy -i $(IMAGE).img test_2.nes ::
```
```bash
make img
```

### Run
```bash
make run
```
# NesUEFI on real hardware
**Do at your own risk. Under no circumstances shall the author be liable for any damage.**
Technically NesUEFI should not mess with your system. But you should at least know what you are doing. 

- Format a pendrive with **FAT32** file system.
- Create **/efi/boot/** folders in the pendrive
- Rename **main.efi** to **bootx64.efi** for **64 bit** and **bootia32.efi** for **32 bit** application.
- Copy **bootx64.efi** or **bootia32.efi** to boot folder.
- For **64 bit** application the pendrive should look like this : **/efi/boot/bootx64.efi** 
- For **32 bit** application the pendrive should look like this : **/efi/boot/bootia32.efi** 
- Copy **splash.bmp** on the root folder of the pendrive
- Copy your .nes roms in the pendrive.

# Demo on Real Hardware
Video Coming Soon
## Controls 
| NesUEFI Key | Keyboard Key |  
| ----------- | ----------- |  
| Up | W |
| Down | S |
| Left | A |
| Right | D |
| Select | U |
| Start | I |
| A | K |
| B | J |
| Reload | R |
## Known Issues
- Mappers are not implemented yet. 
- It does not support Audio emulation yet. 
- Only single player is supported. 
- NesUEFI only supports PS/2 keyboard. Most of the laptop keyboards should support it.

## Tested Games
Below games are currently tested on real hardware. But NesUEFI is not only limited to these games. It should technically run all the classic roms that use mapper 0. [Here](https://nesdir.github.io/mapper0.html) is a list of mapper 0 games.
- [x] Super Mario Bros
- [x] Popeye
- [x] Yie Ar Kung-Fu
- [x] Battle City
- [x] Super Arabian
- [x] Road Fighters
- [x] 1942
- [x] F1 Race


## TODO
- [x] Graphics
- [x] Timer
- [x] Keyboard
- [x] File System
- [x] File Browser
- [x] Graphics Library
- [ ] APU
- [ ] Mappers
- [ ] USB Keyboard Driver

## Acknowledgements
- https://wiki.osdev.org
- https://wiki.osdev.org/Tutorials
- https://wiki.osdev.org/UEFI
- https://wiki.osdev.org/GNU-EFI
- https://wiki.osdev.org/UEFI_App_Bare_Bones
- https://wiki.osdev.org/Loading_files_under_UEFI
- https://github.com/NJU-ProjectN/LiteNES
- https://learn.adafruit.com/adafruit-gfx-graphics-library
- https://edk2-devel.narkive.com/WhxSiG6I/edk2-directory-file-system-traversal-example
- https://www.youtube.com/watch?v=mpPbKEeWIHU&list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ
- https://blog.fpmurphy.com/2015/08/display-bmp-details-and-image-in-uefi-shell.html#ixzz7SHLiufWS
- https://unsplash.com/photos/UqRnUzV5pjQ
- https://www.qmtpro.com/~nes

## Screenshots

![alt text](/screenshots/0.png "Menu")
![alt text](/screenshots/1.png "Super Mario")
![alt text](/screenshots/2.png "Super Arabian")
![alt text](/screenshots/3.png "1942")

# License 
Licensed under the [MIT](https://github.com/shadlyd15/NesUEFI/blob/master/LICENSE) License.
