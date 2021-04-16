/*
 * sleep_resume.ino
 * leaf sleep funcfion
 * 
 * STM32をスリープさせるサンプル
 * DFU/USERボタンを押すとスリープから復帰し緑色LEDが点滅する
 * 
 * STM32をスリープさせたい場合はUSB supportを無効「なし」にすること
 * USBペリフェラルだけで１～５mA消費します
 * 
 * このプログラムは次の外部ライブラリを必要とします
 * STM32LowPower
 *  https://github.com/stm32duino/STM32LowPower
 * STM32RTC
 *  https://github.com/stm32duino/STM32RTC
 * 
 */

// スリープ動作選択
// いずれの１つを選択すること
//#define USE_SLEEP  // 4mA
#define USE_DEEPSLEEP // 0.5mA
// #define USE_SHUTDOWN // 0.2mA シャットダウン、レジューム不可

#include "STM32LowPower.h"
#include <Lpwa.h>
LpwaCtrl pmctrl;

// Blink sequence number
// Declare it volatile since it's incremented inside an interrupt
volatile int repetitions = 1;

// Pin used to trigger a wakeup
#ifndef USER_BTN
#define USER_BTN SYS_WKUP1
#endif

const int pin = USER_BTN;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Set pin as INPUT_PULLUP to avoid spurious wakeup
  pinMode(pin, INPUT_PULLUP);

  pinMode(SENS_PWR_ON, OUTPUT);
  digitalWrite(SENS_PWR_ON, LOW);
  pinMode(STM_RESET_N, OUTPUT);
  digitalWrite(STM_RESET_N, LOW);
  pinMode(PERI_PWR_ON, OUTPUT);
  digitalWrite(PERI_PWR_ON, LOW);
  pinMode(LTE_PWR_ON, OUTPUT);
  digitalWrite(LTE_PWR_ON, LOW);
  
  // Configure low power
  LowPower.begin();
  // Attach a wakeup interrupt on pin, calling repetitionsIncrease when the device is woken up
  LowPower.attachInterruptWakeup(pin, repetitionsIncrease, RISING);
}

void loop() {
  for (int i = 0; i < repetitions; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
  // Triggers an infinite sleep (the device will be woken up only by the registered wakeup sources)
  // The power consumption of the chip will drop consistently
#ifdef USE_SLEEP
  LowPower.sleep();
#endif
#ifdef USE_DEEPSLEEP
  LowPower.deepSleep();
#endif
#ifdef USE_SHUTDOWN
  LowPower.shutdown();
#endif
}

void repetitionsIncrease() {
  // This function will be called once on device wakeup
  // You can do some little operations here (like changing variables which will be used in the loop)
  // Remember to avoid calling delay() and long running functions since this functions executes in interrupt context
  repetitions ++;
}
