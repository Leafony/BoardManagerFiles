/*
 * lpwa_opt3001.ino
 * LTE-M Leaf onboard sensor(6DoF ICM20602) 
 * 
 */

#include <Wire.h>
#include "sensor_icm20602.h"
#include "Lpwa.h"

LpwaAccess lpwaAccess;
LpwaCtrl pmctrl;
Sensor_ICM20602 icm20602;

#define ICM20602_ADDRESS 0x68

void setup()
{
  Serial.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC
  Serial.println("Startup ICM20602");

  // set I2C pin
  Wire.setSDA(I2C2_SDA);
  Wire.setSCL(I2C2_SCL);

//  if (lpwaAccess.begin() == LPWA_READY) {
//    Serial.println("Leaf Power ON");
//  }

  pmctrl.setLed(true, true, true);
  pmctrl.enableSensor(true);

	icm20602.begin(ICM20602_ADDRESS);
	Serial.print("icm20602 Device ID :0x");
	Serial.println(icm20602.readDeviceID(),HEX);

  icm20602.beginAccel();
  icm20602.beginGyro();
}

void loop()
{
#if 1
  icm20602.updAccel();
  Serial.print("icm20602 Accel X:");
  Serial.print(icm20602.getAccelX());
  Serial.print(", Y:");
  Serial.print(icm20602.getAccelY());
  Serial.print(", Z:");
  Serial.print(icm20602.getAccelZ());
  Serial.print(", SQ:");
  Serial.print(icm20602.getAccelSqrt());
  Serial.println("");
#endif
#if 1
  icm20602.updGyro();
  Serial.print("icm20602 Gyro X:");
  Serial.print(icm20602.getGyroX());
  Serial.print(", Y:");
  Serial.print(icm20602.getGyroY());
  Serial.print(", Z:");
  Serial.print(icm20602.getGyroZ());
  Serial.println("");
#endif
#if 1
  Serial.print("TEMP:");
  Serial.println(icm20602.getTemp());
#endif
  delay(1000);
}
