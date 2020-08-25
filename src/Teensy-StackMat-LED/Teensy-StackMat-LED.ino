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

#include "LedControlMatrix.h"
#include "StackMatDecoderProxy.h"
#include "TeensyStackMatFont.h"

#define VERSION "v1.0"

#define NUMBER_OF_DISPLAYS 4 // We have 4 modules, i.e. a 4 x 8 x 8 display
#define PIN_DIN 14
#define PIN_CLK 16
#define PIN_CS 15
#define LED_BRIGHTNESS 1 // 0 weakest .. 15 brightest

// C++ requires declaration before use.
void enterAnimation();
void stepAnimation();
int onPacketReceived(char *telegram);
void onTimeChanged(char *telegram);

// Blinking LED if nothing else works...
int z = LOW;

LedControlMatrix lc = LedControlMatrix(PIN_DIN, PIN_CLK, PIN_CS, NUMBER_OF_DISPLAYS);
StackMatDecoder decoder = StackMatDecoderProxy(&onPacketReceived, &onTimeChanged);

// Show connect animation shortly after reset, unless a signal gets received earlier
#define TO_INITIAL_CONNECTED 500

// After successful reception of a packet, go to connect animation if no further
// sane packet gets received for this many milliseconds (15s)
#define TO_AFTER_PACKET (1000 * 15)

// Showing the connected animation. How fast? Scroll speed (milliseconds)
// Must be different from the other two Timeout values.
#define TO_STEP_ANIMATION 120

unsigned long delayToNextEvent = TO_INITIAL_CONNECTED;
elapsedMillis sinceLastEvent(0);
int animationDisplayCol = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.print(F("Teensy StackMat LED "));
  Serial.println(VERSION);

  /*
    The MAX72XX is in power-saving mode on startup,
    we have to do a wakeup call
  */
  lc.shutdown(false);
  lc.setIntensity(LED_BRIGHTNESS);
  lc.clearDisplay();

  // Serial2 is the connection from StackMat. TX is n/c, we're not sending, only receiving.
  // StackMat sends 2.7 volt square-wave signal 1200 baud 8N1, which is directly compatible
  // with the Teensy hardware UART. Still, expect glitches and incoming inaccurate bytes so
  // the protocol check byte and limited byte range per package byte makes sense.
  // See below for aggressive package decoding that skips to the next package if anything odd
  // is detected.
  //
  // The INV portion of constant SERIAL_8N1_RXINV denotes INVERSE handling of the serial line.
  // This may not be available on Arduinos when the hardware does not support INV.
  Serial2.begin(1200, SERIAL_8N1_RXINV);
}

void loop()
{
  z = (z == LOW) ? HIGH : LOW;
  digitalWrite(LED_BUILTIN, z);

  if (Serial2.available() > 0)
  {
    int b = Serial2.read();
    decoder.update(b);
    // The decoder will call back to below onPacketReceived and onChangedTime
    // where data is sent to the display and timeouts are being managed.
    // See StackMatDecoder::onChangedTime()
  }

  if (sinceLastEvent >= delayToNextEvent)
  {
    sinceLastEvent = 0;
    if (delayToNextEvent != TO_STEP_ANIMATION)
    {
      enterAnimation();
    }
    else
    {
      stepAnimation();
    }
  }
}

/**
 * Start the "connected" animation.
 */
void enterAnimation()
{
  delayToNextEvent = TO_STEP_ANIMATION;
  lc.clearDisplay();
  animationDisplayCol = 0;
}

/**
 * Perform a step of the "connected" animation.
 */
void stepAnimation()
{
  if (++animationDisplayCol >= 40 + 32)
  {
    animationDisplayCol = 0;
  }
  // Let the animation scroll into the display from left to right
  // We start at offset -47 (6 glyphs of size 8 each, one col visible)
  int x = animationDisplayCol - 6 * 8 + 1;
  x = lc.bitBlt(x, 8, TeensyStackMatFont::glyph(' ', 2));
  x = lc.bitBlt(x, 8, TeensyStackMatFont::glyph('0', 2));
  x = lc.bitBlt(x, 8, TeensyStackMatFont::glyph('1', 2));
  x = lc.bitBlt(x, 8, TeensyStackMatFont::glyph('1', 2));
  x = lc.bitBlt(x, 8, TeensyStackMatFont::glyph('2', 2));
  x = lc.bitBlt(x, 8, TeensyStackMatFont::glyph('3', 2));
  lc.refresh();
}

/**
 * A new packet has been decoded correctly.
 * Exit the animation if runing.
 */
int onPacketReceived(char *telegram)
{
  int r = 0;
  sinceLastEvent = 0;
  // When we may have shown some of the animation, clear that out
  // to make room for the time display.
  if (delayToNextEvent == TO_STEP_ANIMATION)
  {
    lc.clearDisplay();
    onTimeChanged(telegram);
    r = 1;
  }
  delayToNextEvent = TO_AFTER_PACKET;
  return r;
}

/**
 * A new packet has been decoded correctly and carries a time that
 * is different from the one seen in the previous packet that was
 * decoded correcly.
 */
void onTimeChanged(char *telegram)
{
  int x = 1;
  int m = telegram[1];
  if (m > '0')
  {
    // minutes
    x = lc.bitBlt(x, 4, TeensyStackMatFont::glyph(m));
    x = lc.bitBlt(x, 3, TeensyStackMatFont::glyph(':'));
  }
  else
  {
    x = lc.bitBlt(x, 7, TeensyStackMatFont::glyph(' '));
  }

  int s10 = telegram[2];
  if (s10 > '0' || m > '0')
  {
    // 10s
    x = lc.bitBlt(x, 4, TeensyStackMatFont::glyph(s10));
  }
  else
  {
    x = lc.bitBlt(x, 4, TeensyStackMatFont::glyph(' '));
  }

  // seconds
  x = lc.bitBlt(x, 5, TeensyStackMatFont::glyph(telegram[3]));
  x = lc.bitBlt(x, 3, TeensyStackMatFont::glyph('.'));

  // 1/10s
  x = lc.bitBlt(x, 3, TeensyStackMatFont::glyph(telegram[4], 1));
  // 1/100s
  x = lc.bitBlt(x, 4, TeensyStackMatFont::glyph(telegram[5], 1));
  // 1/1000s
  x = lc.bitBlt(x, 4, TeensyStackMatFont::glyph(telegram[6], 1));

  // Send updated screen to LED matrix once.
  lc.refresh();
}

// The End. Scramble. Repeat.
