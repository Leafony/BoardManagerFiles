#ifndef LPWA_MODEM_CORE_H_
#define LPWA_MODEM_CORE_H_

#include "debug.h"
#include "types.h"
#include <Arduino.h>
#include <IPAddress.h>

/**
 * theLpwaModemCoreインスタンスを作るための内部利用クラス
 */
class LpwaModemCore {
  PinName _rx;
  PinName _tx;
  HardwareSerial &_serial;
  const unsigned long _baud;
  String _buffer;

public:
  LpwaModemCore(HardwareSerial &serial, const unsigned long baud);
  ~LpwaModemCore();
  NetworkStatus status;
  NetworkStatus begin();
  void end();
  void power(bool enable = false);
  bool reset();
  NetworkStatus sleep(int state =0);

  void send(const char *command = "");
  void send(const String &command) { send(command.c_str()); }
  void sendf(const char *format, ...);
  void sendb(const uint8_t *data, int size);
  size_t printf(const char *format, ...);
  bool noop();
  bool poll(const bool eof = false);
  int poll2(const char *expectedVal);
  int split_resp(char* instring, const char* separator, char** result, size_t max_size);
  IPAddress str2Ip(char* ipstring);

  int waitForResponse3(const char *expectedVal, char *respData = NULL, int respSize = 0, const unsigned long timeout = 3000);
  bool isNetworkRegistered();
 
  /**
   * 現在のEPS Network Registration Statusを取得する
   * @param なし
   * @return EPS Network Registration Status
   */
  int getCereg() {
    return cereg;
  }

  /**
   * 現在のPDP connection statusを取得する
   * @param なし
   * @return PDP connection status
   */
  int getCnxStat() {
    return cnx_stat;
  }

  /**
   * 現在のTCP connection statusを取得する
   * @param なし
   * @return TCP connection status
   */
  int getTcpStat() {
    return tcp_stat;
  }

  /**
   * 取得されたHTTP response codeを取得する
   * @param なし
   * @return HTTP response code
   */
  int getHttpResp() {
    int ret = http_resp;
    http_resp = 0;
    return ret;
  }


  /**
   * 現在のTCP/UDP受信バイト数を取得する
   * @param なし
   * @return 受信バイト数
   */
  int getRcvLength() {
    int ret = rcv_length;
    rcv_length = 0;
    return ret;
  }

private:
  int cereg;    // EPS Network Registration Status
  int cnx_stat; // PDP connection status
  int tcp_stat; // TCP connection status
  int pm_stat; // power managment status
  int tcp_ind; // TCP status
  int udp_ind; // UDP status
  int http_resp; // HTTP response code
  int rcv_length; // +KUDP_DATA: +KTCP_DATA:
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
extern LpwaModemCore theLpwaModemCore;
#endif

#endif
