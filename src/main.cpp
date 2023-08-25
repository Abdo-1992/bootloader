#include <stdint.h>
#include <stdbool.h>
#include "bootloader.h"
#include "main.h"

int main(void)
{
    bootloader bootApplication;
    while(1)
    {

    }
}


void _exit(int status) {
    while(1) { }
}
