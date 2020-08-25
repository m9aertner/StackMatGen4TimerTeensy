/*
 *    LedControlMatrix.cpp - A library for controlling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2020 Matthias Gaertner
 *
 *    based on
 *    https://github.com/wayoda/LedControl/commit/f86a18493e9594d7b7198bf894f529a1c79d5802
 *
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LedControlMatrix_h
#define LedControlMatrix_h

#include <avr/pgmspace.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

// The opcodes for the MAX7221 and MAX7219
// If these interfere, you might disable them by
// #define LedControlMatrixSkipOps
#ifndef LedControlMatrixSkipOps
#define LedControlMatrixSkipOps
#define OP_NOOP   0
#define OP_DIGIT0 1
#define OP_DIGIT1 2
#define OP_DIGIT2 3
#define OP_DIGIT3 4
#define OP_DIGIT4 5
#define OP_DIGIT5 6
#define OP_DIGIT6 7
#define OP_DIGIT7 8
#define OP_DECODEMODE  9
#define OP_INTENSITY   10
#define OP_SCANLIMIT   11
#define OP_SHUTDOWN    12
#define OP_DISPLAYTEST 15
#endif

class LedControlMatrix {
    private :
        /* The array for shifting the data to the devices */
        byte spidata[16];

    /*
       This protected modifier is the whole point of *copying* this otherwise fine
       library rather than just pulling it in as a library. Otherwise, derived
       classes cannot access spiTransfer and status, since they are *private*.

       Admittedly, state or methods may warrant some protection to maintain its
       integrity, for which *private* would be appropriate. Indeed, for *internal*
       state whose integrits must be guaranteed, private is appropriate and even
       required.

       But hey, we're programming a microcontroller here, not some teller machine
       or network security system where such guarantees might be in order!

       Be open for extension! Expect and allow for derived classes to be present!
       Prepare methods so that it becomes easy for the derived classes to
       modify individual aspects of the class' behaviour, small or large!
    */
    protected:
        /* Send out a single command to the device */
        void spiTransfer(int addr, byte opcode, byte data);

        /* We keep track of the led-status for all 8 devices in this array */
        byte canvas[64];

        /** Bit marker: which modules are dirty and need to be re-sent on refresh? */
        int dirtyDevices;

    private:
        /* Data is shifted out of this pin*/
        int SPI_MOSI;
        /* The clock is signaled on this pin */
        int SPI_CLK;
        /* This one is driven LOW for chip selectzion */
        int SPI_CS;
        /* The maximum number of devices we use */
        int maxDevices;

    public:
        /* 
         * Create a new controler 
         * Params :
         * dataPin		pin on the Arduino where data gets shifted out
         * clockPin		pin for the clock
         * csPin		pin for selecting the device 
         * numDevices	maximum number of devices that can be controled
         */
        LedControlMatrix(int dataPin, int clkPin, int csPin, int numDevices=1);

        /*
         * Gets the number of devices attached to this LedControl.
         * Returns :
         * int	the number of devices on this LedControl
         */
        int getDeviceCount();

        /* 
         * Set the shutdown (power saving) mode for the device
         * Params :
         * status	If true the device goes into power-down mode. Set to false
         *		for normal operation.
         */
        void shutdown(bool status);

        /* 
         * Set the brightness of the display.
         * intensity	the brightness of the display. (0 least .. 15 max)
         */
        void setIntensity(int intensity);

        /* 
         * Switch all Leds on the display off. 
         */
        void clearDisplay();

        /**
         * Send canvas data to the display.
         */
        void refresh();

        /**
         * Update canvas with spriteWidth columns starting at displayColumn.
         * Returns displayColumn plus spriteWidth.
         *
         * Use refresh() to really show the canvas data.
         */
        int bitBlt(int displayColumn, int spriteWidth, const byte *sprite);
};

#endif	//LedControlMatrix_h
