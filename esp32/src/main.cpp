#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>


// FS.h is included in SPIFFS.h
// #include <SPIFFS.h>

// WS client https://blog.csdn.net/qq_43415898/article/details/122113228
#define TOUCH_PIN_ON 4
#define TOUCH_PIN_OFF 14

int ontime = millis();
int offtime = millis();
int testtime = millis();
int onstate = 0;
int offstate = 0;

const char* STA_ssid = "HONOR-5102";
const char* STA_password = "qwd13591990755";

// nodeserver ip
const char* websockets_server_host = "192.168.3.112";  // Enter server adress
const uint16_t websockets_server_port = 8080;          // Enter server port

using namespace websockets;

WebsocketsClient client;

void wifi_init_sta() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_ssid, STA_password);
  Serial.println("Connecting to WiFi..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi with IP: ");
  Serial.println(WiFi.localIP());
}


void setup() {
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
  Serial2.begin(9600);

  wifi_init_sta();

  pinMode(TOUCH_PIN_ON, INPUT);
  pinMode(TOUCH_PIN_OFF, INPUT);

  // try to connect to Websockets server
  bool connected =
      client.connect(websockets_server_host, websockets_server_port, "/");
  if (connected) {
    Serial.println("Connected to Server");
    client.send("[C]You are connected with ESP32");
  } else {
    Serial.println("Not Connected!");
  }

  // run callback when messages are received
  client.onMessage([&](WebsocketsMessage message) {
    Serial.print("Got Message: ");
    String msg = message.data();
    if (msg[1] == 'W') {
      Serial2.println(msg);
    }

    Serial.println(msg);
  });

  digitalWrite(GPIO_NUM_2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  client.poll();

  if (Serial2.available() > 0)  // 判读是否串口有数据
  {
    String comdata = "";             // 缓存清零
    while (Serial2.available() > 0)  // 循环串口是否有数据
    {
      comdata += char(Serial2.read());  // 叠加数据到comdata
      delay(2);                         // 延时等待响应
    }
    if (comdata.length() > 0)  // 如果comdata有数据
    {
      comdata.replace("\r", "");  // 去掉回车符
      comdata.replace("\n", "");  // 去掉换行符

      Serial.println(comdata);  // 打印comdata
      client.send("[D]" + comdata);
    }
  }
  // touchswitch();
}