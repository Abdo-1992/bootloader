#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__
#include <stdint.h>
#include "MCU_Drivers.h"

class bootloader
{
private:
    uint8_t* StartAppAddr   ;
    uint64_t systemTime     ;
    MCU_Drivers& McuDrivers{mcu_drivers} ;

public:
    bootloader();
    ~bootloader();
    void BootloaderInitialize();
};



#endif
