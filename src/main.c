//
// STC12LE4052AD POST code display
//

// project definitions
#include "global.h"

// definitions for this file
// (none)

// other local headers
#include "display.h"
#include "serial.h"

// system headers
#include <stdint.h>
#include "stc12.h"

int main()
{
    InitSerial();
    InitDisplay();
    digit[0] = SEVENSEG_dash;
    digit[1] = SEVENSEG_dash;

    while (TRUE)
    {
        // nothing here; everything is handled by interrupts
    }
}
