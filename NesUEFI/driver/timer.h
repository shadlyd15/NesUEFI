#ifndef TIMER_H
#define TIMER_H

#include <efi.h>

#define TICK_PER_SECOND 120

EFI_STATUS timer_init(uint32_t tps);
unsigned long long timer_ticks();

#endif