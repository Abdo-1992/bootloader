#ifndef __MCU_DRIVERS_H__
#define __MCU_DRIVERS_H__
#include <stdint.h>
#include "gpio.h"
#include "sysctl.h"
#include "flash.h"
#include "uart.h"
#include "interrupt.h"
#include "pin_map.h"
#include "hw_memmap.h"
#include "hw_ints.h"
#include "uartstdio.h"
#include "systick.h"

enum Led{
    red   = 2,  //PF1
    blue  = 4,  //PF2
    green = 8,  //PF3
};

enum Switch{
  leftSwitch  = 16,   //PF4 
};

enum Bluetooth{
    bluetooth_Tx      = GPIO_PIN_6,//PD6 (Microcontroller Rx)
    bluetooth_rx      = GPIO_PIN_7,//PD7 (Microcontroller Tx)
    bluetooth_state   = GPIO_PIN_7,//PC6 
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
    void MCU_send_message(const char *pcBuf, uint32_t ui32Len);
    void MCU_delay_ms(uint64_t);
    uint64_t MCU_get_Systick(void);
    State MCU_get_switch(Switch);
    bool MCU_bluetooth_connected();
    MCU_Drivers();
    ~MCU_Drivers();
};

extern MCU_Drivers mcu_drivers;

#endif
