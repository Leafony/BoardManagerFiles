#include "LpwaUdp.h"

LpwaUdp::LpwaUdp()
    : _ip(0, 0, 0, 0), _host(""), _port(0), _txBuffer(""), _rcvSize(0), _read_p(0), _open(false) {}

LpwaUdp::~LpwaUdp() {
  _host.~String();
  _txBuffer.~String();
}

int LpwaUdp::_beginPacket() {
  if (!_open) {
    theLpwaModemCore.send("AT+KUDPCFG=1,0");
    if (theLpwaModemCore.waitForResponse3("OK\r") < 0) {
      _open = true;
      return 1;
    } else {
      return 0;
    }
  }
  return 1;
}

uint8_t LpwaUdp::begin(uint16_t localPort) {
  // Note: LPWA では　localPort を指定できない
  return 1;
}

int LpwaUdp::beginPacket(IPAddress ip, uint16_t port) {
  _ip = ip;
  char host[16];
  sprintf(host, "%u.%u.%u.%u", ip[0], ip[1], ip[2], ip[3]);
  _host = host;
  _port = port;
  return _beginPacket();
}

int LpwaUdp::beginPacket(const char *host, uint16_t port) {
  _ip.fromString(host);
  _host = host;
  _port = port;
  return _beginPacket();
}

int LpwaUdp::endPacket() {
  theLpwaModemCore.sendf("AT+KUDPSND=1,\"%s\",%d,%d", _host.c_str(), _port,
                         _txBuffer.length());
  if (theLpwaModemCore.waitForResponse3("CONNECT\r") < 0)
    return 0;

  char txbuff[_txBuffer.length()];
  _txBuffer.toCharArray(txbuff, _txBuffer.length());
  theLpwaModemCore.sendb((uint8_t *)txbuff, _txBuffer.length());
  theLpwaModemCore.printf("--EOF--Pattern--\r\n");

  theLpwaModemCore.send();
  if (theLpwaModemCore.waitForResponse3("OK\r") < 0)
    return 0;
  
  _txBuffer = "";
  return 1;
}

size_t LpwaUdp::write(const uint8_t c) { return write(&c, 1); }

size_t LpwaUdp::write(const uint8_t *buffer, size_t size) {
  for (size_t i = 0; i < size; i++)
    _txBuffer += (char)buffer[i];

  return size;
}

void LpwaUdp::stop() {
  _open = false;
  theLpwaModemCore.send("AT+KUDPCLOSE=1");
  theLpwaModemCore.waitForResponse3("OK\r");
  theLpwaModemCore.send("AT+KUDPDEL=1");
  theLpwaModemCore.waitForResponse3("OK\r");
}

int LpwaUdp::available() {
  if ((_rcvSize - _read_p) > 0)
    return (_rcvSize - _read_p);

  // 受信サイズ取得
  theLpwaModemCore.waitForResponse3("+KUDP_DATA:");
  int size = theLpwaModemCore.getRcvLength();

  if (size == 0)
    return 0;
  if (size >= RCVBUFFSIZE-20) {
    Serial.print("invalid data size! :");
    Serial.println(size);
    return 0;
  }

  // データ受信
  theLpwaModemCore.sendf("AT+KUDPRCV=1,%d", size);
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
 
  theLpwaModemCore.waitForResponse3("+KUDP_RCV:");
  
  return _rcvSize;
}

int LpwaUdp::read() {
  if ((_rcvSize - _read_p) > 0) {
    int c = _rcvBuffer[_read_p++];
    return c;
  }
  return -1;
}

int LpwaUdp::read(uint8_t *buffer, size_t size) {
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

int LpwaUdp::peek() {
  if (available() == 0)
    return -1;

  return _rcvBuffer[_read_p];
}

int LpwaUdp::peek(uint8_t *buffer, size_t size) {
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
