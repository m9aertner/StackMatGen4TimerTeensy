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

#include "StackMatDecoder.h"

/**
 * Feed the bytes that have been received from StackMat (or network, or...) in here,
 * one by one. Data type chosen to be compatible with read system call.
 */
void StackMatDecoder::update(int b)
{
  const int zIn = z;

  if (z == 0)
  {
    if (b == 'I' || b == 'A' || b == ' ' || b == 'S' || b == 'R' || b == 'L' || b == 'C')
    {
      telegram[z++] = b;
      chk = 64;
      n6 = 0;
    }
  }
  else if (z <= 6)
  {
    if (b >= '0' && b <= '9')
    {
      telegram[z++] = b;
      int d = b - '0';
      chk += d;
      n6 = n6 * 10 + d;
    }
  }
  else if (z == 7)
  {
    // Check digit
    if (b == chk)
    {
      telegram[z++] = 0;
    }
  }
  else if (z == 8)
  {
    if (b == 0x0A)
    {
      z++;
    }
  }
  else if (z == 9)
  {
    if (b == 0x0D)
    {
      z++;
    }
  }
  if (z >= 10)
  {
    onPacketReceived();
    z = 0;
  }
  else if (zIn >= z)
  {
    // No increase? Unexpected character and/or bad checksum.
    // Note that this happens quite frequently. DC-coupled transmission is prone to
    // interferences, even over short distances like in this use case.
    // Reset to zero, wait for next packet.
    onDecodingError(b);
    z = 0;
  }
}

/**
 * A new packet has been decoded correctly, including check digits.
 * Default: Detect changed time and call out to #onChangedTime if so.
 */
void StackMatDecoder::onPacketReceived()
{
  if (nPrev != n6)
  {
    nPrev = n6;
    onChangedTime();
  }
}

/**
 * A new packet has been decoded correctly and carries a time that
 * is different from the one seen in the previous packet that was
 * decoded correcly.
 * Default: output telegram to Serial
 */
void StackMatDecoder::onChangedTime()
{
  Serial.println(telegram);
}

/**
 * A decoding error has occured.
 * Default: do nothing
 * Param: the value supplied to update
 */
void StackMatDecoder::onDecodingError(int b)
{
}