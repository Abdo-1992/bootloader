#ifndef __MCU_DRIVERS_H__
#define __MCU_DRIVERS_H__
#include <stdint.h>

enum Led{
    red   = 2,  //PF1
    blue  = 4,  //PF2
    green = 8,  //PF3
};

enum Switch{
  leftSwitch  = 16,   //PF4 
};

enum State{
    off,    
    on,
};

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
    uint64_t SystickTime{};
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
    void MCU_turn_Led(Led, State);
    uint64_t MCU_get_Systick(void);
    State MCU_get_switch(Switch);
    MCU_Drivers();
    ~MCU_Drivers();
};

#endif