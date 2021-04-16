#ifndef LPWA_MODEM_CTRL_H_
#define LPWA_MODEM_CTRL_H_

#include <Arduino.h>
#include "LpwaModemCore.h"
#include "Stm32Leaf.h"
#include "types.h"

#define LPWA_OFF     0
#define LPWA_NORMAL  1
#define LPWA_RESET   2
#define LPWA_SLEEP  3
#define LPWA_L_HIBERNATE 4 
#define LPWA_HIBERNATE 5 

// 旧API互換定義
//#define PWR_OFF     LPWA_OFF
//#define PWR_NORMAL  LPWA_NORMAL
//#define DEV_RESET   LPWA_RESET
//#define PWR_SLEEP1  LPWA_SLEEP
//#define PWR_SLEEP2  LPWA_L_HIBERNATE
//#define PWR_SLEEP3  LPWA_HIBERNATE

class LpwaCtrl {

public:
  LpwaCtrl();
  void powerDown(int state);
  void enableSensor(bool enable);
  void enableLeaf(bool enable);
  void setLed(bool red, bool green, bool blue);
  void setLed(int led, bool enable);
  int getBattLevel();
  bool getDipSw(int no);
};

#endif // LPWA_MODEM_CTRL_H_
