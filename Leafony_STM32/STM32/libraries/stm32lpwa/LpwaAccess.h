#ifndef LPWA_ACCESS_H_
#define LPWA_ACCESS_H_

#include "LpwaModemCore.h"
#include "types.h"

/**
 * LPWAライブラリのベースとなるクラス
 */
class LpwaAccess {
public:
  /** LPWAライブラリ初期化 */
  NetworkStatus begin() { return theLpwaModemCore.begin(); }
  /** LPWAライブラリ停止 */
  void end() { return theLpwaModemCore.end(); }
  /** 電源をオフにする */
  void shutdown() { return end(); }
  /** スリープ状態制御 */
//  NetworkStatus sleep(int pmstate) { return theLpwaModemCore.sleep(pmstate); }
  /** 網接続チェック */
  bool isNetworkRegistered() {return theLpwaModemCore.isNetworkRegistered(); }
};

#endif
