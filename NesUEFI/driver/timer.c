#include <efi.h>
#include <efilib.h>
#include "kernel.h"
#include "timer.h"

EFI_EVENT  TimerEvent;


EFIAPI void TimerHandler ( IN EFI_EVENT  Event, IN void *Context){    
    kernel.ticks++;   
}

unsigned long long timer_ticks(){
    return kernel.ticks;
}

EFI_STATUS timer_init(uint32_t tps){
    EFI_STATUS rc;
    rc = uefi_call_wrapper(BS->CreateEvent, 5, EVT_TIMER | EVT_NOTIFY_SIGNAL, TPL_CALLBACK, TimerHandler, NULL, &TimerEvent);
    Print(L"Event Status : %r\n", rc);

    rc = uefi_call_wrapper(BS->SetTimer, 3, TimerEvent, TimerPeriodic, 10000000 / tps);
    Print(L"Timer Status : %r\n", rc);
    return rc;
}