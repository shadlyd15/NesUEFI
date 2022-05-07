#
# Copyright (c) 2011, Intel Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#


IMAGE 			= 	NesUEFI
TARGET 			= 	main.efi 

INCDIR 			= 	/usr/local/include

LIBDIR			= 	/usr/local/lib
EFILIB          = 	/usr/local/lib
OVMF_DIR 		= 	../OVMF

CC              = 	gcc
OBJCOPY 		= 	objcopy

%.efi: %.so
	$(OBJCOPY) -j .text -j .sdata -j .data -j .dynamic -j .dynsym -j .rel \
		-j .rela -j .reloc -S --target=$(FORMAT) $*.so $@


HOST  = $(shell $(CC) -dumpmachine | sed "s/\(-\).*$$//")
ARCH := $(shell $(CC) -dumpmachine | sed "s/\(-\).*$$//")


ifeq ($(ARCH),x86_64)
	FORMAT=efi-app-x86-64
else
	ARCH=ia32
	FORMAT=efi-app-$(ARCH)
endif

EFI_CRT_OBJS    := $(EFILIB)/crt0-efi-$(ARCH).o
EFI_LDS         := $(EFILIB)/elf_$(ARCH)_efi.lds

CFLAGS=-I. -I$(INCDIR)/efi -I$(INCDIR)/efi/$(ARCH) -mno-red-zone -Wno-unused-variable\
		-DEFI_FUNCTION_WRAPPER -fPIC -fshort-wchar -ffreestanding \
		-Wall -D$(ARCH) -Werror

ifeq ($(ARCH),ia32)
	ifeq ($(HOST),x86_64)
		CFLAGS += -m32
	endif
endif
ifeq ($(ARCH),x86_64)
	CFLAGS += -mno-red-zone
endif

LDFLAGS=-T $(EFI_LDS) -Bsymbolic -shared -nostdlib -znocombreloc \
		-L$(LIBDIR) $(EFI_CRT_OBJS)

SOURCES         := $(wildcard ./**/*.c) $(wildcard ./*.c)
OBJS            := $(patsubst ./%.c, ./%.o, $(SOURCES))

all: $(TARGET)

$(TARGET): main.so

main.so: $(OBJS) 
	$(LD) $(LDFLAGS) -o $@ $^  -lgnuefi -lefi $(shell $(CC) $(CFLAGS) -print-libgcc-file-name)

clean:
	rm -f $(TARGET) main.so $(OBJS) $(IMAGE).img



img:
	dd if=/dev/zero of=$(IMAGE).img bs=512 count=93750
	mformat -i $(IMAGE).img -h 32 -t 32 -n 64 -c 1 ::
	mmd -i $(IMAGE).img ::/EFI
	mmd -i $(IMAGE).img ::/EFI/BOOT
	mcopy -i $(IMAGE).img $(TARGET) ::/EFI/BOOT
	mcopy -i $(IMAGE).img startup.nsh ::
	mcopy -i $(IMAGE).img splash.bmp ::
	# Add your roms here
	mcopy -i $(IMAGE).img nestest.nes ::

run:
	qemu-system-x86_64  -drive format=raw,file=$(IMAGE).img -m 256M -cpu qemu64 \
						-drive if=pflash,format=raw,unit=0,file="$(OVMF_DIR)/OVMF_CODE-pure-efi.fd",readonly=on \
						-drive if=pflash,format=raw,unit=1,file="$(OVMF_DIR)/OVMF_VARS-pure-efi.fd" \
						-net none

