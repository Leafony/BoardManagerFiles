#ifndef LPWA_MODEM_H_
#define LPWA_MODEM_H_

#include "LpwaModemCore.h"
#include "types.h"
#include <Arduino.h>

/**
 * モデムの診断機能ためのクラス
 */
class LpwaModem {
public:
  /** 電源をオンにする */
  NetworkStatus begin() { return theLpwaModemCore.begin(); }
  /** 電源をオフにする */
  void end() { return theLpwaModemCore.end(); }
  /** 電源をオフにする */
  void shutdown() { return end(); }

  String getImei();
  String getIMEI() { return getImei(); }

  String getIccid();
  String getICCID() { return getIccid(); }

  String getTime();
};
#endif
