/*
 * sensor_ak9754.cpp
 * Arduino library for AK9754 PIR Sensor
 *
 */
#include <Wire.h>

#include "sensor_ak9754.h"

/**
 * PIRライブラリのコンストラクタ
 * @param なし
 * @return なし
 */
Sensor_AK9754::Sensor_AK9754()
{
}

/**
 * PIRライブラリの初期化
 * @param address センサーのI2Cアドレス
 * @return なし
 */
void Sensor_AK9754::begin(uint8_t address) {
	_address = address;
	Wire.begin();
}

/**
 * PIR デバイスIDの取得
 * @param なし
 * @return デバイスID
 */
uint8_t Sensor_AK9754::readDeviceID() {
	uint8_t result = 0;
  result = readByte(AK9754_WIA2); 
	return result;
}

/**
 * PIR デバイスの初期設定
 * @param 検出ゲイン
 * @return なし
 */
void Sensor_AK9754::initPir(int gain) {
  writeByte(AK9754_CNTL1, 0xff); // RESET
  writeByte(AK9754_CNTL2, 0xfc); // SYNCM[1:0] << No sync (default 0xfc)
  writeByte(AK9754_CNTL3, 0xa9); // Low Noise mode, ODR=10Hz ,FC=0.9Hz (default 0xa9)
  writeByte(AK9754_CNTL4, 0xf8); // TOPT disable (default 0xff)
  writeByte(AK9754_CNTL5, 0x80); // TMP offset setting (default 0x80))
  writeByte(AK9754_CNTL6, 0xe0 + (gain & 0x1f)); // IR gain control
  writeByte(AK9754_CNTL7, 0xf0); // IR singal invert, ldle time (default 0xf0)
  writeByte(AK9754_CNTL8, 0x81); // Detection time = 1 (default 0x81)
  writeByte(AK9754_CNTL9, 0x2c); // Threshold Low (default 0x2c)
  writeByte(AK9754_CNTL10, 0x81); // Threshold High (default 0x81)
  writeByte(AK9754_CNTL11, 0xf2); // enable PIR and IRQ (HBDEN,DBDIEN)
  writeByte(AK9754_CNTL12, 0xff); // MODE=1
}

/**
 * PIR 検出待ち
 * @param 待ち時間(単位：秒)
 * @return 検出結果 true: 検出、false: タイムアウト
 */
bool Sensor_AK9754::waitPir(int timeout) {
  bool detect = false;
  uint8_t result;
  int timer = timeout * 10; // sec

  while ((timer > 0) && !detect) {
    result = readByte(AK9754_ST1);
    if ((result & 0x1) && (result & 0x10)) {
      detect = true;
    }
    // dummy read
    uint16_t val = readWord(AK9754_IRL);
    val = readWord(AK9754_TMPL);
    result = readByte(AK9754_ST2);

    // wait
    delay(100);
    timer--;
  }
  return detect;
}


void Sensor_AK9754::writeByte(int reg, byte value) {
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint16_t Sensor_AK9754::readWord(int reg) {
  uint16_t data;
  byte bytes[2];

  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(_address, 2);
  for (int i = 0; i < 2; i++) {
    bytes[i] = Wire.read();
  }
  data = bytes[1] << 8 + bytes[0];
  return data;
}

uint8_t Sensor_AK9754::readByte(int reg) {
  uint8_t data;
  byte bytes[1];

  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.endTransmission(false);

  Wire.requestFrom(_address, 1);
  for (int i = 0; i < 1; i++) {
    bytes[i] = Wire.read();
  }
  data = bytes[0];

  return data;
}
