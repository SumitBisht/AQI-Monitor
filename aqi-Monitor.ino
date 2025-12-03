#include <SPI.h>
#include <Wire.h>
#include "MQ135.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// The load resistance on the board
#define RLOAD 22.0
// Calibration resistance at atmospheric CO2 level
#define RZERO 889.13

#define SCREEN_WIDTH 128    // OLED display width, in pixels
#define SCREEN_HEIGHT 64    // OLED display height, in pixels
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup()
{
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
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
    delay(4000);
    MQ135 gasSensor = MQ135(A0);
    float air_quality = gasSensor.getPPM();
    float zero = gasSensor.getRZero();
    int val = analogRead(A0); 
    Serial.print ("raw = "); 
    Serial.println (val);
    Serial.print ("rzero: "); 
    Serial.println (zero); 
    Serial.print("Air Quality (PPM): ");
    Serial.println(air_quality);
    //// Display this in monitor
    display.clearDisplay();
    display.setCursor(40, 0);
    display.setTextSize(3);
    display.println("AQI");
    display.setCursor(0,30);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.print(val);
    display.setCursor(0,50);
    display.println("PPM (C02)");
    display.display();
}
