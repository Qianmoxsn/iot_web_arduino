#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

// FS.h is included in SPIFFS.h
// #include <SPIFFS.h>
#define TOUCH_PIN_ON 4
#define TOUCH_PIN_OFF 14

int ontime = millis();
int offtime = millis();

const char* STA_ssid = "HONOR-5102";
const char* STA_password = "qwd13591990755";

const char* AP_ssid = "ESP32-AP";
const char* AP_password = "233233666";

void wifi_init_sta() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_ssid, STA_password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to the WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}
void wifi_init_ap() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_ssid, AP_password);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}
void wifi_init_apsta() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(AP_ssid, AP_password);
  WiFi.begin(STA_ssid, STA_password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to the WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
}

void touchswitch() {
  int line = 55;
  if (touchRead(TOUCH_PIN_ON) < line) {
    Serial.println("on");
    digitalWrite(GPIO_NUM_32, LOW);
    digitalWrite(GPIO_NUM_25, LOW);
    ontime = millis();
  }
  if (touchRead(TOUCH_PIN_OFF) < line) {
    Serial.println("off");
    digitalWrite(GPIO_NUM_33, LOW);
    digitalWrite(GPIO_NUM_25, LOW);
  }
  if (millis() - ontime > 500) {
    Serial.println("ont");
    digitalWrite(GPIO_NUM_32, HIGH);
    digitalWrite(GPIO_NUM_25, HIGH);
    ontime = millis();
  }
  if (millis() - offtime > 500) {
    Serial.println("offt");
    digitalWrite(GPIO_NUM_33, HIGH);
    digitalWrite(GPIO_NUM_25, HIGH);
    offtime = millis();
  }
}

void setup() {
  WiFi.disconnect();
  Serial.begin(115200);
  // wifi_init_sta();
  // wifi_init_ap();
  // wifi_init_apsta();
  pinMode(TOUCH_PIN_ON, INPUT);
  pinMode(TOUCH_PIN_OFF, INPUT);
  pinMode(GPIO_NUM_33, OUTPUT);
  pinMode(GPIO_NUM_25, OUTPUT);
  pinMode(GPIO_NUM_32, OUTPUT);
  digitalWrite(GPIO_NUM_33, HIGH);
  digitalWrite(GPIO_NUM_25, HIGH);
  digitalWrite(GPIO_NUM_32, HIGH);
}
void loop() {
  // put your main code here, to run repeatedly:

  touchswitch();
}