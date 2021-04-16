/*
 * blink_rgb.ino
 * LPWA leaf blink RGB LED
 * 
 * LPWAマイコンリーフ上のLED等へのアクセスを行います
 * RGB LED
 */

#include "Lpwa.h"

LpwaCtrl pmctrl;

void setup() {
  pmctrl.setLed(true, true, true); // LED_RED, LED_GREEN, LED_BLUE
  delay(500);
  pmctrl.setLed(false, false, false);
  delay(250);
}
 
void loop() {
  pmctrl.setLed(LED_RED, true);
  delay(250);
  pmctrl.setLed(LED_RED, false);
  pmctrl.setLed(LED_GREEN, true);
  delay(250);
  pmctrl.setLed(LED_GREEN, false);
  pmctrl.setLed(LED_BLUE, true);
  delay(250);
  pmctrl.setLed(LED_BLUE, false);
}
