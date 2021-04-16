#ifndef LPWA_CLIENT_H_
#define LPWA_CLIENT_H_

#include "LpwaModemCore.h"
#include <Arduino.h>
#include <Client.h>

#define RCVBUFFSIZE 2048
/**
 * TCPソケット通信のためのクラス
 */
class LpwaClient : public Client {
  enum State {
    SOCKET_NOT_DEFINED,
    SOCKET_NOT_USED,
    SOCKET_CONNECTING,
    SOCKET_CONNECTED,
    SOCKET_CLOSING,
    SOCKET_CLOSED,
    ERROR
  };
  LpwaClient::State _status();
  bool _connected;
 // String _buffer;
  char _rcvBuffer[RCVBUFFSIZE];
  int _rcvSize;
  int _read_p;


public:
  LpwaClient();
  ~LpwaClient();

  /**
   * 指定されたホスト名とポートに接続します。成功した場合は1を返し、そうでない場合は0を返します。
   * @param host ホスト名
   * @param port ポート番号
   * @return 接続状態 (1: 接続, 0: それ以外)
   */
  int connect(String host, uint16_t port) {
    return connect(host.c_str(), port);
  }

  int connect(IPAddress ip, uint16_t port);
  int connect(const char *host, uint16_t port);
  size_t write(uint8_t c);
  size_t write(const uint8_t *buffer, size_t size);
  int available();
  int read();
  int read(uint8_t *buffer, size_t size);
  int peek();
  int peek(uint8_t *buffer, size_t size);

  /** 何もしません。 */
  void flush() {}

  void stop();
  uint8_t connected();

  /**
   * クライアントが接続されているかどうかを返します。
   * 読み取りに使用できるバッファーが空の場合、サーバーから切断します。
   * @return 接続状態 (true: 接続, false: それ以外)
   */
  operator bool() { return connected(); }
};

#endif
