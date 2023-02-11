#include "display.h"

TFT_eSPI tft_raw = TFT_eSPI();

void DISP::display_init() {
  tft_raw.init();
  tft_raw.fillScreen(TFT_SKYBLUE);
  tft_raw.setCursor(30, tft_raw.height() / 2 - 13, 4);
  tft_raw.setTextColor(TFT_WHITE, TFT_SKYBLUE);
  tft_raw.println("Loading...");
}

void DISP::pageOn() {
  tft_raw.fillScreen(0xff9d42);
  tft_raw.setCursor(50, tft_raw.height() / 2 - 39, 4);
  tft_raw.setTextColor(TFT_WHITE, 0xff9d42);
  tft_raw.println("AC ON");
  tft_raw.setCursor(0, tft_raw.height() / 2 + 39, 2);
  tft_raw.println(
      "this page only use for devlopment which indicate the instrument in real "
      "will be turn on");
}
void DISP::pageOff() {
  tft_raw.fillScreen(0xa9e089);
  tft_raw.setCursor(50, tft_raw.height() / 2 - 39, 4);
  tft_raw.setTextColor(TFT_WHITE, 0xa9e089);
  tft_raw.println("AC OFF");
  tft_raw.setCursor(0, tft_raw.height() / 2 + 39, 2);
  tft_raw.println(
      "this page only use for devlopment which indicate the instrument in real "
      "will be turn off");
}
void DISP::pageReady() {
  tft_raw.setCursor(0, 0, 4);
  tft_raw.fillScreen(TFT_BLACK);
  tft_raw.setTextColor(TFT_WHITE, TFT_BLACK);
  tft_raw.println("______READY______");
}
void DISP::pagePrint(String str, int size, int color, int bg) {
  tft_raw.setCursor(10, tft_raw.height() / 2 - 13, 4);
  tft_raw.fillScreen(bg);
  tft_raw.setTextColor(color, bg);
  tft_raw.println(str);
}

// TODO:consider lifetime of a page(maybe use checkinterval in basic.h)