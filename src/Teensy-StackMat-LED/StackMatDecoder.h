/**

  StackMatGen4TimerTeensy - Teensy LC StackMat Display

*/

/**

  MIT License

  Copyright (c) 2020 Matthias Gaertner https://github.com/m9aertner

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/


#ifndef StackMatDecoder_h
#define StackMatDecoder_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class StackMatDecoder
{
protected:
    char telegram[12];

    // Pointer into telegram
    int z = 0;

    // Current packet: Running checksum and numeric value of current and previous packet.
    int chk;
    int n6;

    // Previous packet: numeric value, for change detection.
    int nPrev = -1;

public:
    /**
     * Feed the bytes that have been received from StackMat (or network, or...) in here,
     * one by one. Data type chosen to be compatible with read system call.
     */
    virtual void update(int fromStackMat);

protected:
    /**
     * Eventually, on successful packet detection, this method will be called.
     * The packet received will be in the "telegram" member.
     */
    virtual void onPacketReceived();

    /**
     * The default implementation of #onPacketReceived calls this method
     * whenever the time value has changed. Overriding this will minimize
     * the number of display updates.
     */
    virtual void onChangedTime();

    /**
     * Allow subclasses to get notified about decoding errors.
     */
    virtual void onDecodingError(int fromStackMat);
};

#endif // StackMatDecoder_h
