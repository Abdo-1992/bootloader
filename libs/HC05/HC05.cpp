#include <string.h>
#include "HC05.h"
#include "MCU_Drivers.h"

static const char* BaudrateToString(unsigned long baud);

HC05::HC05()
{
    _bufsize = sizeof(_buffer)/sizeof(char);
}

static const unsigned long rates[] =
    {4800,9600,19200,38400,57600,115200};

unsigned long HC05::findBaud()
{
    const int bt_rx = 4;
    const int bt_tx = 5;
    int numRates = sizeof(rates)/sizeof(unsigned long);
    int response = false;
    int recvd = 0;
    //char _buffer[128];

    McuDrivers.MCU_delay_ms(100);
    for(int rn = 0; rn < numRates; rn++)
    {
        UARTStdioConfig(2, rates[rn], SysCtlClockGet());
        //trying
        UARTwrite("AT\r\n", 4);
        recvd = UARTgets(_buffer,_bufsize);
        if (recvd > 0)
        {
            //found
            // FIXME: refactor to a single return
            return(rates[rn]);
        }
        else
        {
            // continue
        }
    }
    // No connection
    return(0);
}

int HC05::cmd(const char* cmd, unsigned long timeout)
{
    int recvd = 0;

    // No spec for how long it takes to enter command mode, but 100ms
    // seems to work- assuming the output has been drained.
    McuDrivers.MCU_delay_ms(100);
    UARTwrite(cmd, strlen(cmd));
    UARTwrite("\r\n", 2);
    do
    {
        // ATTENTION: At least through Arduino v1.0.3, it is not possible
        //            to tell the difference between a timeout and
        //            receiving only the termination character (NL in this
        //            case), because the termination character is not
        //            returned and timeout is not returned as a unique
        //            indication.
        //            In this case the result would be an early return
        //            of a multiline response before the OK is received.
        //            The return would incorrectly indicate an error (no
        //            OK response).
        recvd = UARTgets(_buffer,_bufsize);
        if (recvd > 0)
        {
            //there is data
        }
        else
        {
            // no data timeout
        }
    }
    while ((recvd > 0) && (_buffer[0] != 'O' || _buffer[1] != 'K'));

    // Empirically determined that it takes some time to reliably exit
    // command mode. The appeared to be a baud rate dependency and with
    // >100ms required at 9600 baud.
    McuDrivers.MCU_delay_ms(150);
    return((_buffer[0] == 'O' && _buffer[1] == 'K'));
}


/*
 * If setBaud() is called while the HC-05 is connected, then
 * it will be disconnected when AT+RESET command is issued, and
 * it may take 2 (or more?) connection attempts to reconnect. The extra
 * connect attempts may be a host side issue and not specific to the
 * HC-05 module.
 */
void HC05::setBaud(unsigned long baud)
{
    int recvd = 0;
    McuDrivers.MCU_delay_ms(200);

    const char* baudString = BaudrateToString(baud);

    UARTwrite("AT+UART=", strlen("AT+UART="));
    UARTwrite(baudString, strlen(baudString));

    UARTwrite(",",1);

    UARTwrite("0",1);

    UARTwrite(",",1);

    UARTwrite("0",1);

    UARTwrite("\r\n",2);

    recvd = UARTgets(_buffer,_bufsize);
    if (recvd > 0)
    {
        //there is data
    }
    else
    {
        // no data timeout
    }
    cmd("AT+RESET");
    UARTStdioConfig(2, baud, SysCtlClockGet());
    McuDrivers.MCU_delay_ms(1000);
}

void HC05::flush()
{
    // memset(_buffer,sizeof(_buffer),sizeof(_buffer));
    _bufsize = 0;
}

int HC05::read()
{
    return UARTgets(_buffer,_bufsize);
}

bool HC05::connected()
{
    return(McuDrivers.MCU_bluetooth_connected());
}

void HC05::write(uint8_t byte)
{
    // The down side of this check is that the status gets checked for
    // every byte written out. That doesn't seem efficient.
    if (connected())
    {
        // No Connection, waiting...
        return;
    }
    UARTputc(byte);
}


const char* BaudrateToString(unsigned long baud){
    switch (baud)
    {
    case 4800:
        return "4800";
        break;
    case 9600:
        return "9600";
        break;
    case 19200:
        return "19200";
        break;
    case 38400:
        return "38400";
        break;
    case 57600:
        return "57600";
        break;
    case 115200:
        return "115200";
        break;
    default:
        break;
    }
    return nullptr;
}

