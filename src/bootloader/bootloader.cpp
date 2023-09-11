#include "bootloader.h"

bootloader::bootloader()
{

}

bootloader::~bootloader()
{
}


void bootloader::BootloaderInitialize()
{
    McuDrivers.MCU_init_drivers();
    while(1){
        McuDrivers.MCU_send_message("hello",5);
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
