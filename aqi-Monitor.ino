#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  delay(10);

  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.display();
  Serial.println("");
  Serial.println("Initializing");
  
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Starting");
  display.display();
  delay(4000);
}

void loop() {
    delay(2000);
    MQ135 gasSensor = MQ135(A0);
    float air_quality = gasSensor.getPPM();
    Serial.print("Air Quality (PPM): ");
    Serial.println(air_quality);
    display.clearDisplay();
    display.setCursor(40, 0);
    display.setTextSize(3);
    display.println("AQI");
    display.setCursor(0,30);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print(air_quality);
    display.setCursor(0,50);
    display.println("PPM (C02)");
    display.display();
    delay(1000);
}
