#include <SoftwareSerial.h>
#include <TinyGPSPlus.h>

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define GPS_SERIAL_BAUDRATE 38400 // gps baud rate is 38400
#define ARDU_BAUD_RATE 9600

#define OLED_I2C_ADDRESS 0x3C

SoftwareSerial mySerial(2, 3);
TinyGPSPlus gps;
SSD1306AsciiWire display;

bool found = false;

void setup() {
  mySerial.begin(GPS_SERIAL_BAUDRATE);
  Serial.begin(ARDU_BAUD_RATE);
  Serial.println("GPS start");

  display.begin(&Adafruit128x64, OLED_I2C_ADDRESS);
  display.setFont(Adafruit5x7);
  display.clear();
  display.setCursor(0, 0);
  display.println("Getting position...");
  delay(1000);
}

void loop() {
  // gather data from gps serial port
  while(mySerial.available()){
    gps.encode(mySerial.read());
    // Serial.write(mySerial.read());
  }

  // // check if data is ready and updated
  if (gps.location.isUpdated()) {
    if (!found) {
      display.clear();
      found = true;
    }

    char datetime[30];
    sprintf(
      datetime, 
      "%d-%d-%d %d:%d", // e.g. 2024-12-04 22:20
      gps.date.day(), 
      gps.date.month(), 
      gps.date.year(), 
      gps.time.hour(), 
      gps.time.minute()
    );

    display.setCursor(0, 0);
    display.println(datetime);

    display.setCursor(0, 10);
    display.print("lat=");
    display.println(String(gps.location.lat(), 6).c_str());

    display.setCursor(0, 20);
    display.print("long=");
    display.println(String(gps.location.lng(), 6).c_str());
  }

  if (gps.satellites.isUpdated()) {
    display.setCursor(0, 30);
    display.print("satellites=");
    display.println(String(gps.satellites.value()).c_str());
  }

  if (gps.speed.isUpdated()) {
    display.setCursor(0, 40);
    display.print("speed=");
    display.println(String(gps.speed.value(), 3).c_str());
  }

  if (gps.course.isUpdated()) {
    display.setCursor(0, 50);
    display.print("course=");
    display.print(String(gps.course.deg()).c_str());
    display.println("degrees");
  }

  if (gps.altitude.isUpdated()) {
    display.setCursor(0, 60);
    display.print("altitude=");
    display.print(String(gps.altitude.meters(), 2).c_str());
    display.println("m");
  }
}