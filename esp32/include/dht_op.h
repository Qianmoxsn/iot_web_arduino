#pragma once

#include <Arduino.h>
#include <DHT.h>

#include "ws_op.h"

class DHT_OP {
 public:
  ////TODO:use variable to replace dht_pin"15" and dht_type "DHT11"
  DHT dht_raw = DHT(15, DHT11);
  ////END
  
  ////TODO:change some variable to private
  //@param
  float mod_humidity;
  float mod_temperature;
  float send_humidity;
  float send_temperature;
  float raw_humidity;
  float raw_temperature;
  int dhttime = 0;
  ////END

  ////TODO:change some function to private
  //@method
  void dhtread_to_serial();

  String dhtread_to_string();

  void setupdht();

  short dhtread(short interval, short ADD_MOD);
  ////END
};