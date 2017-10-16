#include <Adafruit_NeoPixel.h>
#include <SparkFunDS1307RTC.h>
#define DEBUG 1

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, A2, NEO_GRB + NEO_KHZ800);

uint8_t numbers[12][2], lastMinute;

uint32_t colors[8] = {
  pixels.Color(255, 0, 0),     //red
  pixels.Color(255, 65, 0),    //orange
  pixels.Color(255, 125, 0),   //yellow
  pixels.Color(0, 255, 0),     //green
  pixels.Color(0, 255, 255),   //teal
  pixels.Color(0, 0, 255),     //blue
  pixels.Color(100, 0, 255),    //purple
  pixels.Color(255, 255, 255)    //white
};

void setup() {
#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Clock");
#endif
  rtc.begin();
  rtc.autoTime();
  rtc.set12Hour();
#ifdef DEBUG
  Serial.println("RTC");
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

  lastMinute = 0;
  testLights(50);
}

void loop() {
  rtc.update();
  showTime();

  delay(1000);
}

void showTime() {
  if ( rtc.minute() != lastMinute) {
    uint32_t color;
    color = minuteToColor(rtc.minute());
    showNumber(rtc.hour(), color);
  }
}

void showNumber(uint8_t number, uint32_t color) {
  pixels.clear();
  uint8_t pixelNum;
  for ( int i = 0; i < 2; i++) {
    pixelNum = numbers[number - 1][i];
    pixels.setPixelColor(pixelNum, color);
  }
  pixels.show();
}

uint32_t minuteToColor(byte currentMinute) {
  uint32_t color;
  byte red, green, blue;

  // blue
  if ( currentMinute <= 40 ) {
    blue = mapbyte(currentMinute, 0, 40, 255, 0);
  }
  else {
    blue = 0;
  }

  // red
  if (currentMinute <= 30 ) {
    red = mapbyte(currentMinute, 0, 30, 0, 255);
  }
  else {
    red = mapbyte(currentMinute, 31, 59, 255, 0);
  }

  // green
  if ( currentMinute >= 20 ) {
    green = mapbyte(currentMinute, 20, 59, 0, 255);
  }
  else {
    green = 0;
  }

#ifdef DEBUG
  Serial.print("red: ");
  Serial.println(red);
  Serial.print("green: ");
  Serial.println(green);
  Serial.print("blue: ");
  Serial.println(blue);
#endif

  color = pixels.Color(red, blue, green);
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

byte mapbyte(byte x, byte in_min, byte in_max, byte out_min, byte out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
