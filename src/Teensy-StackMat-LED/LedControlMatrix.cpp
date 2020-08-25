/*
 *    LedControlMatrix.cpp - A library for controlling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2020 Matthias Gaertner
 *
 *    based on
 *    https://github.com/wayoda/LedControl/commit/f86a18493e9594d7b7198bf894f529a1c79d5802
 *
 *    LedControl.cpp - A library for controling Leds with a MAX7219/MAX7221
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

#include "LedControlMatrix.h"

LedControlMatrix::LedControlMatrix(int dataPin, int clkPin, int csPin, int numDevices)
{
    SPI_MOSI = dataPin;
    SPI_CLK = clkPin;
    SPI_CS = csPin;
    if (numDevices <= 0 || numDevices > 8)
        numDevices = 8;
    maxDevices = numDevices;
    pinMode(SPI_MOSI, OUTPUT);
    pinMode(SPI_CLK, OUTPUT);
    pinMode(SPI_CS, OUTPUT);
    digitalWrite(SPI_CS, HIGH);
    dirtyDevices = 0;

    //we go into shutdown-mode on startup
    shutdown(true);
    for (int i = 0; i < 64; i++)
        canvas[i] = 0x00;
    for (int i = 0; i < maxDevices; i++)
    {
        spiTransfer(i, OP_DISPLAYTEST, 0);
        //scanlimit is set to max on startup
        spiTransfer(i, OP_SCANLIMIT, 7);
        //decode is done in source
        spiTransfer(i, OP_DECODEMODE, 0);
    }
    clearDisplay();
}

int LedControlMatrix::getDeviceCount()
{
    return maxDevices;
}

void LedControlMatrix::shutdown(bool b)
{
    for (int addr = 0; addr < maxDevices; addr++)
        if (b)
            spiTransfer(addr, OP_SHUTDOWN, 0);
        else
            spiTransfer(addr, OP_SHUTDOWN, 1);
}

void LedControlMatrix::setIntensity(int intensity)
{
    if (intensity >= 0 && intensity < 16)
        for (int addr = 0; addr < maxDevices; addr++)
        {
            spiTransfer(addr, OP_INTENSITY, intensity);
        }
}

void LedControlMatrix::clearDisplay()
{
    static const byte ZERO[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    for (int addr = 0; addr < maxDevices; addr++)
    {
        bitBlt(addr * 8, 8, ZERO);
    }
    refresh();
}

void LedControlMatrix::spiTransfer(int addr, volatile byte opcode, volatile byte data)
{
    //Create an array with the data to shift out
    int offset = addr * 2;
    int maxbytes = maxDevices * 2;

    for (int i = 0; i < maxbytes; i++)
        spidata[i] = (byte)0;
    //put our device data into the array
    spidata[offset + 1] = opcode;
    spidata[offset] = data;
    //enable the line
    digitalWrite(SPI_CS, LOW);
    //Now shift out the data
    for (int i = maxbytes; i > 0; i--)
        shiftOut(SPI_MOSI, SPI_CLK, MSBFIRST, spidata[i - 1]);
    //latch the data onto the display
    digitalWrite(SPI_CS, HIGH);
}

byte bitreverse(byte x)
{
    x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
    x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
    x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
    return x;
}

void LedControlMatrix::refresh()
{
    int addr = 0;
    while (dirtyDevices && addr < maxDevices)
    {
        if (dirtyDevices & 1)
        {
            int offset = addr * 8;
            for (int row = 0; row < 8; row++)
            {
                spiTransfer(addr, row + 1, canvas[offset + row]);
            }
        }
        dirtyDevices /= 2;
        addr++;
    }
    dirtyDevices = 0;
}

int LedControlMatrix::bitBlt(const int displayColumn, const int spriteWidth, const byte *const sprite)
{
    // We can have negative displayColumn here as long as at least one column of the sprite
    // replaces display column 0.
    if (spriteWidth <= 0 ||
        spriteWidth > 8)
    {
        return displayColumn;
    }
    if (displayColumn + spriteWidth < 1 ||
        displayColumn >= maxDevices * 8)
    {
        return displayColumn + spriteWidth;
    }
    int dc8 = (8 + displayColumn) % 8;
    int addr = displayColumn / 8;
    if (spriteWidth == 8 && dc8 == 0)
    {
        // Easy case: full bytes and aligned displayColumn
        // offset == displayColumn since displayColumn % 8 == 0
        for (int row = 0; row < 8; row++)
        {
            byte s = bitreverse(pgm_read_byte_near(sprite + 7 - row));
            canvas[displayColumn + row] = s;
        }
        dirtyDevices |= (1 << addr);
    }
    else if (spriteWidth > 0 && spriteWidth <= 8)
    {
        // Which devices do we address? Leftmost is 0, ...
        // We may have one or two devices that are relevant.
        // One iff displayColumn % 8 + spriteWidth <= 8.
        // Two otherwise. We can handle both cases with the same code.
        // Example: displayColumn == 14: addr := 1, dc8 := 6
        int addr = displayColumn / 8;
        int dirtyOneOrTwo = (dc8 + spriteWidth <= 8) ? B01 : B11;
        dirtyDevices |= (dirtyOneOrTwo << addr);

        // Deal with all eight rows one by one
        for (int row = 0; row < 8; row++)
        {
            // Offset into canvas for left device/right devices. Right device is just
            // 8 bytes apart from offset of left device.
            // Note that offsetRow0 could be negative. Note that offsetRow8 may exceed
            // the canvas array size. This is checked when we write back the result -
            // or not in the cases that the sprite "flows into" the display from the left,
            // or "flows out" of the display on the right.
            int offsetRow0 = displayColumn - dc8 + row;
            int offsetRow8 = offsetRow0 + 8;
            // Put the existing canvas bytes in an int. We assume that LSB is *left*
            // on the display rows. While this works on MY displays, this may not be
            // the case for YOUR displays, e.g. with different MAX72XX wiring.
            // So, we put the left device's byte into the least significant int byte,
            // and the right device's byte into the next more significant int byte.
            // Example: Display shows
            // LED: -##-#-#-----###-, st will be 0x7056
            int st = canvas[(sizeof(canvas) + offsetRow0) % sizeof(canvas)] + //
                     canvas[offsetRow8 % sizeof(canvas)] * 256;
            // Now we will erase the bits that will be replaced by sprite
            // Example: spriteWidth == 5, sm will be ooooooooooo11111
            int sm = (1 << spriteWidth) - 1;
            // Where in st are these bits located?
            // Exampe: sr will be ooooo11111oooooo (0x07C0)
            int sr = sm << dc8;
            // Erase them
            // Example:
            // ~sr: 11111ooooo111111
            //  st: 0111000001010110
            //  so: 0111000000010110
            int so = st & ~sr;

            // Pull the sprite byte, bottom row first
            // Example: sprite data ##--#-##, reversed 11o1oo11, relevant -#-##
            int s = 0xFF & bitreverse(pgm_read_byte_near(sprite + 7 - row));
            // Remove irrelevant bits
            // Example: s shifted by three ooo11o1o masked with ooo11111 is ooo11010
            s = sm & (s >> (8 - spriteWidth));

            // then shift and insert
            // Example:
            //  so: 0111000000010110
            //  s<: -----11010------ shifted left by 6
            so |= s << dc8;
            //  so: 0111011010010110
            // LED: -##-#-#-----###-, st will be 0x7056
            // LED: -##-#--#-##-###- (<-> LSB-left wiring)
            //      ^^^^^^     ^^^^^
            //      !!!!!!^^^^^+++++--- replaced with relevant sprite data -#-##
            //      !!!!!!     !!!!!
            //      \\\\\\_____\\\\\___ original active LEDs

            // Write back the two so bytes into the canvas buffer at the same
            // locations where they came from.

            // Do not write back beyond the
            // boundaries of the array.
            if (offsetRow0 >= 0)
            {
                canvas[offsetRow0] = so;
            }
            if (offsetRow8 < (int)sizeof(canvas))
            {
                canvas[offsetRow8] = so >> 8;
            }
        }
    }
    return displayColumn + spriteWidth;
}
