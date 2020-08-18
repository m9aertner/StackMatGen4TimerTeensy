/**

  StackMatGen4TimerTeensy - Teensy LC StackMat Display

  Connect your StackMat Gen4 Timer to a Teensy LC, with NO further components other than
  the 2,5mm connector cable that plugs into the StackMat timer.

  Teensy can read the StackMat signals through its built-in UART, thanks to the INVERSE
  line mode that is available in Teensy's UART. Note this mode is not available on
  plain AVR Arduinos so I believe one has to resort to an external inverter there, or
  use one of the bit-banging serial reception libraries. Anyway, this is about Teensy
  here!
 
  StackMat Gen4
    - 2,5mm TRS
    - tip is signal active high ca. 2,7V
    - ring is n/c
    - sleeve is ground
  Teensy Pin 9 Rx2

  Note that at least on my StackMat unit, I found that the ring does NOT carry the signal!
  If things do not work for you, try connecting to the other contact, or simply connect the contacts.

  As an aside, if you want to connect such Gen4 StackMat to a MacBook Pro's 3,5mm TRRS headset/audio
  connector, Sleeve is signal, and the ring next to S is ground. You may need to connect a 2k2Ohm
  resistor across those two points for the Mac to recognize the "Microphone". With this resistor,
  which may well be hidden in the plug/socket on either side of your DIY adapter cable, it will NOT
  be required to use an USB Audio adapter, which is suggested by many YT videos.

  https://www.pjrc.com/store/teensylc.html
 
  https://github.com/m9aertner/StackMatGen4TimerTeensy/StackMatGen4-Timer.md
  

  Teensy checks the incoming packets and displays the time portion (6 digits for Gen4)
  on a small 128x64 pixel OLED that's attached via I2C, again using built-in hardware
  I2C. A small limitatiof Teensy boards is that it is recommended to use pullup
  resistors on the SCL and SDA lines, since the built-in ones are unsuitable.

  Connection of the SSD_1306-based OLED to Teensy is as follows:
  GND - GND
  VCC - 3V3
  SCL - Pin 19, with pull-up to 3V3, any value from 2k2Ohm to 10kOhm should work
  SDA - Pin 18, with pull-up to 3V3, any value from 2k2Ohm to 10kOhm should work

  https://www.az-delivery.de/en/products/0-96zolldisplay

  To compile this software and load it into your Teensy, please see "Teensyduino" at

  https://www.pjrc.com/teensy/td_download.html (v1.53 at the time of this writing)

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

#include <Wire.h>
#include <Adafruit_GFX.h>      // v1.9.0 at the time of writing
#include <Adafruit_SSD1306.h>  // v2.3.1 at the time of writing

#define VERSION "v1.0"

#define SCREEN_WIDTH  128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// StackMat Package Decoder State

// Assembly area for packet received from StackMap
// Eventual content will be e.g.
//   0x49, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x00
// Terminating zero byte added to allow for transmission via Serial.println
// Trailing chk byte and LF CR will not be stored
//   0x40, 0x0A, 0x0D
static char telegram[12];

// Pointer into telegram
int z = 0;

// Current packet: Running checksum and numeric value of current and previous packet.
int chk;
int n6;

// Previous packet: numeric value, for change detection.
int nPrev = -1;


void setup() {
  // This is Teensy's USB serial connection to the computer (if any).
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

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

  delay(500);

  // Display a greeting until first StackMap packet is received successfully.
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(12, 10);
  display.println(F("Teensy StackMat"));
  display.setCursor(12, 24);
  display.println(VERSION);
  display.setCursor(12, 48);
  display.println(F("Go for that PB!")); // :-)
  display.display();
}

void loop() {
  if (Serial2.available() > 0) {
    int b = Serial2.read();
    update(b);
  }
}

void update(int b) {
  const int zIn = z;

  if (z == 0) {
    if (b == 'I' || b == 'A' || b == ' ' || b == 'S' || b == 'R' || b == 'L' || b == 'C' ) {
      telegram[z++] = b;
      chk = 64;
      n6 = 0;
    }
  } else if (z <= 6) {
    if (b >= '0' && b <= '9') {
      telegram[z++] = b;
      int d = b - '0';
      chk += d;
      n6 = n6 * 10 + d;
    }
  } else if (z == 7) {
    // Check digit
    if (b == chk) {
      telegram[z++] = 0;
    }
  } else if (z == 8) {
    if (b == 0x0A) {
      z++;
    }
  } else if (z == 9) {
    if (b == 0x0D) {
      z++;
    }
  }
  if (z >= 10) {
    packetReceived();
    z = 0;
  } else if (zIn >= z) {
    // No increase? Unexpected character and/or bad checksum.
    // Note that this happens quite frequently. DC-coupled transmission is prone to
    // interferences, even over short distances like in this use case.
    // Reset to zero, wait for next packet.
    z = 0;
  }
}

/**
 * A packet has been received and passed the syntactic checks.
 *
 * The telegram array contains the status indicator at offset 0.
 * At offsets 1..7 inclusive there are the 6 ASCII digits that represent
 * the time shown on the timer display, in most of the timer states.
 *
 * This method sends the received time to the display, formatted with
 * a colon and dot as minutes / seconds / thousands separators, respectively.
 *
 * The display is updated only when the value has changed, which reduces flicker.
 */
void packetReceived() {
  if (nPrev != n6) {
    nPrev = n6;

    // Not sure why this is needed, but otherwise the display will scroll right by one pixel on each update?
    display.stopscroll();

    int x = 0;
    int y = 20;
    // Set to 1 for no leading zeroes, some may like that better.
    // In this case, we should also center the output on screen.
    int dropLeadingZero = 0;

    // Clear out the time section of the display.
    // Better than clearing the whole display, which may cause flicker on some pixels.
    display.fillRect(x, y, SCREEN_WIDTH - x - 1, 36, BLACK);
    display.setTextSize(3);

    // Minute
    display.setCursor(x, y);
    dropLeadingZero = showDigit(dropLeadingZero, telegram[1]);
    display.setCursor(x += 12, y);
    dropLeadingZero = showDigit(dropLeadingZero, ':');

    // Seconds
    display.setCursor(x += 12, y);
    showDigit(dropLeadingZero, telegram[2]);
    showDigit(0, telegram[3]);

    // Thousands
    display.setCursor(x += 40, y);
    showDigit(0, telegram[4]);
    showDigit(0, telegram[5]);
    showDigit(0, telegram[6]);

    // Let's draw the thousands separator dot as a few discrete pixels.
    // Plain dot '.' output is too thick for my liking.
    display.fillRect(x - 5, y + 18, 3, 3, WHITE);

    display.display();

    // Output ASCII telegram to Arduino Serial Monitor via USB
    // Format is "I000000", no check byte, CRLF added from println, not LFCR from packet
    Serial.println(telegram);
  }
}

/**
 * Output character c to display context. Handle leading digit suppression.
 */
int showDigit(int dropLeadingZero, char c) {
  if (c == '0' && dropLeadingZero) {
    c = ' ';
  } else if (c == ':' && dropLeadingZero) {
    c = ' ';
  } else {
    dropLeadingZero = 0;
  }
  display.print(c);
  return dropLeadingZero;
}

// The End. Scramble. Repeat.
