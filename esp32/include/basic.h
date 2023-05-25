#pragma once

#include <Arduino.h>
#include "ws_op.h"

typedef unsigned long t_mark;
// pin settings (GPIO)
#define DHTPIN GPIO_NUM_15
#define ONPIN GPIO_NUM_16
#define OFFPIN GPIO_NUM_17
#define STAPIN GPIO_NUM_4
#define LEDPIN GPIO_NUM_2
#define DOWNLOADPIN GPIO_NUM_22

class BASIC {
 public:
  //@param
  //@method
  static void setupPins();
  static bool download_check();
  static t_mark setInterval(int interval, t_mark timemark, void func());
  static bool checkInterval(int interval, t_mark timemark, t_mark *newtimemark);
};
