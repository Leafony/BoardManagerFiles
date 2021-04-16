/*
 * SleepDevice.ino
 * Lpwa sleep test
 *
 *  created 6 Jan 2021
 * by kt-nakamura
 *
 */

// 省電力モード
// いずれの１つを選択すること
#define USE_SLEEP
//#define USE_HIBERNATE
//#define USE_L_HIBERNATE
//#define USE_POWEROFF

#include <Lpwa.h>
#include "arduino_secrets.h"
// Please enter your sensitive data in the Secret tab or arduino_secrets.h
// APN data
const char GPRS_APN[] = SECRET_GPRS_APN;
const char GPRS_LOGIN[] = SECRET_GPRS_LOGIN;
const char GPRS_PASSWORD[] = SECRET_GPRS_PASSWORD;

GPRS gprs;
LpwaAccess lpwaAccess;
LpwaCtrl pmctrl;

void setup() {
  Serial.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC
  Serial.println("Starting LPWA power control.");

  // LPWAデバイスの初期化
  bool lpwa_enabled = false;
  while (!lpwa_enabled) {
    if (lpwaAccess.begin() == LPWA_READY) {
      lpwa_enabled = true;
    } else {
      Serial.println("starting LPWA device.");
      delay(1000);
    }
  }
  Serial.println("LPWA device enabled");
}

void loop() {
  // LPWA接続開始
  Serial.println("connect PDN");
  bool pdn_connected = false;
  while (!pdn_connected) {
    if(gprs.attachGPRS(GPRS_APN, GPRS_LOGIN, GPRS_PASSWORD) == GPRS_READY) {
      pdn_connected = true;
    } else {
      Serial.println("connecting.");
      delay(1000);
    }
  }
  // LPWA接続成功＆IP取得
  Serial.println("connected");
  Serial.print("IP:");
  Serial.println(gprs.getIPAddress());
  delay(5000);

  // LPWA接続終了
  Serial.println("disconnect PDN");
  NetworkStatus a = gprs.dettachGprs();
  delay(5000);

  // 省電力状態に移行
  Serial.println("power-down LPWA device");
#ifdef USE_SLEEP
  pmctrl.powerDown(LPWA_SLEEP);
#endif
#ifdef USE_HIBERNATE
  pmctrl.powerDown(LPWA_HIBERNATE);
#endif
#ifdef USE_L_HIBERNATE
  pmctrl.powerDown(LPWA_L_HIBERNATE);
#endif
#ifdef USE_POWEROFF
  pmctrl.powerDown(LPWA_OFF);
#endif
  delay(5000);

  // 省電力状態からの復帰
  Serial.println("resume LPWA device");
  pmctrl.powerDown(LPWA_NORMAL);
  delay(5000);
}
