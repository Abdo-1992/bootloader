#include "MCU_Drivers.h"
#include "gpio.h"
#include "sysctl.h"
#include "flash.h"
#include "uart.h"
#include "interrupt.h"
#include "pin_map.h"
#include "hw_memmap.h"


bool MCU_Drivers::MCU_flash_init()
{
    if(!MCU_is_device_supported(flash))
        return false;
        
}

bool MCU_Drivers::MCU_interrupt_init(){
    return false;
}

bool MCU_Drivers::MCU_gpio_init()
{
    if(!MCU_is_device_supported(gpio))
        return false;    
    uint8_t output_pins = GPIO_PIN_1 + GPIO_PIN_2 + GPIO_PIN_3;
    uint8_t input_pins = GPIO_PIN_0 + GPIO_PIN_4;
    GPIODirModeSet(GPIO_PORTF_BASE, output_pins, GPIO_DIR_MODE_OUT);
    GPIODirModeSet(GPIO_PORTF_BASE, input_pins, GPIO_DIR_MODE_IN);

    return true;
}

bool MCU_Drivers::MCU_sysctl_init()
{
    if(!MCU_is_device_supported(sysctl))
        return false;
    SysCtlClockSet(SYSCTL_XTAL_16MHZ);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralDisable(SYSCTL_PERIPH_UART2);
    //should have delay here?
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2));

    return true;
}

bool MCU_Drivers::MCU_uart_init()
{
    GPIOPinTypeUART(SYSCTL_PERIPH_GPIOD, GPIO_PIN_6 + GPIO_PIN_7);
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    return true;
}

bool MCU_Drivers::MCU_is_device_supported(MCU Peripheral)
{
    return ((1<<Peripheral) && SupportedPeripheral);
}

bool MCU_Drivers::MCU_init_drivers(){
    if(!MCU_sysctl_init())
        return false;    
    if(!MCU_flash_init())
        return false;
    if(!MCU_gpio_init())
        return false;
    if(!MCU_uart_init())
        return false;
    if(!MCU_interrupt_init())
        return false;
    return true;        
}

MCU_Drivers::MCU_Drivers(){
    IntMasterDisable();
    SupportedPeripheral = (1 << flash) | (1 << gpio) | (1 << sysctl) | (1 << uart);
}

MCU_Drivers::~MCU_Drivers()
{
}
