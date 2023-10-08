#include "bootloader.h"
#include "HC05.h"

bootloader::bootloader()
{
    systemTime = 0;
    StartAppAddr = nullptr;
}

bootloader::~bootloader()
{
}


void bootloader::BootloaderInitialize()
{
    McuDrivers.MCU_init_drivers();
    while(1){
        UARTprintf("hello");
        McuDrivers.MCU_delay_ms(1000);
    }
}
