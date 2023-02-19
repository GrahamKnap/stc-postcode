#ifndef _STC12_H_
#define _STC12_H_

#include <8051.h>

__sfr __at (0x8e) AUXR;
__sfr __at (0x91) P1M0;
__sfr __at (0x92) P1M1;

// AUXR bits for STC12C2052AD family
#define T0X12       0x80
#define T1X12       0x40
#define UART_M0X6   0x20
#define EADC1       0x10
#define ESPI        0x08
#define ELVDI       0x04
// reserved         0x02
// reserved         0x01

// TMOD bits for STC12C2052AD family
#define T1GATE  0x80
#define T1COUNT 0x40    // else TIMER
#define T1M1    0x20
#define T1M0    0x10
#define T0GATE  0x08
#define T0COUNT 0x04    // else TIMER
#define T0M1    0x02
#define T0M0    0x01

#endif // _STC12_H_
