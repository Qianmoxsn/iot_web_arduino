#include "ws_op.h"

WS_OP::WS_OP(String ssid, String password, String host, short port) {
  STA_ssid = ssid;
  STA_password = password;
  websockets_server_host = host;
  websockets_server_port = port;
}

void WS_OP::BlinkHeartBeatLED(int IO_Pin) {
  pinMode(IO_Pin, OUTPUT);
  digitalWrite(IO_Pin, !digitalRead(IO_Pin));
}
//@method
void WS_OP::wifi_init_sta() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_ssid.c_str(), STA_password.c_str());

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

bool WS_OP::checkconnection(const char* host, short port) {
  bool connected = client.connect(host, port, "/");
  if (connected) {
    Serial.println("Connected to Server");
    client.send("[C]You are connected with ESP32");
  }
  return connected;
}

void WS_OP::connect_ws_server(short retrytime) {
  while (!checkconnection(websockets_server_host.c_str(),
                          websockets_server_port)) {
    Serial.println("Connection Failed! Reconnecting...");
    delay(retrytime);
  }
}

void WS_OP::update_ws_connection() { client.poll(); }

void WS_OP::send_ws_message(String msg) { client.send(msg); }

void WS_OP::listen() {
  client.onMessage([](websockets::WebsocketsMessage message) {
    String msg = message.data();
    // Serial.println(">>"+msg);
    if (msg[1] == 'W') {
      Serial.println(msg);
      if (msg.substring(3, 7) == "5678") {
        digitalWrite(STAPIN, HIGH);
        DISP::pageOn();
      }
      if (msg.substring(3, 7) == "1234") {
        digitalWrite(STAPIN, LOW);
        DISP::pageOff();
      }
    }
  });
}
