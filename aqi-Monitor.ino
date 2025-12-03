#include <ESP8266WiFi.h>
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

String apiKey = "xxxxxx"; // Enter your Write API key from ThingSpeak
const char *ssid = "xxxxxx";  // Replace with your wifi ssid and wpa2 key
const char *pass = "xxxxxx";   // Replace with your wifi password
const char* server = "api.thingspeak.com";
WiFiClient wifiClient;

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

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(2000);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
}

void loop() {
    delay(2000);
    MQ135 gasSensor = MQ135(A0);
    float ppm = gasSensor.getPPM();
    float zero = gasSensor.getRZero();
    int val = analogRead(A0); 
    Serial.print ("raw = "); 
    Serial.println (val);
    Serial.print ("rzero: "); 
    Serial.println (zero); 
    Serial.print("Air Quality (PPM): ");
    Serial.println(ppm);
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
    publishExternally(val);
}

void publishExternally(int sensorValue) {
  if (wifiClient.connect(server, 80)) {
    String payload = apiKey + "&field1="+String(sensorValue);
    wifiClient.print("POST /update HTTP/1.1\n");
    wifiClient.print("Host: api.thingspeak.com\n");
    wifiClient.print("Connection: close\n");
    wifiClient.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    wifiClient.print("Content-Type: application/x-www-form-urlencoded\n");
    wifiClient.print("Content-Length: ");
    wifiClient.print(payload.length());
    wifiClient.print("\n\n");
    wifiClient.print(payload);

  }
  wifiClient.stop();
  delay(1000);
}
