#include "LpwaGprs.h"

/**
 * @return GPRS接続状態　(false: 接続中, true: それ以外)
 */
bool LpwaGprs::_ready() {
  return true;
}

/**
 * 指定したアクセスポイント名 (APN) に接続して、GPRS通信を開始します。
 * 接続完了した場合、GPRS_READY を返します。
 * @param apn 提供されているアクセスポイント名 (APN)
 * @param username APNのユーザー名
 * @param password APNのパスワード
 * @param timeout 接続を待つのを打ち切る時間 (ms)
 * @return ネットワークの状態
 */
NetworkStatus LpwaGprs::attachGprs(const char *apn, const char *username,
                                   const char *password,
                                   unsigned long timeout) {
  const unsigned long start = millis();

  // LTE Band 18(800MHz) and 19(800MHz) only
  theLpwaModemCore.sendf("AT+KBNDCFG=0,%x", (1 << (18 - 1)) | (1 << (19 - 1)));
  if (theLpwaModemCore.waitForResponse3("OK\r",NULL,0,10000) < 0)
    return theLpwaModemCore.status = LPWA_FAIL;

  // PDP setting
  theLpwaModemCore.sendf("AT+KCNXCFG=1,\"GPRS\",\"%s\",\"%s\",\"%s\"", apn,
                         username, password);
  if (theLpwaModemCore.waitForResponse3("OK\r") < 0)
    return theLpwaModemCore.status = LPWA_FAIL;

  // connect PDP
  theLpwaModemCore.send("AT+KCNXUP=1");
  if (theLpwaModemCore.waitForResponse3("OK\r") < 0)
    return theLpwaModemCore.status = LPWA_FAIL;

  // wait GPRS connection
  while ((millis() - start) < timeout) {
    theLpwaModemCore.waitForResponse3("+KCNX_IND:",NULL,0,1000);
    if (theLpwaModemCore.getCnxStat() == 1) {
      // success GPRS connection
      return theLpwaModemCore.status = GPRS_READY;
    }
  }

  return theLpwaModemCore.status;
}


/**
 * 接続中のGPRS通信を切断します。
 * 接続完了した場合、LPWA_READY を返します。
 * @return ネットワークの状態
 */
NetworkStatus LpwaGprs::dettachGprs() {
  if (theLpwaModemCore.status != GPRS_READY) {
    Serial.println("<info> dettachGprs(): not connect PDN");
    return theLpwaModemCore.status;
  }

  // disconnect PDP
  theLpwaModemCore.send("AT+KCNXDOWN=1,1");
  theLpwaModemCore.waitForResponse3("OK\r");

  // wait Connection Status Notification
  theLpwaModemCore.waitForResponse3("+KCNX_IND:");
  // check disconnect
  if (theLpwaModemCore.getCnxStat() == 3) { // PDP connection closed
    return theLpwaModemCore.status = LPWA_READY;
  }

  Serial.println("<error> dettachGprs(): close failed");
  return theLpwaModemCore.status = LPWA_FAIL;
}


/**
 * WWANに割り当てられたIPアドレスを返します。
 * 得られない場合は IPAddress(0, 0, 0, 0) を返します。
 * @return WWANに割り当てられたIPアドレス
 */
IPAddress LpwaGprs::getIpAddress() {
  IPAddress ip;
  char rcvbuff[100];
  theLpwaModemCore.send("AT+KCGPADDR");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0) {
    return ip;
  }
  // NOTE: AT+KCGPADDR の応答は `+KCGPADDR: <context>,<address>` 形式
  // NOTE: address のみ文字列で "" で囲まれる
  ip = theLpwaModemCore.str2Ip(strstr(rcvbuff, "+KCGPADDR: "));
  
  return ip;
}
