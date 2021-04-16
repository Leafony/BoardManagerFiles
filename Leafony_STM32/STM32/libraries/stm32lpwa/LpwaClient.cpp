#include "LpwaClient.h"

LpwaClient::State LpwaClient::_status() {
  char rcvbuff[100];
  char *pdest = NULL;
  theLpwaModemCore.send("AT+KTCPSTAT=1");
  if (theLpwaModemCore.waitForResponse3("OK\r",rcvbuff,100) < 0) {
    Serial.println("error AT+KTCPSTAT");
    return SOCKET_NOT_DEFINED;
  }

  // NOTE: "+KTCPSTAT: <status>,<tcp_notif>,<rem_data>,<rcv_data>"
  pdest = strstr(rcvbuff, "+KTCPSTAT: ");
  switch (pdest[11] /* <status>*/) {
  case '0':
//    Serial.println("SOCKET_NOT_DEFINED");
    return SOCKET_NOT_DEFINED;
  case '1':
//    Serial.println("SOCKET_NOT_USED");
    return SOCKET_NOT_USED;
  case '2':
//    Serial.println("SOCKET_CONNECTING");
    return SOCKET_CONNECTING;
  case '3':
//    Serial.println("SOCKET_CONNECTED");
    return SOCKET_CONNECTED;
  case '4':
//    Serial.println("SOCKET_CLOSING");
    return SOCKET_CLOSING;
  case '5':
//    Serial.println("SOCKET_CLOSED");
    return SOCKET_CLOSED;
  }

  Serial.println("SOCKET_ERROR");
  return ERROR;
}

LpwaClient::LpwaClient() : _connected(false), _rcvSize(0), _read_p(0) {}

LpwaClient::~LpwaClient() {}

/**
 * 指定されたIPアドレスとポートに接続します。成功した場合は1を返し、そうでない場合は0を返します。
 * @param ip IPアドレス
 * @param port ポート番号
 * @return 接続状態 (1: 接続, 0: それ以外)
 */
int LpwaClient::connect(IPAddress ip, uint16_t port) {
  char host[16];
  sprintf(host, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  return connect(host, port);
}

/**
 * 指定されたホスト名とポートに接続します。成功した場合は1を返し、そうでない場合は0を返します。
 * @param host ホスト名
 * @param port ポート番号
 * @return 接続状態 (1: 接続, 0: それ以外)
 */
int LpwaClient::connect(const char *host, uint16_t port) {
  // NOTE: 接続を打ち切る時間 (ms)
  constexpr unsigned long timeout = 30000ul;
  const unsigned long start = millis();
  _connected = false;
  // NOTE: 以降 TCP Session Index は適当に1を選んでいるがほかでもよい
  theLpwaModemCore.sendf("AT+KTCPCFG=1,0,\"%s\",%d", host, port);
  if (theLpwaModemCore.waitForResponse3("OK\r") < 0)
    return 0;

  theLpwaModemCore.send("AT+KTCPCNX=1");
  if (theLpwaModemCore.waitForResponse3("OK\r") < 0)
    return 0;

  do {
    switch (_status()) {
    case SOCKET_CONNECTING:
      break;
    case SOCKET_CONNECTED:
      Serial.println("<info>TCP CONNECT_OK");
      _connected = true;
      return 1;
    default:
      Serial.println("<info>TCP CONNECT_ERR");
      return 0;
    }

    delay(100);
  } while ((millis() - start) < timeout);
  Serial.println("<info>TCP CONNECT_TIMEOUT");

  
}

/**
 * 接続しているサーバーにデータを送信します。
 * @param c 送信する値
 * @return 送信されたバイト数
 */
size_t LpwaClient::write(uint8_t c) { return write(&c, 1); }

/**
 * 接続しているサーバーにデータを送信します。
 * @param buffer 送信するバイト列
 * @param size 送信するバイト数
 * @return 送信されたバイト数
 */
size_t LpwaClient::write(const uint8_t *buffer, size_t size) {
  theLpwaModemCore.sendf("AT+KTCPSND=1,%d", size);
  if (theLpwaModemCore.waitForResponse3("CONNECT\r") < 0)
    return 0;

  theLpwaModemCore.sendb(buffer, size);
  theLpwaModemCore.printf("--EOF--Pattern--\r\n");

  theLpwaModemCore.send();
  if (theLpwaModemCore.waitForResponse3("OK\r") < 0)
    return 0;

  return size;
}

/**
 * 読み取りに使用できるバイト数を返します。
 * @return 使用できるバイト数
 */
int LpwaClient::available() {
//  Serial.println("@@@@@ available() enter");
  if ((_rcvSize - _read_p) > 0)
    return (_rcvSize - _read_p);
    
  // 受信サイズ取得
  theLpwaModemCore.waitForResponse3("+KTCP_DATA:");
  int size = theLpwaModemCore.getRcvLength();
//  Serial.print("@@@@@ available() size=");
//  Serial.println(size);

  if (size == 0)
    return 0;
  if (size >= 2028) {
    Serial.print("invalid data size! :");
    Serial.println(size);
    return 0;
  }

  // データ受信
  theLpwaModemCore.sendf("AT+KTCPRCV=1,%d", size);
  if (theLpwaModemCore.waitForResponse3("CONNECT\r") < 0) {
    return 0;
  }
  int rcvsize = theLpwaModemCore.waitForResponse3("--EOF--Pattern--\r", _rcvBuffer, RCVBUFFSIZE, 100000);
  if (rcvsize < 1) {
    return 0;
  }
  if (theLpwaModemCore.waitForResponse3("OK\r") < 0) {
    return 0;
  }

  // データ末端にあるEOFパターン"--EOF--Pattern--\r\n"を削除
  _rcvSize = rcvsize - 18; // "--EOF--Pattern--\r\n"
  _rcvBuffer[_rcvSize+1] = 0x0; // 終端コードを追加
  _read_p = 0;
//  Serial.print("@@@@@ available() rcvsize=");
//  Serial.println(_rcvSize);

//  Serial.print("@@@@@ available() step3:");
//  Serial.println(_rcvBuffer);
  
  theLpwaModemCore.waitForResponse3("+KUDP_RCV:");
  
  return _rcvSize;
}

/**
 * 接続しているサーバーからデータを受信します。
 * @return 受信した値 (失敗した場合: -1)
 */
int LpwaClient::read() {
  if ((_rcvSize - _read_p) > 0) {
    int c = _rcvBuffer[_read_p++];
    return c;
  }
  return -1;
}

/**
 * 接続しているサーバーからデータを受信しバッファーに書き込みます。
 * @param buffer 受信したデータを書き込むためのバッファー
 * @param size バッファーに書き込むバイト数
 * @return 書き込まれたバイト数
 */
int LpwaClient::read(uint8_t *buffer, size_t size) {
  int remain = (_rcvSize - _read_p);
  int readsize =0;
  if (remain > size) {
    readsize = size;
  } else {
    readsize = remain;
  }
  char *rdbuff_p = _rcvBuffer + _read_p;
  memcpy(buffer,rdbuff_p,readsize);
  _read_p += readsize;

  return readsize;
}

/**
 * 次回 read() の呼び出しのときの値を返します。
 * @return 受信した値 (失敗した場合: -1)
 */
int LpwaClient::peek() {
  if (available() == 0)
    return -1;

  return _rcvBuffer[_read_p];
}

/**
 * 次回 read() の呼び出しのときの値を返します。
 * @param buffer 受信したデータを書き込むためのバッファー
 * @param size バッファーに書き込むバイト数
 * @return 受信したバイト数
 */
int LpwaClient::peek(uint8_t *buffer, size_t size) {
  if (available() == 0)
    return -1;

  int remain = (_rcvSize - _read_p);
  int readsize =0;
  if (remain > size) {
    readsize = size;
  } else {
    readsize = remain;
  }
  char *rdbuff_p = _rcvBuffer + _read_p;
  memcpy(buffer,rdbuff_p,readsize);

  return readsize;
}

/**
 * サーバーから切断します。
 */
void LpwaClient::stop() {
  if (_connected) {
    theLpwaModemCore.send("AT+KTCPCLOSE=1,1");
    theLpwaModemCore.waitForResponse3("OK\r");
    _connected = false;
  }
  theLpwaModemCore.send("AT+KTCPDEL=1");
  theLpwaModemCore.waitForResponse3("OK\r");
}

/**
 * クライアントが接続されているかどうかを返します。
 * @return 接続状態 (1: 接続, 0: それ以外)
 */
uint8_t LpwaClient::connected() {
  if (!_connected)
    return 0;

  switch (_status()) {
  case ERROR:
  case SOCKET_CLOSED:
    _connected = false;
  }

  return _connected;
}
