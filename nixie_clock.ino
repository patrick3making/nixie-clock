#include <Adafruit_NeoPixel.h>
#include <SparkFunDS1307RTC.h>
#define DEBUG 1
#define MINIMUM_LUX 15

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(24, A2, NEO_GRB + NEO_KHZ800);

uint8_t numbers[12][2], lastMinute;

void setup() {
  Serial.begin(9600);

  rtc.begin();
  rtc.set24Hour();
#ifdef DEBUG
  rtc.update();
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
  Serial.println("Welcome to the Nixie Clock");
  Serial.println("Enter \"?\" to print help message");
  printDate();
}

void loop() {
  rtc.update();
  showTime();
  if (Serial.available()) {
    menuSelect();
  }
  delay(1000);
}

void showTime() {
  int currentMinute = rtc.minute();
  if ( currentMinute != lastMinute) {
#ifdef DEBUG
    printTime();
#endif
    uint8_t currentHour = rtc.hour();
    uint32_t color;
    lastMinute = currentMinute;
    color = minuteToColor(currentMinute);
    if (currentHour > 12) {
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

  // green
  if ( currentMinute <= 40 ) {
    green = map(currentMinute, 0, 40, 255, MINIMUM_LUX);
  }
  else {
    green = MINIMUM_LUX;
  }

  // red
  if (currentMinute <= 29 ) {
    red = map(currentMinute, 0, 29, MINIMUM_LUX, 255);
  }
  else {
    red = map(currentMinute, 30, 59, 255, MINIMUM_LUX);
  }

  // blue
  if ( currentMinute >= 20 ) {
    blue = map(currentMinute, 20, 59, MINIMUM_LUX, 255);
  }
  else {
    blue = MINIMUM_LUX;
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

/*
   Menu Commands
*/

void printHelpMessage(){
  printTime();
  Serial.println("");
  Serial.println("Enter \"1\" to set time.");
  Serial.println("Enter \"2\" to set date");
  Serial.println("");
  Serial.println("!!! turn off all line endings !!!");
}

void menuSelect() {
  byte menuSelection = Serial.read();
  switch (menuSelection) {
    case '?':
      printHelpMessage();
      break;
    case '1':
      userSetTime();
      break;
    case '2':
      userSetDate();
      break;
    default:
      Serial.print(menuSelection);
      Serial.println(" is an unknown command");
      printHelpMessage();
      break;
  }
}

void printTime() {
  //get current time
  rtc.update();
  int s = rtc.second();
  int m = rtc.minute();
  int h = rtc.hour();

  //print time
  Serial.println("");
  Serial.print("Current time is: ");
  Serial.print(h);
  Serial.print(":");
  Serial.print(m);
  Serial.print(":");
  Serial.println(s);
}

void printDate() {
  //get current date
  rtc.update();
  int ddd = rtc.day();
  int d = rtc.date();
  int m = rtc.month();
  int y = rtc.year();
  //String dayString = getDayString(ddd);
  String dayString = dayIntToStr[ddd];

  //print date
  Serial.println("");
  Serial.print("Today's date is: ");
  Serial.print(dayString);
  Serial.print(" ");
  Serial.print(y);
  Serial.print("-");
  Serial.print(m);
  Serial.print("-");
  Serial.println(d);
}

void printDayStrings() {
  for (int i = 1; i < 8; i++) {
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(dayIntToStr[i]);
  }
}

void userSetTime() {
  Serial.println("Enter hour (24 hour format):");
  while (!Serial.available()) {
    delay(10);
  }
  int newHour = Serial.parseInt();
  Serial.println("Enter minute:");
  while (!Serial.available()) {
    delay(10);
  }
  int newMinute = Serial.parseInt();
  setRTCTime(newHour, newMinute);
}

void setRTCTime(int hour, int minute) {
  int day = rtc.day();
  int date = rtc.date();
  int month = rtc.month();
  int year = rtc.year();

  rtc.setTime(0, minute, hour, day, date, month, year);
  Serial.println("New time set");
  printTime();
}

void userSetDate() {
  Serial.println("Enter day of the week");
  printDayStrings();
  while (!Serial.available()) {
    delay(10);
  }
  int newDay = Serial.parseInt();

  Serial.println("Enter day of month:");
  while (!Serial.available()) {
    delay(10);
  }
  int newDate = Serial.parseInt();

  Serial.println("Enter month:");
  while (!Serial.available()) {
    delay(10);
  }
  int newMonth = Serial.parseInt();

  Serial.println("Enter year:");
  while (!Serial.available()) {
    delay(10);
  }
  int newYear = Serial.parseInt();

  setRTCDate(newDay, newDate, newMonth, newYear);
  printDate();
}

void setRTCDate(int day, int date, int month, int year) {
  int minute = rtc.minute();
  int hour = rtc.hour();

  rtc.setTime(0, minute, hour, day, date, month, year);
  Serial.println("New date set");
}

