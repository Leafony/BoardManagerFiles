#ifndef LPWA_UDP_H_
#define LPWA_UDP_H_

#include "LpwaModemCore.h"
#include <Arduino.h>
#include <Udp.h>

#define RCVBUFFSIZE 2048
/**
 * UDPソケット通信のためのクラス
 */
class LpwaUdp : public UDP {
  IPAddress _ip;
  String _host;
  uint16_t _port;
  String _txBuffer;
//  String _rxBuffer;
  char _rcvBuffer[RCVBUFFSIZE];
  int _rcvSize;
  int _read_p;
  int _beginPacket();
  bool _open;

public:
  LpwaUdp();
  ~LpwaUdp();

  /**
   * initialize, start listening on specified port.
   * Returns 1 if successful, 0 if there are no sockets
   * available to use
   * @param localPort クライアントポート
   * @return 初期化状態 (1: 利用可能, 0: それ以外)
   * FIXME: クライアントポートは機能しない
   */
  uint8_t begin(uint16_t localPort = 0);

  /** Finish with the UDP socket */
  void stop();

  /**
   * Start building up a packet to send to the remote host specific in host and
   * port Returns 1 if successful, 0 if there was a problem resolving the
   * hostname or port
   * @return 接続状態 (1: 成功, 0: それ以外)
   */
  int beginPacket(String host, uint16_t port) {
    return beginPacket(host.c_str(), port);
  }

  /**
   * Start building up a packet to send to the remote host specific in ip and
   * port Returns 1 if successful, 0 if there was a problem with the supplied IP
   * address or port
   * @return 接続状態 (1: 成功, 0: それ以外)
   */
  int beginPacket(IPAddress ip, uint16_t port);

  /**
   * Start building up a packet to send to the remote host specific in host and
   * port Returns 1 if successful, 0 if there was a problem resolving the
   * hostname or port
   * @return 接続状態 (1: 成功, 0: それ以外)
   */
  int beginPacket(const char *host, uint16_t port);

  /**
   * Finish off this packet and send it
   * Returns 1 if the packet was sent successfully, 0 if there was an error
   */
  int endPacket();

  /**
   * Write a single byte into the packet
   */
  size_t write(const uint8_t c);

  /**
   * Write size bytes from buffer into the packet
   */
  size_t write(const uint8_t *buffer, size_t size);

  /**
   * Start processing the next available incoming packet
   * Returns the size of the packet in bytes, or 0 if no packets are available
   */
  int parsePacket() {return available(); }

  /**
   * Number of bytes remaining in the current packet
   */
  int available();

  /**
   * Read a single byte from the current packet
   */
  int read();

  /**
   * Read up to size bytes from the current packet and place them into buffer
   * Returns the number of bytes read, or 0 if none are available
   */
  int read(uint8_t *buffer, size_t size);

  /**
   * Read up to size characters from the current packet and place them into
   * buffer Returns the number of characters read, or 0 if none are available
   */
  int read(char *buffer, size_t size) { return read((uint8_t *)buffer, size); }

  /**
   * Return the next byte from the current packet without moving on to the next
   * byte
   */
  int peek();

  /**
   * 次回 read() の呼び出しのときの値を返します。
   * @param buffer 受信したデータを書き込むためのバッファー
   * @param size バッファーに書き込むバイト数
   * @return 受信したバイト数
   */
  int peek(uint8_t *buffer, size_t size);

  /** 何もしません。 */
  void flush() {}

  /** Return the IP address of the host who sent the current incoming packet */
  IPAddress remoteIP() { return remoteIp(); }

  /** Return the IP address of the host who sent the current incoming packet */
  IPAddress remoteIp() { return _ip; }

  /** Return the port of the host who sent the current incoming packet */
  uint16_t remotePort() { return _port; }

private:
};

#endif
