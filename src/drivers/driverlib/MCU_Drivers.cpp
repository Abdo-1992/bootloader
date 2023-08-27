#include "MCU_Drivers.h"
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

static void GPIOF_Handler(void);
static void UART2Handler(void);
static void SystickHandler(void);

static volatile uint64_t SystickTime_ms = 0;

bool MCU_Drivers::MCU_flash_init()
{
    if(!MCU_is_device_supported(flash))
        return false;
    return true;
}

bool MCU_Drivers::MCU_interrupt_init()
{
    IntMasterEnable();    
    UARTIntClear(UART2_BASE,(UART_INT_RX|UART_INT_TX));
    GPIOIntClear(GPIO_PORTF_BASE,leftSwitch);
    GPIOIntRegister(GPIO_PORTF_BASE,GPIOF_Handler);
    UARTIntRegister(UART2_BASE,UART2Handler);
    SysTickIntRegister(SystickHandler);
    SysTickIntEnable();
    SysTickEnable();
    SysTickPeriodSet(SysCtlClockGet()/1000);    
    return true;
}

bool MCU_Drivers::MCU_gpio_init()
{
    if(!MCU_is_device_supported(gpio))
        return false;    
    uint8_t output_pins = GPIO_PIN_1 + GPIO_PIN_2 + GPIO_PIN_3;
    uint8_t input_pins = GPIO_PIN_4;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, output_pins);
    GPIODirModeSet(GPIO_PORTF_BASE, input_pins, GPIO_DIR_MODE_IN);
    GPIOPadConfigSet(GPIO_PORTF_BASE, input_pins, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOIntEnable(GPIO_PORTF_BASE, input_pins);
    GPIOIntTypeSet(GPIO_PORTF_BASE, input_pins, GPIO_LOW_LEVEL);
    GPIOIntClear(GPIO_PORTF_BASE, input_pins);

    return true;
}

void MCU_Drivers::MCU_turn_Led(Led led, State state){
    if(state == on) state = static_cast<State>(led);
    GPIOPinWrite(GPIO_PORTF_BASE, led, state);
}

State MCU_Drivers::MCU_get_switch(Switch sw){
    State value = static_cast<State>(GPIOPinRead(GPIO_PORTF_BASE,sw));
    return value;
}

bool MCU_Drivers::MCU_sysctl_init()
{
    if(!MCU_is_device_supported(sysctl))
        return false;
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART2));

    return true;
}

bool MCU_Drivers::MCU_uart_init()
{
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);
    GPIOPinTypeUART(GPIO_PORTD_BASE, (GPIO_PIN_6|GPIO_PIN_7));
    GPIOPadConfigSet(GPIO_PORTD_BASE, (GPIO_PIN_6|GPIO_PIN_7), GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    UARTClockSourceSet(UART2_BASE, UART_CLOCK_PIOSC);
    UARTStdioConfig(2, 9600, SysCtlClockGet());

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

uint64_t MCU_get_Systick(){
    return SystickTime_ms;
}

MCU_Drivers::MCU_Drivers(){
    IntMasterDisable();
    SupportedPeripheral = (1 << flash) | (1 << gpio) | (1 << sysctl) | (1 << uart);
}

MCU_Drivers::~MCU_Drivers()
{
}


void GPIOF_Handler(void)
{
    GPIOIntClear(GPIO_PORTF_BASE,leftSwitch);
}

void UART2Handler(void)
{
    UARTIntClear(UART2_BASE,(UART_INT_RX|UART_INT_TX));    
}

void SystickHandler(){
    SystickTime_ms++;
}