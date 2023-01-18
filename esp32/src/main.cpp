#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

// FS.h is included in SPIFFS.h
// #include <SPIFFS.h>
#define TOUCH_PIN_ON 4
#define TOUCH_PIN_OFF 14

int ontime = millis();
int offtime = millis();
int testtime = millis();
int onstate = 0;
int offstate = 0;

const char* STA_ssid = "HONOR-5102";
const char* STA_password = "qwd13591990755";

const char* AP_ssid = "ESP32-AP";
const char* AP_password = "233233666";

// nodeserver ip
const char* host = "192.168.3.112";

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
  if (touchRead(TOUCH_PIN_ON) < line && onstate == 0 &&
      millis() - ontime > 100) {
    // Serial.println("on");
    onstate = 1;
    digitalWrite(GPIO_NUM_32, LOW);
    digitalWrite(GPIO_NUM_25, LOW);
    Serial2.println("5678");
    ontime = millis();
  } else if (touchRead(TOUCH_PIN_OFF) < line && offstate == 0 &&
             millis() - offtime > 100) {
    // Serial.println("off");
    offstate = 1;
    digitalWrite(GPIO_NUM_33, LOW);
    digitalWrite(GPIO_NUM_25, LOW);
    Serial2.println("1234");
  }
  if (millis() - ontime > 500 && onstate == 1) {
    digitalWrite(GPIO_NUM_32, HIGH);
    digitalWrite(GPIO_NUM_25, HIGH);
    onstate = 0;
    ontime = millis();
  }
  if (millis() - offtime > 500 && offstate == 1) {
    digitalWrite(GPIO_NUM_33, HIGH);
    digitalWrite(GPIO_NUM_25, HIGH);
    offstate = 0;
    offtime = millis();
  }
}

void setup() {
  // WiFi.disconnect();
  pinMode(GPIO_NUM_2, OUTPUT);
  pinMode(GPIO_NUM_33, OUTPUT);
  pinMode(GPIO_NUM_25, OUTPUT);
  pinMode(GPIO_NUM_32, OUTPUT);
  pinMode(GPIO_NUM_26, INPUT);
  pinMode(GPIO_NUM_27, OUTPUT);
  digitalWrite(GPIO_NUM_33, HIGH);
  digitalWrite(GPIO_NUM_25, HIGH);
  digitalWrite(GPIO_NUM_32, HIGH);

  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, 26, 27);

  wifi_init_sta();

  // wifi_init_ap();
  // wifi_init_apsta();
  pinMode(TOUCH_PIN_ON, INPUT);
  pinMode(TOUCH_PIN_OFF, INPUT);
}
void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(GPIO_NUM_2, HIGH);


  if (Serial2.available()) {
    Serial.write(Serial2.read());
  }

  touchswitch();
}