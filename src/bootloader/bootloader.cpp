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
    HC05 bluetooth{}; 
    while(1){
        bluetooth.findBaud();
        McuDrivers.MCU_delay_ms(1000);
    }
}
/*
    McuDrivers.MCU_turn_Led(green, on);
    McuDrivers.MCU_turn_Led(green, off);
    McuDrivers.MCU_turn_Led(red, on);
    McuDrivers.MCU_turn_Led(red, off);
    McuDrivers.MCU_turn_Led(blue, on);
    McuDrivers.MCU_turn_Led(blue, off);
*/
