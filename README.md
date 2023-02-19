# STC12x2052AD based POST code display gadget

This simple gadget displays POST codes on motherboards that provide a logic-level serial output for this purpose. This feature is typically implemented by a "Port 80" feature in the Super I/O chip, and may be accessible through a debug header on the motherboard. (Many Super I/O chips can also drive a 2-digit 7-segment display, but there's often no convenient way to access these signals.)

## License and Disclaimer

This project is licensed under the MIT License; see the `license` file for details.

This software and documentation is provided as-is, with no warranty. You may use it at your own risk. See the license for further details.

## Preparation

Ensure that all equipment is grounded correctly. Connect and disconnect equipment only while it is powered off.

Before connecting anything to your motherboard, verify the pin assignments and signal voltages. Do not assume that your motherboard's pin assignments match any information that you may find here or elsewhere.

### Asus COM_DEBUG header

```
GND  GND  COM1
P80  Key  +3V
```

## Alternative

Instead of building this gadget, you can use a PC and a USB serial interface to read POST codes.

Verify that your USB serial interface uses the same signal voltage as your motherboard. Connect only the "ground" and "receive" lines from your USB serial interface to the motherboard.

Each code is transmitted as a byte, which you will want to display as a two-digit hexadecimal value.

To do this on Linux: (adapted from a blog entry by [Alan C. Assis](https://acassis.wordpress.com/2016/01/22/configuring-linuxs-serial-port-to-raw-mode/))

```
$ stty -F /dev/ttyUSB0 115200
$ stty -F /dev/ttyUSB0 raw -echo -echoe -echok
$ hexdump -v -e '/1 "%02X\n"' /dev/ttyUSB0
```

## Hardware construction

### Materials

* (1) STC12Cx052 (5V) or STC12LEx052 (3.3V) microcontroller
* (1) 10uF decoupling capacitor
* (1) 0.1uF decoupling capacitor
* (1) 10K resistor
* (1) 300-ohm (5V) or 180-ohm (3.3V) resistor
* (1) 1N5817 diode
* (1) 2-digit 7-segment LED display
* (8) resistors for LED segment connections
* (1) crystal oscillator
* (2) 20pF capacitors
* (1) 4-pin header (VCC, GND, RxD, TxD)
* circuit board, solder, etc.

### STC12C2052AD connections

1. RST: 10K pull-down
2. P3.0/RxD: serial receive
3. P3.1/TxD: serial transmit
4. XTAL2: crystal, 20pF to GND
5. XTAL1: crystal, 20pF to GND
6. P3.2: LED common digit 0
7. P3.3: LED common digit 1
8. P3.4: NC
9. P3.5: NC
10. GND
11. P3.7: NC
12. P1.0: LED segment A
13. P1.1: LED segment B
14. P1.2: LED segment C
15. P1.3: LED segment D
16. P1.4: LED segment E
17. P1.5: LED segment F
18. P1.6: LED segment G
19. P1.7: LED decimal point
20. VCC

### 7-segment display

Especially if you're using a 3.3V MCU, pay attention to the voltage required by your LEDs.

Connect the common lines directly to the MCU. Connect each segment through a resistor. I found that 1K worked well with red LEDs and a 3.3V MCU. You may need a lower value for different LEDs, or a higher value for a 5V MCU.

### Serial connections

To ensure that the MCU will power-cycle correctly during programming, and to help avoid damage to a connected motherboard:

* Add a resistor on the MCU TxD line. I suggest 180-200 ohms for a 3.3V MCU, or 270-300 ohms for a 5V MCU.
* Add a 1N5817 diode on the MCU RxD line, with the cathode (stripe) away from the MCU.

### Crystal oscillator

The STC12C2052AD family provides a low-accuracy, non-adjustable internal oscillator. It is unsuitable for an application that requires serial communication at 115200 bps. This requires a crystal oscillator with a frequency that is a multiple of 16 &times; 115200 Hz = 1.8432 MHz; I recommend 7.3728 MHz or 11.0592 MHz. The STC datasheet describes how it should be connected.

On a new MCU, the internal clock is initially selected. The datasheet says that XTAL1/XTAL2 should remain disconnected (floating) while using the internal clock.

## Compiling

To compile the software, you'll need `make` and [SDCC](http://sdcc.sf.net).

Configure the software for your hardware editing `global.h`.

Run `make` to compile the software; this produces `postcode.hex`.

## Installation

You will need a USB-to-serial converter that provides the correct power and signal voltages for your MCU.

To configure the processor and flash the firmware, you'll need either [stcgal](https://github.com/grigorig/stcgal) or [STC-ISP](http://www.stcmcudata.com/STCISP/stc-isp-15xx-v6.90D.zip) (Windows-only).

The MCU must be power-cycled to initiate programming. This can be done by temporarily disconnecting the VCC wire to your gadget. Cycling the USB connection is not recommended.

The included `program.sh` script provides an example of `stcgal` invocation.

The STC-ISP tool is pretty straightforward, and is described in the datasheet.

You can select the clock source during programming.

Your MCU may be configured to require P1.0 and P1.1 pulled down to enable programming. This option can be changed during programming; if using `stcgal`, see option `bsl_pindetect_enabled`.

## Authors

Graham Knap

## References

* [STC website (in Chinese)](http://www.stcmcu.com)
* [STC12C2052AD series English datasheet](http://www.stcmcudata.com/datasheet/stc/STC-AD-PDF/STC12C2052AD-english.pdf)
* [SDCC User Guide (PDF)](http://sdcc.sourceforge.net/doc/sdccman.pdf)
* [stcgal documentation](https://github.com/grigorig/stcgal/blob/master/doc/USAGE.md)
