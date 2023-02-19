// project definitions
#include "global.h"

// definitions for this file
#include "serial.h"

// other local headers
#include "display.h"

// system headers
#include "stc12.h"
#include <stdio.h>

#define BAUD 115200

void InitSerial(void)
{
    // SM0 = 0, SM1 = 1: 8-bit UART, variable baud rate
    SM0 = 0;
    SM1 = 1;

    // REN: enable receive
    REN = 1;

    // SMOD: double baud rate
    PCON |= SMOD;

    // T1M1 = 1, T1M0 = 0: timer 1 mode 2: 8-bit auto-reload
    TMOD |= T1M1;

    // Timer 1 source is SYSclk/1
    AUXR |= T1X12;

    // Timer 1 auto-reload value and initial value
    uint8_t ticks = FOSC / (BAUD * 16);
    TH1 = 256 - ticks;
    TL1 = TH1;

    // Start timer 1
    TR1 = 1;

    // IE (interrupt enable) register
    // PS is left at its default of 0 (low UART1 interrupt priority).
    ES = 1;         // enable serial port 1 interrupt
    EA = 1;         // enable interrupts
}

#ifdef DEBUG

static char txQueue[8];
static uint8_t txStart;
static uint8_t txCount;
static __bit txBusy;

void ISR_UART1(void) __interrupt 4 __using 1
{
    if (RI)
    {
        uint8_t value = SBUF;
        RI = 0;
        DisplayHex(value);
    }

    if (TI)
    {
        TI = 0;

        if (txCount == 0)
        {
            txBusy = 0;
        }
        else
        {
            SBUF = txQueue[txStart];
            txStart = (txStart + 1) & 7;
            txCount--;
        }
    }
}

void PrintChar(uint8_t c)
{
    while (txCount == 8)
    {
        // wait until queue is not full
    }

    ES = 0;

    if (txBusy)
    {
        txQueue[(txStart + txCount) & 7] = c;
        txCount++;
    }
    else
    {
        txBusy = 1;
        SBUF = c;
    }

    ES = 1;
}

static void PrintHexDigit(uint8_t n)
{
    n += (n > 9) ? ('A' - 10) : '0';
    PrintChar(n);
}

void PrintHex(uint8_t value)
{
    PrintHexDigit(value >> 4);
    PrintHexDigit(value & 0x0f);
}

void PrintString(const char * s)
{
    while (*s != '\0')
    {
        PrintChar(*s);
        s++;
    }
}

// See function prototype in <stdio.h>
int putchar(int c)
{
    PrintChar(c);
    return c;
}

#else  // not DEBUG

void ISR_UART1(void) __interrupt 4 __using 1
{
    if (RI)
    {
        uint8_t value = SBUF;
        RI = 0;
        DisplayHex(value);
    }

    if (TI)
    {
        TI = 0;
    }
}

#endif // DEBUG
