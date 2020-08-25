/**

  StackMatGen4TimerTeensy - Teensy LC StackMat Display - LED version

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

#ifndef TeensyStackMatFont_h
#define TeensyStackMatFont_h

#include <avr/pgmspace.h>

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/**
 * A simple font on 4x8 pixels; only digits, colon, dot and space.
 */
class TeensyStackMatFont
{
public:
  static const byte DIGITS4[] PROGMEM;
  static const byte DIGITS3[] PROGMEM;
  static const byte ANIMATION[] PROGMEM;

  /**
   * Return pointer to 8-byte glyph based on ASCII input c.
   * Return DIGITS4 (0, default), DIGITS3 (1) or ANIMATION (2) font.
   */
  static byte const *const glyph(char c, int sz = 0);
};

#endif // TeensyStackMatFont_h
