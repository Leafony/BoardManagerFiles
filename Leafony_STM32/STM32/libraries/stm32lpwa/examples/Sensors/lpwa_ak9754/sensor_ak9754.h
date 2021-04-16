/*
 * sensor_ak9754.h
 * Arduino library for AK9754 PIR Sensor
 *
 */
 
#ifndef SENSOR_AK9754
#define SENSOR_AK9754

#include <Arduino.h>

#define AK9754_WIA1  0x00 // Company code 0x48
#define AK9754_WIA2  0x01 // Device ID 0x15
#define AK9754_INFO1 0x02
#define AK9754_INFO2 0x03
#define AK9754_ST1   0x04  // Detection result
#define AK9754_IRL   0x05
#define AK9754_IRH   0x06
#define AK9754_TMPL  0x07
#define AK9754_TMPH  0x08
#define AK9754_ST2   0x09
#define AK9754_ST3   0x0a
#define AK9754_SB0L  0x0b
#define AK9754_SB0H  0x0c
#define AK9754_SB1L  0x0d
#define AK9754_SB1H  0x0e
#define AK9754_SB2L  0x0f
#define AK9754_SB2H  0x10
#define AK9754_SB3L  0x11
#define AK9754_SB3H  0x12
#define AK9754_SB4L  0x13
#define AK9754_SB4H  0x14
#define AK9754_SB5L  0x15
#define AK9754_SB5H  0x16
#define AK9754_SB6L  0x17
#define AK9754_SB6H  0x18
#define AK9754_SB7L  0x19
#define AK9754_SB7H  0x1a
#define AK9754_SB8L  0x1b
#define AK9754_SB8H  0x1c
#define AK9754_SB9L  0x1d
#define AK9754_SB9H  0x1e
#define AK9754_ST4   0x1F

#define AK9754_CNTL1 0x20
#define AK9754_CNTL2 0x21
#define AK9754_CNTL3 0x22
#define AK9754_CNTL4 0x23
#define AK9754_CNTL5 0x24
#define AK9754_CNTL6 0x25
#define AK9754_CNTL7 0x26
#define AK9754_CNTL8 0x27
#define AK9754_CNTL9 0x28
#define AK9754_CNTL10 0x29
#define AK9754_CNTL11 0x2a
#define AK9754_CNTL12 0x2b

#define AK9754_GAIN50  0x10
#define AK9754_GAIN55  0x11
#define AK9754_GAIN60  0x12
#define AK9754_GAIN65  0x13
#define AK9754_GAIN70  0x14
#define AK9754_GAIN75  0x15
#define AK9754_GAIN80  0x16
#define AK9754_GAIN85  0x17
#define AK9754_GAIN90  0x18
#define AK9754_GAIN95  0x19
#define AK9754_GAIN100 0x1a
#define AK9754_GAIN105 0x1b
#define AK9754_GAIN110 0x1c
#define AK9754_GAIN115 0x1d
#define AK9754_GAIN120 0x1e
#define AK9754_GAIN125 0x1f
#define AK9754_GAIN130 0x00
#define AK9754_GAIN135 0x01
#define AK9754_GAIN140 0x02
#define AK9754_GAIN145 0x03
#define AK9754_GAIN150 0x04
#define AK9754_GAIN155 0x05
#define AK9754_GAIN160 0x06
#define AK9754_GAIN165 0x07
#define AK9754_GAIN170 0x08
#define AK9754_GAIN175 0x09
#define AK9754_GAIN180 0x0a
#define AK9754_GAIN185 0x0b
#define AK9754_GAIN190 0x0c
#define AK9754_GAIN195 0x0d
#define AK9754_GAIN200 0x0e
#define AK9754_GAIN205 0x0f


class Sensor_AK9754 {
public:
	Sensor_AK9754();

	void begin(uint8_t address);
	uint8_t readDeviceID();
  void initPir(int gain);
  bool waitPir(int timeout);

private:
	uint8_t _address;
  void writeByte(int reg, byte value);
  uint16_t readWord(int reg);
  uint8_t readByte(int reg);
};

#endif // SENSOR_AK9754
