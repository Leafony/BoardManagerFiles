#ifndef LPWA_PIN_H_
#define LPWA_PIN_H_

#include "LpwaModemCore.h"
#include "types.h"

/**
 * SIM PINコードの診断機能のためのクラス
 */
class LpwaPin {
public:
  /** 電源をオンにする */
  NetworkStatus begin() { return theLpwaModemCore.begin(); }
  /** 電源をオフにする */
  void end() { return theLpwaModemCore.end(); }
  /** 電源をオフにする */
  void shutdown() { return end(); }

  /**
   * Check if PIN lock or PUK lock is activated
   * @return 0 if PIN lock is off, 1 if PIN lock is on, -1 if PUK lock is on,
   * -2 if error exists
   */
  int isPin();

  /**
   * Check if PIN lock or PUK lock is activated
   * @return 0 if PIN lock is off, 1 if PIN lock is on, -1 if PUK lock is on,
   * -2 if error exists
   */
  int isPIN() { return isPin(); }
};

#endif
