/*
 * BatteryADC.ino
 * LPWA leaf Battery voltage
 * 
 * LPWAマイコンリーフに接続されたバッテリ電圧を読み取ります
 * USB接続を使うと常時バッテリ充電となり放電とならないことから
 * 放電テストは"USB support(USART)"を無効のこと
 * その場合はSerialはCN703(pin12)から出力
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
  Serial.println("\nLPWA leaf battery ADC");
}
 
void loop() {
  int batt = pmctrl.getBattLevel();
  Serial.print("battery:");
  Serial.print(batt);
  Serial.println(" mV");
  delay(1000);          
}