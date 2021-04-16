#include "LpwaScanner.h"

/**
 * 現在のネットワークキャリアの名前を取得して返します。
 * @return 現在のネットワークキャリアの名前
 */
String LpwaScanner::getCurrentCarrier() {
  char rcvbuff[100];
  char *start_p = NULL;
  char *end_p = NULL;

  theLpwaModemCore.send("AT+COPS?");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0)
    return "";
  
  // NOTE: AT+COPS? の応答は `+COPS: <mode>[,<format>,<oper>[,<AcT>]]` 形式
  // NONE: oper のみ文字列で "" で囲まれる
  start_p = strstr(rcvbuff, "+COPS:");
  if (start_p == NULL)
    return "";
  start_p = strstr(start_p, "\"");
  if (start_p == NULL)
    return "";
  start_p++;
  end_p = strstr(start_p, "\"");
  if (end_p == NULL)
    return "";
  *end_p = 0x0;

  return start_p;
}

/**
 * モデムが接続されているネットワークの信号の強度を取得して返します。
 * @return 0-31スケールの信号強度。31 は信号強度 > -51 dBmを意味します。
 * 99は検出不可を意味します。
 */
String LpwaScanner::getSignalStrength() {
  char rcvbuff[100];
  char *start_p = NULL;
  char *end_p = NULL;

  theLpwaModemCore.send("AT+CSQ");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0)
    return "";

  // NOTE: AT+CSQ の応答は `+CSQ: <rssi>,<ber>` 形式
  start_p = strstr(rcvbuff, "+CSQ: ");
  if (start_p == NULL)
    return "";
  start_p = start_p + 6;
  end_p = strstr(start_p, ",");
  if (end_p == NULL)
    return "";
  *end_p = 0x0;

  return start_p;
}
