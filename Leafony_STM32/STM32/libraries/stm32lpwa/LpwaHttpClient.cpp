#include "LpwaHttpClient.h"

LpwaHttpClient::LpwaHttpClient(const char *host, uint16_t port, const bool https)
    : _host(host), _port(port), _https(https), _status(-1), _body(""), _requestHeader(""),
      _requestBody("") {}

LpwaHttpClient::LpwaHttpClient(String &host, uint16_t port, const bool https)
    : _host(host), _port(port), _https(https), _status(-1), _body(""), _requestHeader(""),
      _requestBody("") {}

LpwaHttpClient::~LpwaHttpClient() {
  _host.~String();
  _body.~String();
  _requestHeader.~String();
  _requestBody.~String();
}

/** Delete Session */
void LpwaHttpClient::_close() {
  _requestHeader = "";
  _requestBody = "";
  theLpwaModemCore.send("AT+KHTTPCLOSE=1,0");
  theLpwaModemCore.waitForResponse3("OK\r");
}

/**
 * GETリクエストを送信します。
 * @param path リクエストパス
 * @return ステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::get(const char *path) {
  return sendRequest(HTTP_REQUEST_METHOD_GET, path);
}

/**
 * POSTリクエストを送信します。
 * @param path リクエストパス
 * @param contentType Content-Type ヘッダー
 * @param body リクエストボディ
 * @return ステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::post(const char *path, const char *contentType,
                         const char *body) {
  addHeader("Content-Type", contentType);
  setBody(body);
  return sendRequest(HTTP_REQUEST_METHOD_POST, path);
}

/**
 * PUTリクエストを送信します。
 * @param path リクエストパス
 * @param contentType Content-Type ヘッダー
 * @param body リクエストボディ
 * @return ステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::put(const char *path, const char *contentType,
                        const char *body) {
  addHeader("Content-Type", contentType);
  setBody(body);
  return sendRequest(HTTP_REQUEST_METHOD_PUT, path);
}

/**
 * DELETEリクエストを送信します。
 * @param path リクエストパス
 * @return ステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::del(const char *path) {
  return sendRequest(HTTP_REQUEST_METHOD_DELETE, path);
}

/**
 * DELETEリクエストを送信します。
 * @param path リクエストパス
 * @param contentType Content-Type ヘッダー
 * @param body リクエストボディ
 * @return ステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::del(const char *path, const char *contentType,
                        const char *body) {
  addHeader("Content-Type", contentType);
  setBody(body);
  return sendRequest(HTTP_REQUEST_METHOD_DELETE, path);
}

/**
 * リクエストを送信します。
 * @param method HTTPメソッド ("GET", "POST", "PUT", "DELETE")
 * @param path リクエストパス
 * @return ステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::sendRequest(HttpRequestMethod method, const char *path) {
  const unsigned long start = millis();
  _status = -1;
  _body = "";

  int show_resp = 0 /* Whether to show HTTP headers */;

  if (_https) {
    theLpwaModemCore.sendf("AT+KHTTPCFG=1,\"%s\",%d,2", _host.c_str(), _port);
    if (theLpwaModemCore.waitForResponse3("OK\r",NULL,0,30000) < 0) {
      _close();
      return _status = -1;
    }
  } else {
    theLpwaModemCore.sendf("AT+KHTTPCFG=1,\"%s\",%d", _host.c_str(), _port);
    if (theLpwaModemCore.waitForResponse3("OK\r",NULL,0,10000) < 0) {
      _close();
      return _status = -1;
    }
  }

  if (_requestBody.length() > 0) {
    addHeader("Content-Length", _requestBody.length());
  }

  if (_requestHeader.length() > 0) {
    theLpwaModemCore.send("AT+KHTTPHEADER=1");
    if (theLpwaModemCore.waitForResponse3("CONNECT\r",NULL,0,10000) < 0) {
      _close();
      return _status = -1;
    }
    char reqArray[_requestHeader.length()+1];
    _requestHeader.toCharArray(reqArray, _requestHeader.length()+1);
    theLpwaModemCore.sendb((uint8_t *)reqArray, _requestHeader.length());
    theLpwaModemCore.printf("--EOF--Pattern--\r\n");
    theLpwaModemCore.send();
    if (theLpwaModemCore.waitForResponse3("OK\r",NULL,0,10000) < 0) {
      _close();
      return _status = -1;
    }
  }

  switch (method) {
  case HTTP_REQUEST_METHOD_GET:
    theLpwaModemCore.sendf("AT+KHTTPGET=1,\"%s\",%d", path, show_resp);
    break;
  case HTTP_REQUEST_METHOD_POST:
    theLpwaModemCore.sendf("AT+KHTTPPOST=1,,\"%s\",%d", path, show_resp);
    break;
  case HTTP_REQUEST_METHOD_PUT:
    theLpwaModemCore.sendf("AT+KHTTPPUT=1,,\"%s\",%d", path, show_resp);
    break;
  case HTTP_REQUEST_METHOD_DELETE:
    theLpwaModemCore.sendf("AT+KHTTPDELETE=1,\"%s\",%d", path, show_resp);
    break;
  default:
    _close();
    return _status = -1;
  }

  if (theLpwaModemCore.waitForResponse3("CONNECT\r",NULL,0,10000) < 0) {
    _close();
    return _status = -1;
  }

  if (_requestBody.length() > 0) {
    char reqArray[_requestBody.length()+1];
    _requestBody.toCharArray(reqArray, _requestBody.length()+1);
    theLpwaModemCore.sendb((uint8_t *)reqArray, _requestBody.length());
    theLpwaModemCore.printf("--EOF--Pattern--\r\n");
    theLpwaModemCore.send();
  }

  // NOTE: "--EOF--Pattern--" 到達まで待つ
  if (theLpwaModemCore.waitForResponse3("--EOF--Pattern--\r",_rcvBuffer,RCVBUFFSIZE,30000) < 0) {
    // NOTE: Timeout Error
    _close();
    return _status = -1;
  }
  if (theLpwaModemCore.waitForResponse3("OK\r",NULL,0,10000) < 0) {
    _close();
    return _status = -1;
  }
//  Serial.println("@@@@@@ HTTP response :");
//  Serial.println(_rcvBuffer);
  char *end_p = NULL;
  end_p = strstr(_rcvBuffer, "--EOF--Pattern--\r");
  if (end_p != NULL) {
    *end_p = 0x0; // 終端コードを追加
    _body = _rcvBuffer;
  } else {
    _body = "";
  }

  if (theLpwaModemCore.waitForResponse3("+KHTTP_IND:",_rcvBuffer,RCVBUFFSIZE,30000) < 0) {
    _close();
    return _status = -1;
  }

  switch (method) {
  case HTTP_REQUEST_METHOD_GET:
  case HTTP_REQUEST_METHOD_POST:
  case HTTP_REQUEST_METHOD_PUT:
  case HTTP_REQUEST_METHOD_DELETE:
    _status = theLpwaModemCore.getHttpResp();
//    Serial.println("@@@@@@ HTTP response code :");
//    Serial.println(_status);
    break;
  default:
    _status = -1;
  }

//  index = _body.indexOf("\r\n", index);
//  _body.remove(0, index + 2 /* length of "\r\n" */);
  _close();
  return _status;
}

/**
 * レスポンスに含まれるHTTPステータスコードを取得します。
 * @return HTTPステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::responseStatusCode() { return _status; }

/**
 * レスポンスボディを取得します。
 * @return レスポンスボディ (取得に失敗した場合: "")
 */
String LpwaHttpClient::responseBody() { return _body; }

/**
 * HTTPリクエストヘッダーを削除します。
 * @param なし
 */
void LpwaHttpClient::clrHeader() {
  _requestHeader = "";
}

/**
 * HTTPリクエストヘッダーを追加します。
 * @param name ヘッダーの名前
 * @param value ヘッダーの値
 */
template <class T, class U> void LpwaHttpClient::addHeader(T name, U value) {
  _requestHeader += name;
  _requestHeader += ':';
  _requestHeader += value;
  _requestHeader += "\r\n";
}

/**
 * HTTPリクエストボディを指定します。
 * @param body リクエストボディ
 */
template <class T> void LpwaHttpClient::setBody(T body) { _requestBody = body; }


/**
 * HTTPS証明書を登録します
 * @param cert 証明書データ
 * @return ステータスコード (取得に失敗した場合: -1)
 */
int LpwaHttpClient::setCert(const char *cert) {
  int len = strlen(cert);
//  Serial.println("@@@@@ setCert()enter");
  int retry = 3;
  for (int i=0;i<5;i++) {
    theLpwaModemCore.send("ATE0");
    theLpwaModemCore.waitForResponse3("OK\r");

    theLpwaModemCore.sendf("AT+KCERTSTORE=0,%d", len);
    if (theLpwaModemCore.waitForResponse3("CONNECT\r") >= 0) {
      if (len > 0) {
        const char* strEOF = "--EOF--Pattern--\n";
        theLpwaModemCore.sendb((uint8_t *)cert, len);
        theLpwaModemCore.sendb((uint8_t *)strEOF, strlen(strEOF));

        if (theLpwaModemCore.waitForResponse3("OK\r",NULL,0,5000) >= 0) {
          return 0;
        }
        Serial.print("<warn> retry store cert ");
        Serial.print(i);
        Serial.println("");
        delay(2000);
      }
    }
  }

  Serial.print("<error> failed store cert");
  return -1;
}
