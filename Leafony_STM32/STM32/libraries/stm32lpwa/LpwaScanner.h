#ifndef LPWA_SCANNER_H_
#define LPWA_SCANNER_H_

#include "LpwaModemCore.h"
#include "types.h"

/**
 * ネットワークの診断機能のためのクラス
 */
class LpwaScanner {
public:
  /** 電源をオンにする */
  NetworkStatus begin() { return theLpwaModemCore.begin(); }
  /** 電源をオフにする */
  void end() { return theLpwaModemCore.end(); }
  /** 電源をオフにする */
  void shutdown() { return end(); }

  String getCurrentCarrier();
  String getSignalStrength();
};

#endif
