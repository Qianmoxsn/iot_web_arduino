#include <TFT_eSPI.h>

#include "dht_op.h"
#include "ws_op.h"

// pin settings (GPIO)
#define DHTPIN GPIO_NUM_15
#define ONPIN GPIO_NUM_16
#define OFFPIN GPIO_NUM_4
#define STAPIN GPIO_NUM_17

////TODO:write tft driver in a class
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

extern void display_on() {
  tft.fillScreen(0xff9d42);
  tft.setCursor(50, tft.height() / 2 - 13, 4);
  tft.setTextColor(TFT_WHITE, 0xff9d42);
  tft.println("AC ON");
}
extern void display_off() {
  tft.fillScreen(0xa9e089);
  tft.setCursor(50, tft.height() / 2 - 13, 4);
  tft.setTextColor(TFT_WHITE, 0xa9e089);
  tft.println("AC OFF");
}
////END

extern unsigned long btntime = 0;
bool btn_down(short pin) {
  if (digitalRead(pin) == LOW && millis() - btntime > 1000) {
    btntime = millis();
    return true;
  }
  return false;
}
ws_op ws("HONOR-5102", "qwd13591990755", "192.168.3.19", 8080);
dht_op dht;

void setup() {
  pinMode(GPIO_NUM_2, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(ONPIN, INPUT_PULLUP);
  pinMode(OFFPIN, INPUT_PULLUP);
  pinMode(STAPIN, OUTPUT);

  // initial object of class

  tft.init();
  tft.fillScreen(TFT_SKYBLUE);
  tft.setCursor(30, tft.height() / 2 - 13, 4);
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
  delay(1000);
  tft.fillScreen(TFT_BLACK);
}

void loop() {
  tft.setCursor(0, 0, 4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.println("______READY______");
  // put your main code here, to run repeatedly:
  ws.update_ws_connection();

  dht.dhtread(2000, 0);

  ////TODO: rewrite this function to ws_op class and use as a method
  ws.send_ws_message("[D][T]" + String(dht.send_temperature, 1) + "," +
                     String(dht.send_humidity, 0));
  ////END

  // ws.dhtread_to_websockets(4000);
  if (btn_down(ONPIN)) {
    digitalWrite(STAPIN, HIGH);
    display_on();
    ws.send_ws_message("[D][L]5678");
    ////todo:

  } else if (btn_down(OFFPIN)) {
    digitalWrite(STAPIN, LOW);
    display_off();
    ws.send_ws_message("[D][L]1234");
  }
}