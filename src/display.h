#ifndef _DISPLAY_H
#define _DISPLAY_H

#include <stdint.h>

// Segments A through F are in clockwise order around the perimeter,
// starting with segment A at the top. Segment G is in the center.

#define SEVENSEG(a,b,c,d,e,f,g) \
    (uint8_t)((0 << 7) | ((g) << 6) | ((f) << 5) | ((e) << 4) \
    | ((d) << 3) | ((c) << 2) | ((b) << 1) | ((a) << 0))

#define SEVENSEG_0 SEVENSEG(1,1,1,1,1,1,0)
#define SEVENSEG_1 SEVENSEG(0,1,1,0,0,0,0)
#define SEVENSEG_2 SEVENSEG(1,1,0,1,1,0,1)
#define SEVENSEG_3 SEVENSEG(1,1,1,1,0,0,1)
#define SEVENSEG_4 SEVENSEG(0,1,1,0,0,1,1)
#define SEVENSEG_5 SEVENSEG(1,0,1,1,0,1,1)
#define SEVENSEG_6 SEVENSEG(1,0,1,1,1,1,1)
#define SEVENSEG_7 SEVENSEG(1,1,1,0,0,0,0)
#define SEVENSEG_8 SEVENSEG(1,1,1,1,1,1,1)
#define SEVENSEG_9 SEVENSEG(1,1,1,1,0,1,1)

#define SEVENSEG_A SEVENSEG(1,1,1,0,1,1,1)
#define SEVENSEG_b SEVENSEG(0,0,1,1,1,1,1)
#define SEVENSEG_C SEVENSEG(1,0,0,1,1,1,0)
#define SEVENSEG_c SEVENSEG(0,0,0,1,1,0,1)
#define SEVENSEG_d SEVENSEG(0,1,1,1,1,0,1)
#define SEVENSEG_E SEVENSEG(1,0,0,1,1,1,1)
#define SEVENSEG_F SEVENSEG(1,0,0,0,1,1,1)

#define SEVENSEG_dash SEVENSEG(0,0,0,0,0,0,1)

#define DP_BIT      0x80

extern uint8_t digit[2];

void InitDisplay(void);
void DisplayHex(uint8_t value);
void ClearDisplay(void);

// SDCC requires that "a prototype of the ISR must be present or included in
// the file that contains the function main"
void ISR_Timer0() __interrupt (1);

#endif
