#ifndef __MCU_DRIVERS_H__
#define __MCU_DRIVERS_H__
#include <stdint.h>


enum MCU{
    adc,
    gpio,
    aes,
    crc,
    eeprom,
    flash,
    hibernate,
    i2c,
    mpu,
    rom,
    interrupt,
    shamd5,
    sysctl,
    sysexc,
    systick,
    timer,
    uart,
    watchdog,
};

class MCU_Drivers
{
private:
    uint32_t SupportedPeripheral{};
    /*Flash group*/
    bool MCU_flash_init();
    /*GPIO group*/
    bool MCU_gpio_init();
    /*System Control group*/
    bool MCU_sysctl_init();
    /*UART group*/
    bool MCU_uart_init();
    /*Interrupt group*/
    bool MCU_interrupt_init();
public:
    bool MCU_init_drivers();
    bool MCU_is_device_supported(MCU);
    MCU_Drivers();
    ~MCU_Drivers();
};

#endif