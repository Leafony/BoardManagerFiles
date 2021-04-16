#include "LpwaModemCore.h"
#include "Stm32Leaf.h"

LpwaModemCore::LpwaModemCore(HardwareSerial &serial, const unsigned long baud)
    : _serial(serial), _baud(baud), status(LPWA_IDLE) {}

LpwaModemCore::~LpwaModemCore() { }

char mdm_rxbuff[2048];
int mdm_rxbuff_p;
int mdm_rxfind_p;
int mdm_rxlast_p;

#define MAX_RESULT 8 // モデム応答の最大要素数

/**
 * モデム応答を分割する
 * @param instring 入力文字列 処理後に文字列が破壊される(NULLが入る)ので注意
 * @param separator 分割文字
 * @param result 分割文字列を入れるポインタ
 * @param max_size 最大分割数
 * @return 分割された文字列数
 */
int LpwaModemCore::split_resp(char* instring, const char* separator, char** result, size_t max_size)
{
  // 入力チェック
  if ((instring == NULL) || (separator == NULL) || (result == NULL) || (max_size < 1)) {
    return -1;
  }

  // 改行以降は判定させないよう削除する
  char *cr_p = NULL;
  cr_p = strchr(instring, '\n');
  if (cr_p != NULL) {
    *cr_p = 0x0; // 終端文字を入れる
  }
  cr_p = strchr(instring, '\r');
  if (cr_p != NULL) {
    *cr_p = 0x0; // 終端文字を入れる
  }

  // レスポンス部のセパレータを探す
  char *start_p = strstr(instring, ": ");
  if (start_p == NULL) {
    return -1;    
  }
  start_p++;  // セパレータをスキップ
  start_p++;

  // 文字列分割
  int ret = 0;
  char* res_p = strtok(start_p, separator);
  if (res_p != NULL) {
    while ((res_p != NULL) && (ret < max_size)) {
      result[ret++] = res_p;
      res_p = strtok(NULL, separator);
    }
  } else {
    // セパレータが見つからない場合は全部
    if (strlen(start_p) > 0) {
      result[ret++] = start_p;
    }
  }
  return ret;
}

/**
 * モデム電源制御
 * @param enable 電源ON
 * @return なし
 */
void LpwaModemCore::power(bool enable) {
  if (enable) {
    digitalWrite(LTE_PWR_ON, HIGH);
    delay(100);
    status = LPWA_IDLE;
  } else {
    digitalWrite(STM_USART3_RTS, LOW);
    digitalWrite(STM_RST_OUT_N, LOW);
    delay(100);
    digitalWrite(LTE_PWR_ON, LOW);
    delay(100);
    status = LPWA_POWEROFF;
  }
}

/**
 * モデムをリセットする
 * @param なし
 * @return モデム起動(true:成功, false:タイムアウト)
 */
bool LpwaModemCore::reset() {
  char resp[20];
  digitalWrite(STM_RST_OUT_N, LOW);
  delay(500);
  digitalWrite(STM_RST_OUT_N, HIGH);
  digitalWrite(STM_WAKE_UP, HIGH); // WAKEUP信号を出す


  // +CEREG:が来るまで待つ
  if (waitForResponse3("+CEREG:", resp, 20, 20000) < 0) { 
    Serial.println("<error> HW: modem not wake");
    send("AT+CFUN=0,0"); // 前回AT+CFUNで別の値をセットすると+CEREGが来ないので再設定する
    status = LPWA_FAIL;
    return false;
  }
  // ダミーATコマンド送信
  send("ATE0");
  waitForResponse3("OK\r");

  send("AT");
  waitForResponse3("OK\r");

  send("AT");
  if (waitForResponse3("OK\r") < 0) {
    status = LPWA_FAIL;
    return false;
  }

  // スリープ解除設定
  send("AT+KSLEEP=2");
  if (waitForResponse3("OK\r") < 0) {
    status = LPWA_FAIL;
    return false;
  }

  // ハードウェアハンドシェイク設定
  send("AT&K3");
  if (waitForResponse3("OK\r") < 0) {
    status = LPWA_FAIL;
    return false;
  }

  // モデム起動成功
  status = LPWA_READY;
  return true;
}

/**
 * モデム省電力設定<br>
 * @param mode 省電力モード 0:NOMAL 1:SLEEP_MODE 2:LIGHT_HIBERNATE 3:HIBERNATE
 * @return 通信状態
 */

NetworkStatus LpwaModemCore::sleep(int pmstate) {
//  Serial.print("<debug> modem sleep");
//  Serial.println(pmstate);

  switch(pmstate) {
  case 0:
    if (status != LPWA_POWERDOWN) {
      // スリープ状態以外の場合は何もしない
      Serial.println("<warn> LPWA:  not sleep mode");
      return status;
    }

    digitalWrite(STM_WAKE_UP, HIGH); // WAKEUP信号を出す
    // ダミーATコマンド送信
    send("AT");
    waitForResponse3("OK\r");

    send("ATE0");
    waitForResponse3("OK\r");

    // スリープ解除コマンド送信
    send("AT+KSLEEP=2");
    if (waitForResponse3("OK\r") < 0) {
      Serial.println("<error> LPWA: resume failed");
      return status = LPWA_FAIL;
    }

    // ハードウェアハンドシェイク設定
    send("AT&K3");
    if (waitForResponse3("OK\r") < 0) {
      Serial.println("<error> LPWA: resume failed");
      return status = LPWA_FAIL;
    }

    digitalWrite(STM_WAKE_UP, LOW); // WAKEUP信号を止める
    digitalWrite(STM_USART3_RTS, HIGH);
    Serial.println("<info> LPWA: resume from sleep mode");
    return status = LPWA_READY;
    break;

  case 1:
  case 2:
  case 3:
    if (status != LPWA_READY) {
      // 無通信状態以外の場合はエラー
      Serial.print("<warn> LPWA: invalid state");
      Serial.println(pmstate);
      return status = LPWA_FAIL;
    }

    sendf("AT+KSLEEP=1,%d,1",pmstate-1);
    if (waitForResponse3("OK\r") < 0) {
      return status = LPWA_FAIL;
    }

    digitalWrite(STM_WAKE_UP, LOW); // WAKEUP信号を止める

    // HIBERNATEではVGPIOの出力が止まることからモデム入力信号から逆流しないようLowレベルに落とす
    if (pmstate == 3) {
//      digitalWrite(STM_USART3_DTR, LOW);
//      digitalWrite(STM_USART3_RTS, LOW);
//      digitalWrite(MDMTX,LOW); 
//      pinMode(MDMTX , OUTPUT);
    }

    Serial.println("<info> LPWA: enter sleep mode");
    return  status = LPWA_POWERDOWN;
    break;
  }

  return  status = LPWA_FAIL;
}


/**
 * ライブラリ初期化
 * @param なし
 * @return 通信状態
 */
NetworkStatus LpwaModemCore::begin() {
  _serial.begin(_baud);
  mdm_rxbuff[0] = 0;
  mdm_rxbuff_p = 0;
  mdm_rxfind_p = 0;
  cereg = -1;
  cnx_stat = 0;
  tcp_stat = 0;
  pm_stat = 0;
  tcp_ind = 0;
  udp_ind = 0;
  http_resp = 0;
  rcv_length = 0;


  if ((status == LPWA_READY) || (status == GPRS_READY)) {
    // 既に初期化されいる場合はスキップする
    return status;
  }

  // GPIO初期化
  pinMode(LTE_PWR_ON, OUTPUT);
  digitalWrite(LTE_PWR_ON, LOW);
  pinMode(STM_PWR_ON_N, OUTPUT);
  digitalWrite(STM_PWR_ON_N, HIGH);
  pinMode(STM_WAKE_UP, OUTPUT);
  digitalWrite(STM_WAKE_UP, LOW);
  pinMode(STM_RST_OUT_N, OUTPUT);
  digitalWrite(STM_RST_OUT_N, LOW);
  pinMode(STM_USART3_RTS, OUTPUT);
  digitalWrite(STM_USART3_RTS, HIGH);
  delay(10);
  
  // モデム電源オン
  power(true);

  // モデムリセット&起動待ち
  if (reset() == false) {
    return status = LPWA_FAIL;
  }

  return status = LPWA_READY; // 正常終了
}

/**
 * ライブラリ不活性化
 */
void LpwaModemCore::end() {
  if (status == LPWA_FAIL || status == GPRS_READY) {
    send("AT+KCNXDOWN=1");
    waitForResponse3("OK\r");
  }

  // モデム電源オフ
  power(false);
  status = LPWA_POWEROFF;
}

/**
 * ATコマンドを送信する
 * @param command ATコマンド
 */
void LpwaModemCore::send(const char *command) {
  int len = strlen(command);
  uint8_t *tmp = (uint8_t *)command;

  DEBUG();
  DEBUG("```send");
  DEBUG(command);
  DEBUG("```");
//  Serial.print("@@@@@ send: ");
//  Serial.println(command);

  for (int i=0;i<len;i++) {
//    Serial.print("[");
//    Serial.print(*tmp,HEX);
//    Serial.print("]");
    _serial.write(*tmp++);
    delay(2); // コマンドが長い場合モデム側がオーバーフローするためタイミング調整
  }
//  Serial.println("");

  _serial.write('\r');
  _serial.flush();
  mdm_rxbuff_p = 0;
}

/**
 * ATコマンドを送信する
 * @param format ATコマンド (printf形式)
 */
void LpwaModemCore::sendf(const char *format, ...) {
  char buffer[BUFSIZ];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buffer, sizeof(buffer) - 1, format, ap);
  va_end(ap);
//  Serial.print("@@@@@ sendf: ");
//  Serial.println(buffer);

  send(buffer);
}

/**
 * モデムのシリアルポートに出力する
 * @param data バイナリデータ
 * @param size 送信データ長
 */
void LpwaModemCore::sendb(const uint8_t *data, int size) {
  const uint8_t *tmp = data;

//  Serial.print("@@@@@ sendb: size=");
//  Serial.print(size);
//  Serial.print(" data=");
  for (int i=0;i<size;i++) {
//    Serial.print("[");
//    Serial.print(*tmp,HEX);
//    Serial.print("]");
    _serial.write(*tmp++);
    delay(2); // 長いデータだとモデム側がオーバーフローするためタイミング調整
  }
  _serial.flush();
//  Serial.println("");
}

/**
 * モデムのシリアルポートに出力する
 * @param format 出力する文字列 (printf形式)
 */
size_t LpwaModemCore::printf(const char *format, ...) {
  char buffer[BUFSIZ];
  va_list ap;
  va_start(ap, format);
  vsnprintf(buffer, sizeof(buffer) - 1, format, ap);
  va_end(ap);

  DEBUG();
  DEBUG("```printf");
  DEBUG(buffer);
  DEBUG("```");

  return _serial.print(buffer);
}

/**
 * LPWA網接続チェック
 * @param なし
 * @return 網接続あり true / 網接続なし false
 */
bool LpwaModemCore::isNetworkRegistered() {
  if (cereg == 1) {
    return true;
  }
  return false;
}

/**
 * 繰り返し呼び出して応答を待つ
 * @param expectedVal 期待するモデム応答文字列
 * @param lead true: 行頭判定、 false: 行末判定
 * @return 0: データなし、1: データあり、-1: エラー検出
 */
int LpwaModemCore::poll2(const char *expectedVal) {
  digitalWrite(STM_USART3_RTS, LOW);
  delay(1);

  while (_serial.available() > 0) {
    char c = _serial.read();
    mdm_rxbuff[mdm_rxbuff_p++] = c; // 受信データをバッファに積む
//    Serial.print("[");
//    Serial.print(c,HEX);
//    Serial.print("]");

    // バイナリ検索対応
    if ( ((c < 0x20) || (c > 0x7f)) & !((c == '\r') || (c =='\n'))) {
      mdm_rxfind_p = mdm_rxbuff_p; // 制御文字が来た場合は検索開始ポインタを進めておく
//      Serial.println("");
//    } else {
//      if ((c == '\r') || (c =='\n')) {
//        Serial.println("");
//      } else {
//        Serial.print("[");
//        Serial.print(c);
//        Serial.print("]");
//      }
    }
 
    if (mdm_rxbuff_p > 2) {
      if ((mdm_rxbuff[mdm_rxbuff_p-2] == '\r') && (mdm_rxbuff[mdm_rxbuff_p-1] == '\n')) {

        digitalWrite(STM_USART3_RTS, HIGH); // 処理中はモデムからの送信を止めさせる
        mdm_rxbuff[mdm_rxbuff_p] = 0x0; // 終端を追加

        // モデム応答チェック
        bool respValid = false; // 有効な応答があった？
        char *pdest = NULL;
        char* mdm_result[MAX_RESULT];
        char* find_string = mdm_rxbuff + mdm_rxfind_p;
//        Serial.print("chk response:");
//        Serial.println(find_string);

        // 非同期通知の処理１
        // CEREGは電源ON検出に使うため最初にチェック
        pdest = strstr(mdm_rxbuff, "+CEREG:");
        if (pdest != NULL) {
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 0) {
            cereg = atoi(mdm_result[0]);
            switch (cereg) {
            case 0:
              Serial.println("<info> EPS: Not registered ");
              break;
            case 1:
              Serial.println("<info> EPS: Registered, home network");
              break;
            case 2:
              Serial.println("<info> EPS: trying to attach or searching");
              break;
            case 3:
              Serial.println("<info> EPS: Registration denied");
              break;
            default:
              Serial.print("<info> CEREG:");
              Serial.println(cereg);
              break;
            }            
          }
//          Serial.print("@@@@@ recive msg: CEREG= ");
//          Serial.println(cereg);
          respValid = true;
        }
        // +KCNX_IND: <cnx cnf>,<status>...
        pdest = strstr(mdm_rxbuff, "+KCNX_IND:");
        if (pdest != NULL) {
//          Serial.print("recive msg: ");
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 1) {
            cnx_stat = atoi(mdm_result[1]);
            switch (cnx_stat) {
            case 0:
              Serial.println("<info> PDP: Disconnected due to network");
              break;
            case 1:
              Serial.println("<info> PDP: Connected");
              break;
            case 2:
              Serial.println("<info> PDP: Failed to connect");
              break;
            case 3:
              Serial.println("<info> PDP: Closed");
              break;
            case 4:
              Serial.println("<info> PDP: Connecting");
              break;
            case 5:
              Serial.println("<info> PDP: Idle time down counting started for disconnection");
              break;
            case 6:
              Serial.println("<info> PDP: Idle time down counting canceled");
              break;
            }
//            Serial.print("KCNX_IND=");
//            Serial.println(cnx_stat);
          }
          respValid = true;
        }
        // +KTCP_DATA: <session_id>,<ndata available>[,<data>]
        pdest = strstr(mdm_rxbuff, "+KTCP_DATA:");
        if (pdest != NULL) {
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 1) {
            rcv_length = atoi(mdm_result[1]);
//            Serial.print("@@@@@recive msg: KTCP_DATA=");
//            Serial.println(rcv_length);
          }
          respValid = true;
        }
        // +KUDP_DATA: <session_id>,<ndata available>[,<udp remote address>,<udp remote port>,<data>]
        pdest = strstr(mdm_rxbuff, "+KUDP_DATA:");
        if (pdest != NULL) {
//          Serial.print("recive msg: ");
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 1) {
            rcv_length = atoi(mdm_result[1]);
//            Serial.print("KUDP_DATA=");
//            Serial.println(rcv_length);
          }
          respValid = true;
        }
        // +KUDP_RCV: <udp remote address>,<udp remote port>
        pdest = strstr(mdm_rxbuff, "+KUDP_RCV:");
        if (pdest != NULL) {
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 1) {
//            Serial.print("@@@@@ recive msg: KUDP_RCV=");
//            Serial.println(mdm_result[0]);
          }
          respValid = true;
        }
        // +KHTTP_IND: <session>,<status>[,<length>,<statuscode>,<statusreason>]
        pdest = strstr(mdm_rxbuff, "+KHTTP_IND:");
        if (pdest != NULL) {
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 4) {
            http_resp = atoi(mdm_result[3]);
//            Serial.print("@@@@@ recive msg: +KHTTP_IND=");
//            Serial.println(http_resp);
          }
          respValid = true;
        }

        // 期待値の応答チェック
//        pdest = strstr(mdm_rxbuff, expectedVal);
        pdest = strstr(find_string, expectedVal);
        if (pdest != NULL) {
          int ret = mdm_rxbuff_p;
//          Serial.println("@@@@@ Response OK");
//          Serial.println(mdm_rxbuff);
          mdm_rxbuff_p = 0;
          mdm_rxfind_p = 0;
          return ret;
        }

        // エラー応答チェック１
        pdest = strstr(mdm_rxbuff, "ERROR\r");
        if (pdest != NULL) {
          _buffer = "ERROR";
          mdm_rxbuff_p = 0;
          mdm_rxfind_p = 0;
//          Serial.println("@@@@@ Response ERROR");
          return -1;
        }
        
        // エラー応答チェック２
        pdest = strstr(mdm_rxbuff, "+CME ERROR:");
        if (pdest != NULL) {
          // エラー応答
          _buffer = "ERROR";
          mdm_rxbuff_p = 0;
          mdm_rxfind_p = 0;

//          Serial.print("@@@@@ Response ERROR: ");
          return -1;
        }

        // +KTCP_NOTIF
        pdest = strstr(mdm_rxbuff, "+KTCP_NOTIF:");
        if (pdest != NULL) {
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 1) {
            tcp_stat = atoi(mdm_result[1]);
//            Serial.print("@@@@@ recive msg: KTCP_NOTIF=");
//            Serial.println(tcp_stat);
          }
          respValid = true;
        }
        // +KUDP_IND:
        pdest = strstr(mdm_rxbuff, "+KUDP_IND:");
        if (pdest != NULL) {
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 1) {
            udp_ind = atoi(mdm_result[1]);
//            Serial.print("@@@@@ recive msg: KUDP_IND=");
//            Serial.println(udp_ind);
          }
          respValid = true;
        }
        // +KTCP_IND: +KTCP_IND: <session_id>,<status>
        pdest = strstr(mdm_rxbuff, "+KTCP_IND:");
        if (pdest != NULL) {
          int cnt = split_resp(pdest, ",", mdm_result, MAX_RESULT);
          if (cnt > 1) {
            tcp_ind = atoi(mdm_result[1]);
//            Serial.print("@@@@@ recive msg: KTCP_IND=");
//            Serial.println(tcp_ind);
          }
          respValid = true;
        }

        if (respValid) {
          mdm_rxbuff_p = 0;
          mdm_rxfind_p = 0;

        } else {
          mdm_rxfind_p = mdm_rxbuff_p; // 検索対象がない場合は検索開始ポインタを進めておく
        }

        
        digitalWrite(STM_USART3_RTS, LOW); // 受信を再開
      }
    }
  } 

  // 受信バッファが空になった
  digitalWrite(STM_USART3_RTS, HIGH);
  return 0;
}

/**
 * モデム応答を待つ
 * @param expectedVal 期待するモデム応答文字列
 * @param respData 応答文字列を入れるバッファ(NULLの場合は応答待ちのみ)
 * @param respSize 応答文字列の許容サイズ
 * @return 0: データなし、1<: データあり、-1: エラー検出
 */
int LpwaModemCore::waitForResponse3(const char *expectedVal, char *respData, int respSize, const unsigned long timeout) {
  const unsigned long end = millis() + timeout;
  int ret_len = 0;

  while (1) {
    int resp = poll2(expectedVal);
    if (resp > 0) {
//      Serial.println("@@@@@ valid response");
      if (respData != NULL) { // 戻り値バッファが空の場合は判定だけ行い終了
        if (resp < respSize) {
          ret_len = resp;
        } else {
          ret_len = respSize; // 許容値よりも大きいデータが来たら許容値
        }
        memcpy(respData, mdm_rxbuff, ret_len);
        respData[ret_len] = 0x0;  // 終端コードを追加
      }
      mdm_rxbuff_p = 0;
      return ret_len;
    }
    if (resp < 0) {
      // エラー応答
//      Serial.println("@@@@@ error response ");
      return -1;
    }
    if (millis() > end) {
//      Serial.println("@@@@@ timeout");
      mdm_rxbuff_p = 0;
      return -1;
    }
  }
}

/*
 * "aa.bb.cc.dd"形式のIPアドレスをIPAdress形式に変換
 * @param ipstring IPアドレス文字列
 * 
 */
IPAddress LpwaModemCore::str2Ip(char*ipstring) {
  IPAddress ip;
  char *ipstart_p = NULL;
  char *ipend_p = NULL;
  ipstart_p = strstr(ipstring, "\"");
  if (ipstart_p == NULL)
    return ip;
  ipstart_p++;
  ipend_p = strstr(ipstart_p, "\"");
  if (ipend_p == NULL)
    return ip;
  *ipend_p = '.';
  for (int i=0;i<4;i++) {
    ipend_p = strstr(ipstart_p, ".");
    if (ipend_p != NULL) {
      *ipend_p = 0x0;
    }
    ip[i] = atoi(ipstart_p);
    ipstart_p = ipend_p +1; 
  }
  return ip;
}


#if 0
/**
 * 繰り返し呼び出して応答を待つ
 * @param eof EOF Pattern 応答を待つか否か (待つ場合: true, それ以外: false)
 * @return 応答があれば true / 無ければ false
 */
bool LpwaModemCore::poll(const bool eof) {
  while (_serial.available() > 0) {
    char c = _serial.read();

    _buffer += c;

    if (c == '\r')
      break;
  }

  if (eof) {
    const bool response = _buffer.endsWith("--EOF--Pattern--\r\nOK\r");
    if (!response)
      return response;

    // NOTE: length of "--EOF--Pattern--\r\nOK\r"
    constexpr size_t lengthOfEof = 21;
    _buffer.remove(_buffer.length() - lengthOfEof, lengthOfEof);

    return response;
  } else if (_buffer.endsWith("OK\r") || _buffer.endsWith("ERROR\r")) {
    _buffer.trim();
    return true;
  }

  return false;
}

/**
 * ATコマンドの送信後のモデムからの応答を待つ
 * @param timeout 応答を待つのを打ち切る時間 (ms)
 * @param eof EOF Pattern 応答を待つか否か (待つ場合: true, それ以外: false)
 * @return 応答 (末尾に"OK"/"ERROR"を含む, ただし eof = true の場合を除く)
 */
String &LpwaModemCore::waitForResponse(const unsigned long timeout,
                                       const bool eof) {
  const unsigned long start = millis();

  while (!poll(eof) && (millis() - start) < timeout) {
    // NOTE: Do nothing.
  }

  DEBUG();
  DEBUG("```response");
  DEBUG(_buffer);
  DEBUG("```");

  // FIXME: タイムアウトしたことを伝える手段がない
  return _buffer;
}
#endif // if 0

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SERIAL)
#ifdef ESP32
// NOTE: with esp32 leaf board
LpwaModemCore theLpwaModemCore(Serial2, 9600);
#endif
HardwareSerial Serial3(PC_5,PC_4);
LpwaModemCore theLpwaModemCore(Serial3, 115200);

#ifdef __AVR__
// NOTE: with avr leaf board
// NOTE: おそらく Serial ではない
LpwaModemCore theLpwaModemCore(Serial, 9600);
#endif
#endif
