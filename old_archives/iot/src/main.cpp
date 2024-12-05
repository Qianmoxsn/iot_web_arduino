#include <Adafruit_Sensor.h>
#include <Arduino.h>
#include <DHT.h>
// #include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "gprs.h"

// lcd defination
// #define COLUMS 16  // LCD columns
// #define ROWS 2     // LCD rows
// pin defination
#define PIN_DHT A2   // DHT11 data pin
#define PIN_LED 13  // LED pin
// #define PIN_ACON 11
// #define PIN_ACOFF 12
// dhttype defination
#define DHTTYPE DHT11

unsigned long buttontime = 0;
unsigned long dhttime = 0;
unsigned long lcdtime = 0;
unsigned long gprstime = 0;
int readtime = -1;
int lastdata = 0;

char gprsBuffer[64];
int inComing = 1;

// LiquidCrystal_I2C lcd(PCF8574_ADDR_A21_A11_A01, 4, 5, 6, 16, 11, 12, 13, 14,
//                       POSITIVE);
DHT dht(PIN_DHT, DHTTYPE);
GPRS gprs;

// SoftwareSerial s_Serial(3, 2);  // RX, TX

// void lcdclearline(int line) {
//   lcd.setCursor(0, line);
//   for (size_t i = 0; i < COLUMS; i++) {
//     lcd.print(" ");
//   }
// }
bool gprsInit() {
  gprs.preInit();
  int i = 0;

  // 初始化GPRS模块
  while (0 != gprs.init()) {
    delay(500);
    Serial.println("Init error");
    i++;
    if (i >= 10) {
      return false;
    }
  }

  // 连接TCP，进行简单测试后关闭
  while (0 != gprs.connectTCP("82.157.254.205", 28801)) { // 替换为您的服务器IP和端口
    Serial.println("Connect error");
    i++;
    delay(2000);
    if (i >= 3) {
      break;
    }
  }
  gprs.closeTCP();
  Serial.println("GPRS connect OK");

  i = 0;
  // 入网
  while (!gprs.join()) {
    Serial.println("GPRS join network error");
    delay(2000);
    i++;
    if (i >= 5) {
      return false;
    }
  }

  // DHCP成功
  Serial.print("IP Address is ");
  Serial.println(gprs.getIPAddress());
  Serial.println("Init success, start to connect platform...");
  return true;
}

void setup() {
  // put your setup code here, to run once:
  // digitalWrite(PIN_ACON, HIGH);
  // pinMode(PIN_ACON, INPUT_PULLUP);
  // // digitalWrite(PIN_ACOFF, HIGH);
  // pinMode(PIN_ACOFF, INPUT_PULLUP);
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);
  // digitalWrite(LED_BUILTIN, LOW);

  // serial begin
  Serial.begin(9600);
  // s_Serial.begin(9600);
  Serial.println("Start Init");
  // LCD begin
  // lcd.begin(COLUMS, ROWS);
  // // open lcd backlight
  // lcd.backlight();
  // lcd.setCursor(0, 0);
  // lcd.print("Loading...");
  // dht begin
  dht.begin();
  delay(5000);
  // lcd.clear();

  if (!gprsInit()) {
    Serial.println("GPRS Initialization failed.");
    while (1);
  }
  else {
    Serial.println("GPRS Initialization successful.");
  }

  String identify = gprs.getIdentify(); // 获取模块标识
  Serial.print("Identify: ");
  Serial.println(identify);
}

void loop() {
  // Persudo Global Variables
  float t = 0.0;
  float h = 0.0;

  // ############################
  Serial.println("======Start Loop======");

  // TCP连接
  Serial.println("Connecting to TCP...");
  int connectionStatus = gprs.connectTCP("82.157.254.205", 28801); // 替换为您的服务器IP和端口
  if (connectionStatus == 0) {
    Serial.println("TCP Connection Successful");

    // 登录到平台
    Serial.println("Logging in to platform...");
    int loginStatus = gprs.sendTCPData("nnkoWqflwH&13w9okpnh5pv966i"); // 替换为登录认证信息
    if (loginStatus == 0) {
      Serial.println("Login to platform successful");
    }
    else {
      Serial.println("Login to platform failed");
    }
  }
  else {
    Serial.println("TCP Connection Failed");
    return;
  }

  //// TIMER 1 --- 1s --- DHT11 read
  if (millis() - dhttime > 1000) {
    readtime++;
    // read dht11
    t = dht.readTemperature();
    h = dht.readHumidity();
    dhttime = millis();
    if (readtime > 3 || readtime == -1) {
      Serial.println("[T]" + String(t, 1) + "," + String(h, 0));
      readtime = 0;
    }
  }
  //// TIMER 2 --- 1s --- TFT display
  if (millis() - lcdtime > 1000) {
    // lcdclearline(1);
    lcdtime = millis();
  }
  //// TIMER 3 --- 1s --- GPRS send json
  // 构造JSON格式的数据
  if (millis() - gprstime > 20000) {

    String jsonData = "{";
    jsonData += "\"temperature\":";
    jsonData += t; // 添加温度数据
    jsonData += ",";
    jsonData += "\"humidity\":";
    jsonData += h; // 添加湿度数据
    jsonData += "}";

    Serial.println("[G]Sending JSON data: " + jsonData);

    // 发送JSON数据
    if (gprs.sendTCPData((char*)jsonData.c_str()) == 0) {
      Serial.println("[G]JSON data sent successfully");
    }
    else {
      Serial.println("[G]Failed to send JSON data");
    }

    gprstime = millis();
  }


  //"AC ON" : "5678" : 0,
  //"AC OFF" : "1234" : 1,
  int key2operation[2] = { 5678, 1234 };



  // if (Serial.available() > 0)  // 判读是否串口有数据
  // {
  if (gprs.serialSIM800.available()) {
    inComing = 1;
    gprs.readBuffer(gprsBuffer, 32, DEFAULT_TIMEOUT);
    // char[] to String
    String payload = String(gprsBuffer);

    // REMOVE GPRS overhead
    // $$+CIPRCV:1,<<data>>$$
    payload.replace("$$", "");
    int commaIndex = payload.indexOf(",");
    if (commaIndex != -1) {
      // 截取逗号后面的部分
      payload = payload.substring(commaIndex + 1);
    }
    payload.replace("\r", "");
    payload.replace("\n", "");
    payload.replace("\r\n", "");


    Serial.print("[>>]");
    Serial.print(payload);
    Serial.println();

    // empty buffer
    for (size_t i = 0; i < 64; i++) {
      gprsBuffer[i] = 0;
    }

    inComing = 0;
  }
  // String comdata = "";            // 缓存清零
  // while (Serial.available() > 0)  // 循环串口是否有数据
  // {
  //   comdata += char(Serial.read());  // 叠加数据到comdata
  //   delay(2);                        // 延时等待响应
  // }
  // if (comdata.length() > 0)  // 如果comdata有数据
  // {
  //   comdata.replace("\r\n", "");  // 去掉回车符
  //   // lcd.setCursor(0, 1);  // 设置光标位置
  //   comdata.replace("\r", "");
  //   comdata.replace("\n", "");
  //   comdata.replace("[W]", "");
  //   Serial.print("[>>]");  // 打印comdata数据
  //   Serial.println(comdata);  // 打印comdata数据
    // if (comdata == (String)key2operation[0]) {
    //   // lcd.print("[W]AC ON");  // 打印comdata数据
    //   digitalWrite(PIN_LED, HIGH);
    // }
    // else if (comdata == (String)key2operation[1]) {
    //   // lcd.print("[W]AC OFF");  // 打印comdata数据
    //   digitalWrite(PIN_LED, LOW);
    // }
    // else {
    //   // lcd.print("[err]"+comdata);  // 打印comdata数据
    // }
    // lcdtime = millis();
//   }
// }

// 断开TCP连接
  delay(5000);
  gprs.closeTCP();
  Serial.println("TCP Connection Closed");
}
