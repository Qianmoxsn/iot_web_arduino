#include "web_update.h"

#include "basic.h"
#include "dht_op.h"
#include "display.h"
#include "ws_op.h"

////TEST:OTA test area
// may be not stable
String HOST = "10.122.234.77:5173";
String Dir = "/uploads/firmware.bin";

web_update webUpdate(HOST, Dir, 2, 0);

// END of OTA test area

// TODO:consider rewrite this in basic.h
extern unsigned long btntime = 0;
bool btn_down(short pin) {
  if (digitalRead(pin) == LOW && millis() - btntime > 1000) {
    btntime = millis();
    return true;
  }
  return false;
}
// END

WS_OP ws("BUPT-portal", "", "10.122.234.77", 8080);
DHT_OP dht;

t_mark tm_sendws = millis();

void setup() {
  BASIC::setupPins();

  DISP::display_init();

  Serial.begin(115200);

  dht.setupdht();

  ws.wifi_init_sta();

  ////TEST:OTA test area
  if (BASIC::download_check()) {
    Serial.println("download mode");
    DISP::pagePrint("download mode");
    webUpdate.update_wifi();
  } else {
    Serial.println("normal mode");
    DISP::pagePrint("nomral mode");
  }
  // END of OTA test area

  ws.connect_ws_server(1500);

  // run callback when messages are received
  ws.listen();

  // setupfinish
  digitalWrite(GPIO_NUM_2, LOW);
  delay(1000);

  DISP::pageReady();
}

void loop() {
  // put your main code here, to run repeatedly:
  ws.update_ws_connection();

  dht.dhtread(2000, 0);

  if (BASIC::checkInterval(4000, tm_sendws, &tm_sendws)) {
    ws.send_ws_message(dht.dhtread_to_string());
  }

  if (btn_down(ONPIN)) {
    digitalWrite(STAPIN, HIGH);
    DISP::pageOn();
    ws.send_ws_message("[D][L]5678");

  } else if (btn_down(OFFPIN)) {
    digitalWrite(STAPIN, LOW);
    DISP::pageOff();
    ws.send_ws_message("[D][L]1234");
  }
}