#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#include "Adafruit_Sensor.h"

#define COLUMS 16  // LCD columns
#define ROWS 2     // LCD rows
#define DHTPIN 9   // DHT11 data pin
#define DHTTYPE DHT11

unsigned long buttontime = 0;
int lastdata = 0;

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14,
                      POSITIVE);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  digitalWrite(11, HIGH);
  pinMode(11, INPUT_PULLUP);
  digitalWrite(12, HIGH);
  pinMode(12, INPUT_PULLUP);
  
  // serial begin
  Serial.begin(9600);
  // LCD begin
  lcd.begin(COLUMS, ROWS);
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HELLO");
}

void loop() {
  delay(3000);
  lcd.clear();

  // int data = dht.read();
  float t = dht.readTemperature();
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    Serial.print("TEMP:");
    Serial.print(t);
    Serial.println("C");
  }
  // lcd.setCursor(0, 0);
  // lcd.print("TEMP:");
  // lcd.print(t);
  // lcd.print("C");
  // lcd.setCursor(0, 1);
  // lcd.print("HUMI:");
  // lcd.print(h);
  // lcd.print("%");

  //"AC ON" : "5678" : 0,
  //"AC OFF" : "1234" : 1,
  int key2operation[2] = {5678, 1234};

  ////PIN 11 LOW,AC ON
  if (digitalRead(11) == LOW && millis() - buttontime > 1000) {
    buttontime = millis();
    // AC ON
    if (lastdata == 0) {
      Serial.println(key2operation[0]);
      lastdata = 1;
    }
  }

  ////PIN 12 LOW,AC OFF
  if (digitalRead(12) == LOW && millis() - buttontime > 1000) {
    buttontime = millis();
    // AC OFF
    if (lastdata == 0) {
      Serial.println(key2operation[1]);
      lastdata = 1;
    }
  }
  if (digitalRead(12) == HIGH && digitalRead(11) == HIGH) {
    lastdata = 0;
  }
}
