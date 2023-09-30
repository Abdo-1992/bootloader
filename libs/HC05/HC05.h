/*
 * HC05.h - interface definitions for the HC05 library
 *
 * Select hardware or software serial port:
 *   Define HC05_SOFTWARE_SERIAL to select a SoftwareSerial port, then
 *   initialize the HC05 class with either two arguments, for a hardware port,
 *   or four arguments for a software serial port:
 */
#ifndef HC05_h
#define HC05_h

#include "MCU_Drivers.h"
#include "uartstdio.h"


/*
 * This macro must be defined even if you are using a software serial
 * port. You can change this to any serial port supported by your
 * Arduino (i.e, Serial1, Serial2, etc.)
 */
#define HC05_HW_SERIAL_PORT Serial

class HC05
{
public:
    MCU_Drivers& McuDrivers{mcu_drivers} ;
    HC05();
    unsigned long findBaud();
    void setBaud(unsigned long baud);  // always no parity, one stop bit
    void write(uint8_t byte);
    void flush();
    int cmd(const char* cmd, unsigned long timeout=100);
    bool connected(void);
    int read(void);

private:
    int _statePin;
    int _bufsize;
    char _buffer[32];
};

#endif  // HC05_h
