/*
 * LED_SW.ino
 * LPWA leaf on-board io access
 * 
 * LPWAマイコンリーフ上のLED等へのアクセスを行います
 * RGB LED
 * DIPスイッチ状態取得
 */

#include "Lpwa.h"

LpwaCtrl pmctrl;

void setup() {
  Serial.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC
  Serial.println("\nLPWA leaf onboard I/O");

  Serial.println("Leaf Power ON");
  pmctrl.setLed(true, false, false);
  delay(500);
  pmctrl.setLed(false, true, false);
  delay(500);
  pmctrl.setLed(false, false, true);
  delay(500);
  pmctrl.setLed(false, false, false);
}
 
void loop() {
  Serial.print("DIP-SW: ");
  if (pmctrl.getDipSw(1)) {
    Serial.print("SW1 ");
  }
  if (pmctrl.getDipSw(2)) {
    Serial.print("SW2 ");
  }
  if (pmctrl.getDipSw(3)) {
    Serial.print("SW3 ");
  }
  if (pmctrl.getDipSw(4)) {
    Serial.print("SW4 ");
  }
  Serial.println("");
  delay(1000);          
}
