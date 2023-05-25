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
  // WiFi.begin("test", "233233666");
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

  //certification
  // 设置请求头
  HTTPClient http;
  http.begin("http://" + String(server) + ":" + String(server_port) + login_path);
  http.addHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7");
  http.addHeader("Accept-Encoding", "gzip, deflate");
  http.addHeader("Accept-Language", "zh-CN,zh;q=0.9,en;q=0.8,en-GB;q=0.7,en-US;q=0.6");
  http.addHeader("Cache-Control", "max-age=0");
  http.addHeader("Connection", "keep-alive");
  http.addHeader("Content-Length", "29");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.addHeader("Cookie", "SessionId=b289698c8d723865");
  http.addHeader("Host", server);
  http.addHeader("Origin", "http://" + String(server));
  http.addHeader("Referer", "http://" + String(server) + "/index");
  http.addHeader("Upgrade-Insecure-Requests", "1");
  http.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/110.0.0.0 Safari/537.36 Edg/110.0.1587.50");

  // 设置请求数据
  String data = "user=2021213064&pass=Bupt2833";

  // 发送POST请求
  int httpCode = http.POST(data);

  // 输出响应结果
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
  } else {
    Serial.println("Error on HTTP request");
  }

  // Check response content
  String response_content = http.getString();
  Serial.println("Response content: ");
  Serial.println(response_content);

  http.end();
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
