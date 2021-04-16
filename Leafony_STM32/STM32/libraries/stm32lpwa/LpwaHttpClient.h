#ifndef LPWA_HTTP_CLIENT_H_
#define LPWA_HTTP_CLIENT_H_

#include "LpwaModemCore.h"
#include "types.h"
#include <Arduino.h>
#define RCVBUFFSIZE 2048
/**
 * HTTPクライアントのためのクラス
 */
class LpwaHttpClient {
  enum HttpRequestMethod {
    HTTP_REQUEST_METHOD_GET,
    HTTP_REQUEST_METHOD_POST,
    HTTP_REQUEST_METHOD_PUT,
    HTTP_REQUEST_METHOD_DELETE
  };
  String _host;
  uint16_t _port;
  bool _https;
  int _status;
  char _rcvBuffer[RCVBUFFSIZE];
  String _body;
  String _requestHeader;
  String _requestBody;
  void _close();

public:
  LpwaHttpClient(const char *host, uint16_t port, const bool https = false);
  LpwaHttpClient(String &host, uint16_t port, const bool https = false);
  ~LpwaHttpClient();
  int get(const char *path);
  int post(const char *path, String &contentType, String &body) {
    return post(path, contentType.c_str(), body.c_str());
  }
  int post(const char *path, const char *contentType, const char *body);
  int put(const char *path, String &contentType, String &body) {
    return put(path, contentType.c_str(), body.c_str());
  }
  int put(const char *path, const char *contentType, const char *body);
  int del(const char *path);
  int del(const char *path, String &contentType, String &body) {
    return del(path, contentType.c_str(), body.c_str());
  }
  int del(const char *path, const char *contentType, const char *body);
  int sendRequest(HttpRequestMethod method, const char *path);
  int responseStatusCode();
  String responseBody();
  void clrHeader();
  template <class T, class U> void addHeader(T name, U value);
  template <class T> void setBody(T body);

  int setCert(const char *cert);
};

#endif
