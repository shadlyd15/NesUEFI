@echo -off
if exist fs0:\efi\boot\main.efi then
 fs0:
 echo Booting NesUEFI:
 efi\boot\main.efi
endif