/*
 * read_sw.ino
 * LPWA leaf on-board io access
 * 
 * LPWAマイコンリーフ上のスイッチ読み出し
 * 押しボタン(GPIO) -- S702(DFUスイッチ)
 * DIPスイッチ状態取得 -- S703
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
  Serial.println("\nLPWA leaf switch test ");
  pinMode(USER_BTN, INPUT);

}
 
void loop() {
  Serial.print("BUTTON-SW: ");
    if (digitalRead(USER_BTN) == 1) {
    Serial.print("Push!");
  }
  Serial.println("");
  
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
