#include "MCU_Drivers.h"


static void GPIOF_Handler(void);
static void SystickHandler(void);
static void UART0_Handler(void);
extern "C" void UARTStdioIntHandler(void);

static uint64_t SystickTime_ms = 0;
MCU_Drivers mcu_drivers{};

bool MCU_Drivers::MCU_flash_init()
{
    if(!MCU_is_device_supported(flash))
        return false;
    return true;
}

bool MCU_Drivers::MCU_interrupt_init()
{
    
    UARTIntClear(UART0_BASE,UART_INT_RX|UART_INT_RT);
    GPIOIntClear(GPIO_PORTF_BASE,leftSwitch);
    GPIOIntRegister(GPIO_PORTF_BASE,GPIOF_Handler);
    UARTIntRegister(UART0_BASE,UART0_Handler);
    SysTickIntRegister(SystickHandler);
    SysTickIntEnable();
    SysTickEnable();
    SysTickPeriodSet(SysCtlClockGet()/1000);
    IntMasterEnable();
    return true;
}

bool MCU_Drivers::MCU_gpio_init()
{
    if(!MCU_is_device_supported(gpio))
        return false;    
    uint8_t output_pins_led  = red + green + blue;
    uint8_t output_pins_bluetooth = bluetooth_state;
    uint8_t input_pins = leftSwitch;
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, output_pins_led);
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, output_pins_bluetooth);
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

void MCU_Drivers::MCU_send_message(const char *pcBuf, uint32_t ui32Len){
    UARTputBuff(pcBuf, ui32Len);
}

State MCU_Drivers::MCU_get_switch(Switch sw){
    State value = static_cast<State>(GPIOPinRead(GPIO_PORTF_BASE,sw));
    return value;
}

bool MCU_Drivers::MCU_bluetooth_connected()
{
    return GPIOPinRead(GPIO_PORTC_BASE, bluetooth_state);
}

bool MCU_Drivers::MCU_sysctl_init()
{
    if(!MCU_is_device_supported(sysctl))
        return false;
    // Set the clocking to run directly from the crystal.
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));    

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0));
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));    
    return true;
}

bool MCU_Drivers::MCU_uart_init()
{
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1); 
    UARTStdioConfig(0, 9600, SysCtlClockGet());
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

void MCU_Drivers::MCU_delay_ms(uint64_t ms){
    uint64_t timeout = MCU_get_Systick() + ms;
    while(timeout - MCU_get_Systick());
}

uint64_t MCU_Drivers::MCU_get_Systick(void)
{
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
    GPIOIntDisable(GPIO_PORTF_BASE,leftSwitch);
    GPIOIntClear(GPIO_PORTF_BASE,leftSwitch);
    GPIOIntEnable(GPIO_PORTF_BASE, leftSwitch);
}

void UART0_Handler(void)
{
    UARTStdioIntHandler();
}

void SystickHandler(){
    SystickTime_ms++;
}
extern "C"
void Delay_ms(uint64_t delay){
    mcu_drivers.MCU_delay_ms(delay);
}
