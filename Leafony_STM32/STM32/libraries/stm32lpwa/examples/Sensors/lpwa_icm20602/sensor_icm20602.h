/*

Arduino library for AK9754  Sensor

*/

#ifndef SENSOR_ICM20602
#define SENSOR_ICM20602

#include <Arduino.h>

#define XG_OFFS_TC_H 4
#define XG_OFFS_TC_L 5
#define YG_OFFS_TC_H 7
#define YG_OFFS_TC_L 8
#define ZG_OFFS_TC_H 0x0a
#define ZG_OFFS_TC_L 0x0b
#define SELF_TEST_X_ACCEL 0x0d
#define SELF_TEST_Y_ACCEL 0x0e
#define XG_OFFS_USRH 0x13
#define XG_OFFS_USRL 0x14
#define YG_OFFS_USRH 0x15
#define YG_OFFS_USRL 0x16
#define ZG_OFFS_USRH 0x17
#define ZG_OFFS_USRL 0x18
#define SMPLRT_DIV 0x19
#define CONFIG 0x1a
#define GYRO_CONFIG 0x1b
#define ACCEL_CONFIG 0x1c
#define ACCEL_CONFIG2 0x1d
#define LP_MODE_CFG 0x1e
#define ACCEL_WOM_X_THR 0x20
#define ACCEL_WOM_Y_THR 0x21
#define ACCEL_WOM_Z_THR 0x22
#define FIFO_EN 0x23
#define FSYNC_INT 0x36
#define INT_PIN_CFG 0x37
#define INT_ENABLE 0x38
#define FIFO_WM_INT_STATUS 0x39
#define INT_STATUS 0x3a
#define ACCEL_XOUT_H 0x3b
#define ACCEL_XOUT_L 0x3c
#define ACCEL_YOUT_H 0x3d
#define ACCEL_YOUT_L 0x3e
#define ACCEL_ZOUT_H 0x3f
#define ACCEL_ZOUT_L 0x40
#define TEMP_OUT_H 0x41
#define TEMP_OUT_L 0x42
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48
#define SELF_TEST_X_GYRO 0x50
#define SELF_TEST_Y_GYRO 0x51
#define SELF_TEST_Z_GYRO 0x52
#define FIFO_WM_TH1 0x60
#define FIFO_WM_TH2 0x61
#define SIGNAL_PATH_RESET 68
#define ACCEL_INTEL_CTRL 0x69
#define USER_CTRL 0x6a
#define PWR_MGMT_1 0x6b
#define PWR_MGMT_2 0x6c
#define I2C_IF 0x70
#define FIFO_COUNTH 0x72
#define FIFO_COUNTL 0x73
#define FIFO_R_W 0x74
#define WHO_AM_I 0x75 // read only fixed value 0x12
#define XA_OFFSET_H 0x77
#define XA_OFFSET_L 0x78
#define YA_OFFSET_H 0x7a
#define YA_OFFSET_L 0x7b
#define ZA_OFFSET_H 0x7d
#define ZA_OFFSET_L 0x7e

// ACCEL_CONFIG reg28
#define ACC_FULL_SCALE_2_G       0x00
#define ACC_FULL_SCALE_4_G       0x08
#define ACC_FULL_SCALE_8_G       0x10
#define ACC_FULL_SCALE_16_G      0x18

// GYRO_CONFIG reg27
#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

class Sensor_ICM20602 {
public:
	Sensor_ICM20602();

	void begin(uint8_t address);
  uint8_t readDeviceID();
  
  void beginAccel(uint8_t mode = ACC_FULL_SCALE_16_G);
  bool updAccel();
  float getAccelX(void);
  float getAccelY(void);
  float getAccelZ(void);
  float getAccelSqrt(void);

  void beginGyro(uint8_t mode = GYRO_FULL_SCALE_2000_DPS);
  bool updGyro();
  float getGyroX(void);
  float getGyroY(void);
  float getGyroZ(void);

  float getTemp(void);

private:
  uint8_t _address;
  float _accelRange;
  uint8_t _accelBuf[14];
  float _gyroRange;
  uint8_t _gyroBuf[6];

  float _accelGet(uint8_t highIndex, uint8_t lowIndex);
  float _gyroGet(uint8_t highIndex, uint8_t lowIndex);

  void _writeByte(int reg, byte value);
  uint16_t _readWord(int reg);
  uint8_t _readByte(int reg);
  bool _readBytes(int reg, uint8_t nBytes, uint8_t* data);
};

#endif // SENSOR_ICM20602
