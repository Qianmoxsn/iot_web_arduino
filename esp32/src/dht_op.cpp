#include "dht_op.h"

void dht_op::dhtread_to_serial() {
  Serial.print("H:");
  Serial.print(send_humidity);
  Serial.print("  T:");
  Serial.print(send_temperature);
  Serial.println();
}

////TODO:enable send_ws_message() function
void dht_op::dhtread_to_websocket() {
  String msg =
      "[D][T]" + String(send_temperature, 1) + "," + String(send_humidity, 0);
  //   ws.send_ws_message(msg);
  Serial.println(msg);
}
////END

void dht_op::setupdht() {
  dht_raw.begin();
  delay(1000);
}

short dht_op::dhtread(short interval, short ADD_MOD) {
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