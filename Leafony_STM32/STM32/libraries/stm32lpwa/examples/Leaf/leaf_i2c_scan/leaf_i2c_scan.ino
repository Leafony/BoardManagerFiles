/*
 * leaf_i2c_scan.ino
 * LTE-M Leaf I2C bus scanner
 * 
 * primary bus:
 *  SDA -- Leaf F23
 *  SCL -- Leaf F25
 * secondary bus:
 *  SDA -- Leaf F14(A4)
 *  SCL -- Leaf F16(A5)
 */
#include <Wire.h>
#include "Lpwa.h"

LpwaCtrl pmctrl;

#define USE_PRIMARY_BUS
#define USE_ONBOARD_SENSOR
#define USE_LEAF

void setup() {
  Serial.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC

#ifdef USE_PRIMARY_BUS
  Serial.println("\nI2C Scanner(primary)");
  Wire.setSDA(I2C2_SDA);
  Wire.setSCL(I2C2_SCL);
#else
  Serial.println("\nI2C Scanner(secondary)");
  Wire.setSDA(I2C3_SDA);
  Wire.setSCL(I2C3_SCL);
#endif
  Wire.begin();
#ifdef USE_ONBOARD_SENSOR
  pmctrl.enableSensor(true); // LPWAリーフ上のセンサーを有効にする
#endif
#ifdef USE_LEAF
  pmctrl.enableLeaf(true);  // Leafの電源を入れる
#endif

}
 
void loop() {
  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
     if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);          
}
