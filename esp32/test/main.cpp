#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include <DHT.h>
#include <TFT_eSPI.h>
#include <WiFi.h>

// pin settings (GPIO)
#define DHTPIN GPIO_NUM_15
#define ONPIN GPIO_NUM_16
#define OFFPIN GPIO_NUM_4
#define STAPIN GPIO_NUM_17

// WS client ref: https://blog.csdn.net/qq_43415898/article/details/122113228
class ws_op {
 private:
  const char* STA_ssid = "HONOR-5102";
  const char* STA_password = "qwd13591990755";
  unsigned long ws_time = 0;
  // const char* STA_ssid = "HONOR9X";
  // const char* STA_password = "233233666";

 public:
  // nodeserver ip
  websockets::WebsocketsClient client;
  const char* websockets_server_host = "192.168.3.19";  // Enter server adress
  const uint16_t websockets_server_port = 8080;         // Enter server port
  //@test

  void BlinkHeartBeatLED(int IO_Pin) {
    pinMode(IO_Pin, OUTPUT);
    digitalWrite(IO_Pin, !digitalRead(IO_Pin));
  }
  //@method
  void wifi_init_sta() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(STA_ssid, STA_password);

    Serial.println("Connecting to WiFi..");
    Serial.println("ssid: " + String(STA_ssid));
    while (WiFi.status() != WL_CONNECTED) {
      BlinkHeartBeatLED(2);
      delay(1000);
      Serial.print(WiFi.status());
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi with IP: ");
    Serial.println(WiFi.localIP());
  }

  bool checkconnection(const char* host, short port) {
    bool connected = client.connect(host, port, "/");
    if (connected) {
      Serial.println("Connected to Server");
      client.send("[C]You are connected with ESP32");
    }
    return connected;
  }

  void connect_ws_server(short retrytime) {
    while (!checkconnection(websockets_server_host, websockets_server_port)) {
      Serial.println("Connection Failed! Reconnecting...");
      delay(retrytime);
    }
  }

  void update_ws_connection() { client.poll(); }

  void send_ws_message(String msg) { client.send(msg); }

  void listen() {
    client.onMessage([](websockets::WebsocketsMessage message) {
      String msg = message.data();
      // Serial.println(">>"+msg);
      if (msg[1] == 'W') {
        Serial.println(msg);
        if (msg.substring(3, 7) == "5678") {
          digitalWrite(STAPIN, HIGH);
        }
        if (msg.substring(3, 7) == "1234") {
          digitalWrite(STAPIN, LOW);
        }
      }
    });
  }
};

class dht_op {
  ws_op ws;

 private:
  //@param
  DHT dht_raw = DHT(DHTPIN, DHT11);
  float raw_humidity;
  float raw_temperature;
  // HardwareSerial Serial = Serial;
  int dhttime = 0;

  //@method
  void dhtread_to_serial() {
    Serial.print("H:");
    Serial.print(send_humidity);
    Serial.print("  T:");
    Serial.print(send_temperature);
    Serial.println();
  }

  void dhtread_to_websocket() {
    String msg =
        "[D][T]" + String(send_temperature, 1) +","+ String(send_humidity, 0);
    ws.send_ws_message(msg);
    Serial.println(msg);
  }

 public:
  //@param
  float mod_humidity;
  float mod_temperature;
  float send_humidity;
  float send_temperature;

  //@method
  void setupdht() {
    dht_raw.begin();
    delay(1000);
  }

  short dhtread(short interval, short ADD_MOD) {
    // Serial.println("dhtread:"+String(dhttime) + " " +
    // String(millis()-dhttime));
    if (millis() - dhttime < interval) {
      return 0;
    } else {
      raw_humidity = dht_raw.readHumidity();
      delay(100);
      raw_temperature = dht_raw.readTemperature();
      dhttime = millis();
      if (isnan(raw_humidity) || isnan(raw_temperature)) {
        Serial.println("Failed to read from DHT sensor!");
        return 0;
      }
      send_humidity = raw_humidity + mod_humidity;
      send_temperature = raw_temperature + mod_temperature;
      switch (ADD_MOD) {
        case 0:
          break;
        case 1:
          dhtread_to_serial();
          break;
        case 2:
          dhtread_to_websocket();
          break;
        default:

          Serial.println(String(raw_humidity) + String(send_humidity) +
                         String(raw_temperature) + String(send_temperature));
          break;
      }
      return 1;
    }
  }
};

extern unsigned long btntime = 0;
bool btn_down(short pin) {
  if (digitalRead(pin) == LOW && millis() - btntime > 1000) {
    btntime = millis();
    return true;
  }
  return false;
}

// declare object of class
dht_op dht;
ws_op ws;
TFT_eSPI tft = TFT_eSPI();

void setup() {
  pinMode(GPIO_NUM_2, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(ONPIN, INPUT_PULLUP);
  pinMode(OFFPIN, INPUT_PULLUP);
  pinMode(STAPIN, OUTPUT);

  tft.init();
  tft.fillScreen(TFT_SKYBLUE);
  tft.setCursor(20, 20, 3);
  tft.setTextColor(TFT_WHITE, TFT_SKYBLUE);
  tft.println("Loading...");

  Serial.begin(115200);

  dht.setupdht();

  ws.wifi_init_sta();

  ws.connect_ws_server(1500);

  // run callback when messages are received
  ws.listen();

  // setupfinish
  digitalWrite(GPIO_NUM_2, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  ws.update_ws_connection();

  dht.dhtread(2000, 0);
  ////TODO: rewrite this function to class
  ws.send_ws_message("[D][T]" + String(dht.send_temperature, 1) + "," +
                     String(dht.send_humidity, 0));
  // ws.dhtread_to_websockets(4000);
  if (btn_down(ONPIN)) {
    digitalWrite(STAPIN, HIGH);
    tft.fillScreen(TFT_GREEN);
    ws.send_ws_message("[D][L]5678");
  } else if (btn_down(OFFPIN)) {
    tft.fillScreen(TFT_RED);
    digitalWrite(STAPIN, LOW);
    ws.send_ws_message("[D][L]1234");
  }
}