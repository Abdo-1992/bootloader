#include <stdint.h>
#include <stdbool.h>
#include "bootloader.h"
#include "main.h"

int main(void)
{
    bootloader bootApplication{};
    bootApplication.BootloaderInitialize();
    while(1)
    {

    }
}

