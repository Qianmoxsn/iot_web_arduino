#include "basic.h"

void BASIC::setupPins() {
  pinMode(GPIO_NUM_2, OUTPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(ONPIN, INPUT_PULLUP);
  pinMode(OFFPIN, INPUT_PULLUP);
  pinMode(STAPIN, OUTPUT);
}

t_mark BASIC::setInterval(int interval, t_mark timemark, void func()) {
  t_mark newtimemark = millis();
  if (newtimemark - timemark >= interval) {
    func();
  }
  return newtimemark;
}

bool BASIC::checkInterval(int interval, t_mark timemark, t_mark *ptr_timemark) {
  t_mark newtimemark = millis();
  if (newtimemark - timemark >= interval) {
    *ptr_timemark = newtimemark;
    return true;
  }
  return false;
}
