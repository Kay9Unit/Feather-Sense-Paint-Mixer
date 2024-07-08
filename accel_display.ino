/**
This is free and unencumbered software released into the public domain.

Anyone is free to copy, modify, publish, use, compile, sell, or
distribute this software, either in source code form or as a compiled
binary, for any purpose, commercial or non-commercial, and by any
means.

In jurisdictions that recognize copyright laws, the author or authors
of this software dedicate any and all copyright interest in the
software to the public domain. We make this dedication for the benefit
of the public at large and to the detriment of our heirs and
successors. We intend this dedication to be an overt act of
relinquishment in perpetuity of all present and future rights to this
software under copyright law.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

For more information, please refer to <https://unlicense.org>

2024 MATTHEW WALLACE
Adafruit Feather Sense NeoPixel "Paint Mixer"
*/

#include <Adafruit_LSM6DS3TRC.h>
#include <Adafruit_LSM6DS33.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <math.h>

Adafruit_LSM6DS3TRC accelerometer;
Adafruit_NeoPixel neoPixel = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);
Adafruit_SH1107 display = Adafruit_SH1107(64, 128, &Wire);

void setup(void)
{
  Serial.begin(115200);

  neoPixel.begin();
  neoPixel.clear();
  neoPixel.setBrightness(20);
  neoPixel.show();

  accelerometer.begin_I2C();
  display.begin(0x3C, true);
  display.clearDisplay();
  display.setRotation(1);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.display();
}

float highestAccel = 0;

void loop(void)
{
  // read data
  float accel_x;
  float accel_y;
  float accel_z;
  accelerometer.readAcceleration(accel_x, accel_y, accel_z);

  // begin display
  display.clearDisplay();
  display.setCursor(0, 0);

  display.print("Accel X: ");
  display.print(accel_x);
  display.print("\nAccel Y: ");
  display.print(accel_y);
  display.print("\nAccel Z: ");
  display.print(accel_z);

 // debugging purposes
  if (accel_y < highestAccel)
    highestAccel = accel_y;
  
  display.print("\nAccel Y Min: ");
  display.print(highestAccel);

  // accel values (seem to) range from -4 to 4
  // so we normalize them and multiply by
  // max color value of 255.
  // abs: negative values will also display
  // the color the same as positive
  accel_x = fabs(accel_x/4) * 255.0f;
  accel_y = fabs(accel_y/4) * 255.0f;
  accel_z = fabs(accel_z/4) * 255.0f;

  // apply colors
  neoPixel.setPixelColor(0, accel_x, accel_y, accel_z);
  neoPixel.show();

  // finish up, and actually display
  delay(10);
  yield();
  display.display();
}