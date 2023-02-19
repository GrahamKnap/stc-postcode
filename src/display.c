// project definitions
#include "global.h"

// definitions for this file
#include "display.h"

// other local headers
// (none)

// system headers
#include "stc12.h"

// Global variables
uint8_t digit[2];

// The following definitions must be provided by global.h:
//
// #define COMMON_ANODE         TRUE or FALSE
// #define LED_COMMON_PORT      P3
// LED digit common connections, either common anodes or cathodes.
//
// #define LED_COMMON_MASK      0b00111100
// A value with bits set corresponding to LED common connections.
//
// #define LED_COMMON_BIT(pos)  (2 + (pos))
// Maps a digit position to an output bit (0-7).
//
// #define LED_SEGMENT_PORT     P2
// The code assumes that Px.0 - Px.7 map to LED segments A - G, DP.
// See the SEVENSEG and DP_MASK macros in "display.h".

#define CONCAT_NX(a, b)     a ## b
#define CONCAT(a, b)        CONCAT_NX(a, b)

#define LED_SEGMENT_PXM0    CONCAT(LED_SEGMENT_PORT, M0)
#define LED_SEGMENT_PXM1    CONCAT(LED_SEGMENT_PORT, M1)
#define LED_COMMON_PXM0     CONCAT(LED_COMMON_PORT, M0)
#define LED_COMMON_PXM1     CONCAT(LED_COMMON_PORT, M1)

// Accesses to the LED Common register must be implemented so as to avoid
// affecting other pins within the same port. The following are OK:
//
// orl _P3, #0x04
// anl _P3, #0xC3
// setb _P3_2
// clr _P3_2
//
// The following can interfere with other functions:
//
// mov r6, _P3
// orl r6, a
// mov _P3, r6
//
// This was found to break a serial port assigned to P3.6/P3.7.


#define T0FREQ          240
#define T0RELOAD        (65536 - (FOSC / (12 * T0FREQ)))


void InitDisplay(void)
{
    TMOD |= T0M0;   // Mode 1: 16-bit timer/counter, no auto-reload
    TL0 = T0RELOAD & 0xFF;
    TH0 = T0RELOAD >> 8;
    TR0 = 1;        // Timer0 start run
    PT0 = 1;        // high interrupt priority
    ET0 = 1;        // enable timer0 interrupt
    EA  = 1;        // global interrupt enable

    // PxM0 = 0, PxM1 = 0: 20mA sink, weak pull-up, quasi-bidir (default)
    // PxM0 = 0, PxM1 = 1: 20mA push/pull
    // PxM0 = 1, PxM1 = 0: input only
    // PxM0 = 1, PxM1 = 1: open drain
    //
    // The weak pull-up of ~200uA is insufficient for anodes,
    // so set anode connections to 20mA push/pull.
#if COMMON_ANODE
    //LED_COMMON_PXM0 &= ~(LED_COMMON_MASK);
    LED_COMMON_PXM1 |= LED_COMMON_MASK;
    //LED_SEGMENT_PXM0 = 0;
    //LED_SEGMENT_PXM1 = 0;
    LED_COMMON_PORT &= ~(LED_COMMON_MASK);
#else
    //LED_COMMON_PXM0 &= ~(LED_COMMON_MASK);
    //LED_COMMON_PXM1 &= ~(LED_COMMON_MASK);
    //LED_SEGMENT_PXM0 = 0;
    LED_SEGMENT_PXM1 = 0xff;
    LED_COMMON_PORT |= (LED_COMMON_MASK);
#endif
}

void ISR_Timer0(void) __interrupt (1)
{
    static uint8_t pos;             // cycles through digit positions 0 to 1

    TL0 = T0RELOAD & 0xFF;
    TH0 = T0RELOAD >> 8;

#if COMMON_ANODE
    LED_COMMON_PORT &= ~(LED_COMMON_MASK);
    LED_SEGMENT_PORT = ~digit[pos];

    switch (pos)
    {
        case 0: LED_COMMON_PORT |= (1 << LED_COMMON_BIT(0)); break;
        default: LED_COMMON_PORT |= (1 << LED_COMMON_BIT(1)); break;
    }
#else
    LED_COMMON_PORT |= (LED_COMMON_MASK);
    LED_SEGMENT_PORT = digit[pos];

    switch (pos)
    {
        case 0: LED_COMMON_PORT &= ~(1 << LED_COMMON_BIT(0)); break;
        default: LED_COMMON_PORT &= ~(1 << LED_COMMON_BIT(1)); break;
    }
#endif

    if (pos < 1)
    {
        pos++;
    }
    else
    {
        pos = 0;
    }
}

static const uint8_t hexadecimalGlyph[] = {
    SEVENSEG_0, SEVENSEG_1, SEVENSEG_2, SEVENSEG_3,
    SEVENSEG_4, SEVENSEG_5, SEVENSEG_6, SEVENSEG_7,
    SEVENSEG_8, SEVENSEG_9, SEVENSEG_A, SEVENSEG_b,
    SEVENSEG_C, SEVENSEG_d, SEVENSEG_E, SEVENSEG_F
};

void DisplayHex(uint8_t value)
{
    digit[0] = hexadecimalGlyph[value >> 4];
    digit[1] = hexadecimalGlyph[value & 0x0f];
}

void ClearDisplay(void)
{
    digit[0] = 0;
    digit[1] = 0;
}
