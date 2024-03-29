#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>

class DISP {

 public:
//   static TFT_eSPI tft_raw;
  static void display_init();
  static void pageOn();
  static void pageOff();
  static void pageReady();
  static void pagePrint(String str, int size = 4,int color = TFT_WHITE, int bg = TFT_BLACK);
};