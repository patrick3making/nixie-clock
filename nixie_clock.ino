#include <Adafruit_NeoPixel.h>
#include <SparkFunDS1307RTC.h>
#define DEBUG 1
#define MINIMUM_LUX 15

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, A2, NEO_GRB + NEO_KHZ800);

uint8_t numbers[12][2], lastMinute;

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Welcome to the Nixie Clock");
#endif
  rtc.begin();
  //setRTCTime();
  rtc.set12Hour();
#ifdef DEBUG
  rtc.update();
  Serial.println("RTC Setup");
  Serial.print("  12 hour mode: ");
  Serial.println(rtc.is12Hour());
  Serial.print("  Current Time: ");
  Serial.println(rtc.hour());
  Serial.print(":");
  Serial.println(rtc.minute());
#endif
  pixels.begin();
  pixels.clear();
  pixels.show();
#ifdef DEBUG
  Serial.println("LEDs");
#endif

  for ( uint8_t h = 1; h <= 12; h++ ) {
    numbers[h - 1][0] = 2 * (13 - h) - 2;
    numbers[h - 1][1] = 2 * (13 - h) - 1;
  }

  lastMinute = 70;
  testLights(30);
}

void loop() {
  rtc.update();
  showTime();

  delay(1000);
}

void showTime() {
  int currentMinute = rtc.minute();
  if ( currentMinute != lastMinute) {
#ifdef DEBUG
    Serial.print("Time to display: ");
    Serial.print(rtc.hour());
    Serial.print(":");
    Serial.println(rtc.minute());
#endif
    uint8_t currentHour = rtc.hour();
    uint32_t color;
    lastMinute = currentMinute;
    color = minuteToColor(currentMinute);
    if (currentHour > 12) {
#ifdef DEBUG
      Serial.println("Hour over 12");
#endif
      currentHour -= 12;
    }
    showNumber(currentHour, color);
  }
}

void showNumber(uint8_t number, uint32_t color) {
  pixels.clear();
  int pixelNum;
  for ( uint8_t i = 0; i < 2; i++) {
    pixelNum = numbers[number - 1][i];
    pixels.setPixelColor(pixelNum, color);
  }
  pixels.show();
}

uint32_t minuteToColor(byte currentMinute) {
  uint32_t color;
  byte red, green, blue;

  // blue
  if ( currentMinute <= 45 ) {
    blue = map(currentMinute, 0, 45, 255, MINIMUM_LUX);
  }
  else {
    blue = MINIMUM_LUX;
  }

  // red
  if (currentMinute <= 29 ) {
    red = map(currentMinute, 0, 29, MINIMUM_LUX, 255);
  }
  else {
    red = map(currentMinute, 30, 59, 255, MINIMUM_LUX);
  }

  // green
  if ( currentMinute >= 15 ) {
    green = map(currentMinute, 15, 59, MINIMUM_LUX, 255);
  }
  else {
    green = MINIMUM_LUX;
  }

#if DEBUG >= 2
  Serial.print("red: ");
  Serial.println(red);
  Serial.print("green: ");
  Serial.println(green);
  Serial.print("blue: ");
  Serial.println(blue);
#endif

  color = pixels.Color(red, green, blue);
  return color;
}

void testLights(uint8_t dwell) {
  uint8_t number;
  uint32_t color;
  for (int t = 0; t < 60; t++) {
#ifdef DEBUG
    Serial.print("*** T ");
    Serial.println(t);
#endif
    color = minuteToColor(t);
    number = map(t, 0, 59, 1, 12);
    showNumber( number, color);

    delay(dwell);
  }
}

void setRTCTime() {
  int hour = 18;
  int minute = 12;
  int second = 00;
  int day = 6; // Sunday=1, Monday=2, ..., Saturday=7.
  int date = 2;
  int month = 2;
  int year = 18;

  rtc.setTime(second, minute, hour, day, date, month, year);
}

