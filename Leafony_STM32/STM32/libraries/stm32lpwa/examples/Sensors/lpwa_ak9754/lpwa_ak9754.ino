/*
 * LTE-M Leaf onboard sensor demo
 * AK9754 PIR sensor
 *
 */

#include <Wire.h>
#include "sensor_ak9754.h"
#include "Lpwa.h"

LpwaCtrl pmctrl;
Sensor_AK9754 ak9754;

#define AK9754_ADDRESS 0x61

void setup()
{
  Serial.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC
  Serial.println("Starting sensor test.");

  // set I2C pin
  Wire.setSDA(I2C2_SDA);
  Wire.setSCL(I2C2_SCL);

  pmctrl.setLed(false, true, false);
  pmctrl.enableSensor(true);
  delay(10); // Waiting for sensor-startup from power on

  ak9754.begin(AK9754_ADDRESS);
  Serial.print("ak9754 Device ID :0x");
  Serial.println(ak9754.readDeviceID(),HEX);

//  ak9754.initPir(AK9754_GAIN205);
  ak9754.initPir(AK9754_GAIN100);
}

void loop()
{
  if (ak9754.waitPir(1)) {
    pmctrl.setLed(true, false, false);
    Serial.println("PIR detect");
  } else {
    pmctrl.setLed(false, true, false);
  }
}
