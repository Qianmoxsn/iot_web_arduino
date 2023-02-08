#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include "basic.h"
#include "display.h"

// WS client ref: https://blog.csdn.net/qq_43415898/article/details/122113228
class WS_OP {
 private:
  String STA_ssid;
  String STA_password;
  String websockets_server_host;  // Enter server adress
  short websockets_server_port;   // Enter server port
  unsigned long ws_time = 0;

 public:
  // nodeserver ip
  websockets::WebsocketsClient client;
  //@method
  WS_OP(String, String, String, short);

  void BlinkHeartBeatLED(int);

  void wifi_init_sta();

  bool checkconnection(const char*, short);

  void connect_ws_server(short);

  void update_ws_connection();

  void send_ws_message(String);

  void listen();
};