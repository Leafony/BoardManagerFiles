#ifndef LPWA_GPRS_H_
#define LPWA_GPRS_H_

#include "LpwaModemCore.h"
#include "types.h"
#include <IPAddress.h>

/**
 * GPRS接続のためのクラス
 */
class LpwaGprs {
  bool _ready();

public:
  NetworkStatus attachGprs(const char *apn, const char *username,
                           const char *password, unsigned long timeout = 30000);
  NetworkStatus attachGPRS(const char *apn, const char *username,
                           const char *password,
                           unsigned long timeout = 30000) {
    return attachGprs(apn, username, password, timeout);
  }
  NetworkStatus dettachGprs();
  IPAddress getIpAddress();
  IPAddress getIPAddress() { return getIpAddress(); }
};

#endif
