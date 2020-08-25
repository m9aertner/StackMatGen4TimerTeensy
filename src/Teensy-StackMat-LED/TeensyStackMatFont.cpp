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

#include "TeensyStackMatFont.h"

/*
 * Encoding is 8 byte per character, only lower nibble used,
 * lower address represents lower row on the display,
 * least significant bit is rightmost bit on display.
 * Yes, with *my* display this needs a bit reversal step, but
 * I can't be bothered to re-code these (manually).
 */
const byte TeensyStackMatFont::DIGITS4[13 * 8] PROGMEM = {
    // 0
    B00000110,
    B00001001,
    B00001001,
    B00001001,
    B00001001,
    B00001001,
    B00001001,
    B00000110,
    // 1
    B00000010,
    B00000110,
    B00000010,
    B00000010,
    B00000010,
    B00000010,
    B00000010,
    B00000111,
    // 2
    B00000110,
    B00001001,
    B00000001,
    B00000010,
    B00000100,
    B00001000,
    B00001000,
    B00001111,
    // 3
    B00000110,
    B00001001,
    B00000001,
    B00000110,
    B00000001,
    B00000001,
    B00001001,
    B00000110,
    // 4
    B00001010,
    B00001010,
    B00001010,
    B00001111,
    B00000010,
    B00000010,
    B00000010,
    B00000010,
    // 5
    B00001111,
    B00001000,
    B00001000,
    B00001110,
    B00000001,
    B00000001,
    B00001001,
    B00000110,
    // 6
    B00000110,
    B00001001,
    B00001000,
    B00001110,
    B00001001,
    B00001001,
    B00001001,
    B00000110,
    // 7
    B00001111,
    B00000001,
    B00000001,
    B00000010,
    B00000010,
    B00000010,
    B00000010,
    B00000010,
    // 8
    B00000110,
    B00001001,
    B00001001,
    B00000110,
    B00001001,
    B00001001,
    B00001001,
    B00000110,
    // 9
    B00000110,
    B00001001,
    B00001001,
    B00000111,
    B00000001,
    B00000001,
    B00001001,
    B00000110,
    // :
    B00000000,
    B00000000,
    B00000010,
    B00000010,
    B00000000,
    B00000010,
    B00000010,
    B00000000,
    // .
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000010,
    // Space
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000};

const byte TeensyStackMatFont::DIGITS3[13 * 8] PROGMEM = {
    // 0
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000101,
    B00000101,
    B00000101,
    B00000111,
    // 1
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    // 2
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000001,
    B00000111,
    B00000100,
    B00000111,
    // 3
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000001,
    B00000111,
    B00000001,
    B00000111,
    // 4
    B00000000,
    B00000000,
    B00000000,
    B00000101,
    B00000101,
    B00000111,
    B00000001,
    B00000001,
    // 5
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000100,
    B00000111,
    B00000001,
    B00000111,
    // 6
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000100,
    B00000111,
    B00000101,
    B00000111,
    // 7
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000001,
    B00000001,
    B00000001,
    B00000001,
    // 8
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000101,
    B00000111,
    B00000101,
    B00000111,
    // 9
    B00000000,
    B00000000,
    B00000000,
    B00000111,
    B00000101,
    B00000111,
    B00000001,
    B00000111,
    // :
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000010,
    B00000000,
    B00000010,
    B00000000,
    // .
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000010,
    // Space
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000};

const byte TeensyStackMatFont::ANIMATION[13 * 8] PROGMEM = {
    // 0
    B00000000,
    B00000000,
    B00000000,
    B00000001,
    B00000010,
    B00001100,
    B00110000,
    B11000000,
    // 1
    B00000000,
    B00000000,
    B00000110,
    B00001001,
    B10010000,
    B01100000,
    B00000000,
    B00000000,
    // 2
    B00000000,
    B00000111,
    B00001111,
    B00011111,
    B10101111,
    B01001111,
    B00000111,
    B00000000,
    // 3
    B00000000,
    B11000000,
    B11011010,
    B11011011,
    B11011011,
    B11011010,
    B11000000,
    B00000000,
    // 4
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // 5
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // 6
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // 7
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // 8
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // 9
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // :
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // .
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    // Space
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000};

byte const *const TeensyStackMatFont::glyph(char c, int sz)
{
  const byte *font;
  if (sz == 2)
    font = ANIMATION;
  else if (sz == 1)
    font = DIGITS3;
  else
    font = DIGITS4;

  int n = 12;
  if (c >= '0' && c <= '9')
    n = c - '0';
  else if (c == ':')
    n = 10;
  else if (c == '.')
    n = 11;

  return font + n * 8;
}
