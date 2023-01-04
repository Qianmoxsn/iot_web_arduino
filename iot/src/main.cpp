#include <Arduino.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
//lcd defination
#define COLUMS 16  // LCD columns
#define ROWS 2     // LCD rows
//pin defination
#define PIN_DHT 9   // DHT11 data pin
#define PIN_ACON 11
#define PIN_ACOFF 12
//dhttype defination
#define DHTTYPE DHT11

unsigned long buttontime = 0;
unsigned long dhttime = 0;
unsigned long lcdtime = 0;
int lastdata = 0;

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14,
                      POSITIVE);
DHT dht(PIN_DHT, DHTTYPE);

void lcdclearline(int line) {
  lcd.setCursor(0, line);
  for (size_t i = 0; i < COLUMS; i++) {
    lcd.print(" ");
  }
}

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
  //open lcd backlight
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("HELLO");
  // dht begin
  dht.begin();
  delay(1000);
}

void loop() {
  if (millis() - dhttime > 2000) {
    //read dht11
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    dhttime = millis();
    // print dht11 data on lcd
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(String(t, 1));
    lcd.print("C");
    lcd.setCursor(8, 0);
    lcd.print("H:");
    lcd.print(String(h, 0));
    lcd.print("%");
  }

  //"AC ON" : "5678" : 0,
  //"AC OFF" : "1234" : 1,
  int key2operation[2] = {5678, 1234};

  ////PIN 11 LOW,AC ON
  if (digitalRead(PIN_ACON) == LOW && millis() - buttontime > 1000) {
    buttontime = millis();
    // AC ON
    if (lastdata == 0) {
      Serial.println(key2operation[0]);
      lcd.setCursor(0, 1);
      lcd.print("AC ON");
      lcdtime = millis();
      lastdata = 1;
    }
  }

  ////PIN 12 LOW,AC OFF
  if (digitalRead(PIN_ACOFF) == LOW && millis() - buttontime > 1000) {
    buttontime = millis();
    // AC OFF
    if (lastdata == 0) {
      Serial.println(key2operation[1]);
      lcd.setCursor(0, 1);
      lcd.print("AC OFF");
      lcdtime = millis();
      lastdata = 1;
    }
  }
  if (digitalRead(12) == HIGH && digitalRead(11) == HIGH) {
    lastdata = 0;
  }
  if (millis() - lcdtime > 1000) {
    lcdclearline(1);
    lcdtime = millis();
  }
}
