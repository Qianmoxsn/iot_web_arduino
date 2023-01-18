#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>

// lcd defination
#define COLUMS 16  // LCD columns
#define ROWS 2     // LCD rows
// pin defination
#define PIN_DHT 9   // DHT11 data pin
#define PIN_LED 8  // LED pin
#define PIN_ACON 11
#define PIN_ACOFF 12
// dhttype defination
#define DHTTYPE DHT11

unsigned long buttontime = 0;
unsigned long dhttime = 0;
unsigned long lcdtime = 0;
int readtime = -1;
int lastdata = 0;

LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14,
                      POSITIVE);
DHT dht(PIN_DHT, DHTTYPE);

SoftwareSerial s_Serial(3, 2);  // RX, TX

void lcdclearline(int line) {
  lcd.setCursor(0, line);
  for (size_t i = 0; i < COLUMS; i++) {
    lcd.print(" ");
  }
}

void setup() {
  // put your setup code here, to run once:
  // digitalWrite(PIN_ACON, HIGH);
  pinMode(PIN_ACON, INPUT_PULLUP);
  // digitalWrite(PIN_ACOFF, HIGH);
  pinMode(PIN_ACOFF, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(8, LOW);

  // serial begin
  Serial.begin(9600);
  s_Serial.begin(9600);
  // LCD begin
  lcd.begin(COLUMS, ROWS);
  // open lcd backlight
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Loading...");
  // dht begin
  dht.begin();
  delay(1000);
  lcd.clear();
}

void loop() {
  if (millis() - dhttime > 2000) {
    readtime++;
    // read dht11
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    dhttime = millis();
    if (readtime > 3 || readtime == -1) {
      Serial.println("[T]" + String(t, 1) + "," + String(h, 0));
      readtime = 0;
    }
    // print dht11 data on lcd
    lcd.setCursor(0, 0);
    lcd.print("T:");
    lcd.print(String(t, 1));
    lcd.print("C");
    lcd.setCursor(9, 0);
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
      Serial.println("[L]" + String(key2operation[0]));
      lcd.setCursor(0, 1);
      lcd.print("[L]AC ON");
      digitalWrite(PIN_LED, HIGH);
      lcdtime = millis();
      lastdata = 1;
    }
  }

  ////PIN 12 LOW,AC OFF
  if (digitalRead(PIN_ACOFF) == LOW && millis() - buttontime > 1000) {
    buttontime = millis();
    // AC OFF
    if (lastdata == 0) {
      Serial.println("[L]" + String(key2operation[1]));
      lcd.setCursor(0, 1);
      lcd.print("[L]AC OFF");
      digitalWrite(PIN_LED, LOW);
      lcdtime = millis();
      lastdata = 1;
    }
  }
  if (digitalRead(12) == HIGH && digitalRead(11) == HIGH) {
    lastdata = 0;
  }

  if (Serial.available() > 0)  // 判读是否串口有数据
  {
    String comdata = "";            // 缓存清零
    while (Serial.available() > 0)  // 循环串口是否有数据
    {
      comdata += char(Serial.read());  // 叠加数据到comdata
      delay(2);                        // 延时等待响应
    }
    if (comdata.length() > 0)  // 如果comdata有数据
    {
      comdata.replace("\r\n", "");  // 去掉回车符
      lcd.setCursor(0, 1);  // 设置光标位置
      if (comdata == (String)key2operation[0]) {
        lcd.print("[W]AC ON");  // 打印comdata数据
        digitalWrite(PIN_LED, HIGH);
      } else if (comdata == (String)key2operation[1]) {
        lcd.print("[W]AC OFF");  // 打印comdata数据
        digitalWrite(PIN_LED, LOW);
      } else {
        lcd.print("[err]"+comdata);  // 打印comdata数据
      }
      lcdtime = millis();
    }
  }

  if (millis() - lcdtime > 1000) {
    lcdclearline(1);
    lcdtime = millis();
  }
}
