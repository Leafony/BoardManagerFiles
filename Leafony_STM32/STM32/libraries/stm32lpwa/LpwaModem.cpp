#include "LpwaModem.h"

/**
 * モデムのIMEI番号を取得します。begin() の後に呼び出してください。
 * @return モデムのIMEI番号
 */
String LpwaModem::getImei() {
  char rcvbuff[100];
  char *start_p = NULL;
  char *end_p = NULL;

  theLpwaModemCore.send("AT+CGSN");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0)
    return "";

  start_p = strstr(rcvbuff, "\r\n");
  if (start_p == NULL) 
    return "";
  while (1) {
    if ((*start_p == '\r') || (*start_p == '\n') || (*start_p == ' ')) {
       start_p++;
    } else {
      break;
    }
  }
  end_p = strstr(start_p, "\r");
  if (end_p == NULL) 
    return "";
  *end_p = 0x0;
  String imei = start_p;
  return imei;
}

/**
 * SIMのICCID番号を取得します。begin() の後に呼び出してください。
 * @return SIMのICCID番号
 */
String LpwaModem::getIccid() {
  char rcvbuff[100];
  char *start_p = NULL;
  char *end_p = NULL;

  theLpwaModemCore.send("AT+CCID");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0)
    return "";

  start_p = strstr(rcvbuff, "+CCID: ");
  if (start_p == NULL) 
    return "";
  start_p = start_p + 7;
  end_p = strstr(start_p, "\r");
  *end_p = 0x0;
  String iccid = start_p;
  return iccid;
}

/**
 * 現在時刻を取得します。
 * NOTE: 圏外では"00/00/00,00:00:00-00" が得られる
 * @return 現在時刻
 */
String LpwaModem::getTime() {
  char rcvbuff[100];
  char *start_p = NULL;
  char *end_p = NULL;

  theLpwaModemCore.send("AT+CCLK?");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0)
    return "";
  
  start_p = strstr(rcvbuff, "+CCLK: \"");
  if (start_p == NULL) 
    return "";
  start_p = start_p + 8;
  end_p = strstr(start_p, "\"");
  *end_p = 0x0;
  String currentTime = start_p;
  return currentTime;
}
