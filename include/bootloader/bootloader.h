#ifndef __BOOTLOADER_H__
#define __BOOTLOADER_H__
#include <stdint.h>

class bootloader
{
private:
    uint8_t* StartAppAddr ;


public:
    bootloader();
    ~bootloader();
    void BootloaderInitialize();
};



#endif