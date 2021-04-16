/*

Arduino library for ICM20602 6DoF Sensor

*/
#include <Wire.h>
#include <math.h>

#include "sensor_icm20602.h"

/**
 * 6DoFライブラリのコンストラクタ
 * @param なし
 * @return なし
 */
Sensor_ICM20602::Sensor_ICM20602()
{
}

/**
 * 6DoFライブラリの初期化
 * @param address センサーのI2Cアドレス
 * @return なし
 */
void Sensor_ICM20602::begin(uint8_t address) {
	_address = address;

	Wire.begin();
  _writeByte(0x6b, 0x80); // reset 
  delay (1000);
  _writeByte(0x6B, 0x00); // PWR_MGMT_1
  _writeByte(0x6C, 0x00); // PWR_MGMT_2[none]
  _writeByte(0x1D, 0x09); // ACCEL_CONFIG2[ ACCEL_FCHOICE_B (RATE=4)
  _writeByte(0x1E, 0x80);
  _writeByte(0x19,0x0); // SMPLRT_DIV[]
  _writeByte(0x6B,0x03); // PWR_MGMT_1[CLKSEL=3]
}

/**
 * 6DoF 加速度センサの起動
 * @param mode 加速度レンジ
 * @return なし
 */
void Sensor_ICM20602::beginAccel(uint8_t mode) {
  switch(mode) {
  case ACC_FULL_SCALE_2_G:
    _accelRange = 2.0;
    break;
  case ACC_FULL_SCALE_4_G:
    _accelRange = 4.0;
    break;
  case ACC_FULL_SCALE_8_G:
    _accelRange = 8.0;
    break;
  case ACC_FULL_SCALE_16_G:
    _accelRange = 16.0;
    break;
  default:
    return; // Return without writing invalid mode
  }
  _writeByte(ACCEL_CONFIG, mode);
  delay(10);  
}

/**
 * 6DoF 加速度センサのデータ読み込み
 * @param なし
 * @return true:成功 false:失敗
 */
bool Sensor_ICM20602::updAccel() {
  return _readBytes(ACCEL_XOUT_H, 14, _accelBuf);
}

float Sensor_ICM20602::_accelGet(uint8_t highIndex, uint8_t lowIndex) {
  int16_t v = ((int16_t) _accelBuf[highIndex]) << 8 | _accelBuf[lowIndex];
  return ((float) -v) * _accelRange / (float) 0x8000; // (float) 0x8000 == 32768.0
}

/**
 * 6DoF 加速度センサのX値取得
 * @param なし
 * @return 加速度データ値
 */
float Sensor_ICM20602::getAccelX(void) {
  return _accelGet(0,1);
}

/**
 * 6DoF 加速度センサのY値取得
 * @param なし
 * @return 加速度データ値
 */
float Sensor_ICM20602::getAccelY(void) {
  return _accelGet(2,3);
}

/**
 * 6DoF 加速度センサのZ値取得
 * @param なし
 * @return 加速度データ値
 */
float Sensor_ICM20602::getAccelZ(void) {
  return _accelGet(4,5);
}

/**
 * 6DoF 加速度センサの平方根値取得
 * @param なし
 * @return SQRTデータ値
 */
float Sensor_ICM20602::getAccelSqrt(void) {
  return sqrt(pow(_accelGet(0, 1), 2) +
              pow(_accelGet(2, 3), 2) +
              pow(_accelGet(4, 5), 2));
}

/**
 * 6DoF ジャイロセンサの起動
 * @param 動作モード
 * @return なし
 */
void Sensor_ICM20602::beginGyro(uint8_t mode) {
  switch (mode) {
  case GYRO_FULL_SCALE_250_DPS:
    _gyroRange = 250.0;
    break;
  case GYRO_FULL_SCALE_500_DPS:
    _gyroRange = 500.0;
    break;
  case GYRO_FULL_SCALE_1000_DPS:
    _gyroRange = 1000.0;
    break;
  case GYRO_FULL_SCALE_2000_DPS:
    _gyroRange = 2000.0;
    break;
  default:
    return; // Return without writing invalid mode
  }
  _writeByte(GYRO_CONFIG, mode);
}

/**
 * 6DoF ジャイロセンサのデータ読み込み
 * @param なし
 * @return true:成功 false:失敗
 */
bool Sensor_ICM20602::updGyro() {
  return _readBytes(GYRO_XOUT_H, 6, _gyroBuf);
}

float Sensor_ICM20602::_gyroGet(uint8_t highIndex, uint8_t lowIndex) {
  int16_t v = ((int16_t) _gyroBuf[highIndex]) << 8 | _gyroBuf[lowIndex];
  return ((float) -v) * _gyroRange / (float) 0x8000;
}

/**
 * 6DoF ジャイロセンサのX値取得
 * @param なし
 * @return ジャイロ角速度データ値
 */
float Sensor_ICM20602::getGyroX(void) {
  return _gyroGet(0,1);
}

/**
 * 6DoF ジャイロセンサのY値取得
 * @param なし
 * @return ジャイロ角速度データ値
 */
float Sensor_ICM20602::getGyroY(void) {
  return _gyroGet(2,3);
}

/**
 * 6DoF ジャイロセンサのZ値取得
 * @param なし
 * @return ジャイロ角速度データ値
 */
float Sensor_ICM20602::getGyroZ(void) {
  return _gyroGet(4,5);
}


/**
 * 6DoF センサ内温度の取得
 * @param なし
 * @return 温度値
 */
float Sensor_ICM20602::getTemp(void) {
  uint8_t tempBuf[2];
  _readBytes(TEMP_OUT_H, 2, tempBuf);
  int16_t v = ((int16_t) tempBuf[0]) << 8 | tempBuf[0];
  return (float) v / 326.8 + 25.0;
}

/**
 * 6DoF センサのID取得
 * @param なし
 * @return ID値(ICM20602 = 0x12)
 */
uint8_t Sensor_ICM20602::readDeviceID() {
	uint8_t result = 0;
  result = _readByte(WHO_AM_I);
	return result;
}

void Sensor_ICM20602::_writeByte(int reg, byte value) {
  Wire.beginTransmission(_address);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission();
}

uint16_t Sensor_ICM20602::_readWord(int reg) {
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

uint8_t Sensor_ICM20602::_readByte(int reg) {
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

bool Sensor_ICM20602::_readBytes(int reg, uint8_t nBytes, uint8_t* data){
  Wire.beginTransmission(_address);
  Wire.write(reg);
  int retval = Wire.endTransmission(false);
  if (retval != 0) {
    return false;
  }

  Wire.requestFrom(_address, nBytes);
  for (int i = 0; i < nBytes; i++) {
    data[i] = Wire.read();
  }
  return true;
}
