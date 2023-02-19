#ifndef _GLOBAL_H
#define _GLOBAL_H

#define TRUE    1
#define FALSE   0

//#define DEBUG

#define FOSC    7372800    // clock speed in Hz

#define COMMON_ANODE        FALSE

#define LED_COMMON_PORT     P3
#define LED_COMMON_MASK     0b00001100
#define LED_COMMON_BIT(pos) (2 + (pos))
#define LED_SEGMENT_PORT    P1

#endif  // file
