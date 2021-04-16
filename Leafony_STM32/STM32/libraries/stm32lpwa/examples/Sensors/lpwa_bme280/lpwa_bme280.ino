/*
 * lpwa_bme280.ino
 * LTE-M Leaf onboard sensor(BME280) 
 * 
 * このサンプルコードは次のライブラリをインストールしてください
 * ・Grove - Barometer Sensor (BME280)
 * https://github.com/Seeed-Studio/Grove_BME280
 * インストール後にSeeed_BME280.hを次のように編集
 * #define BME280_ADDRESS   0x76
 */

#include "Seeed_BME280.h"
#include <Wire.h>
#include "Lpwa.h"

LpwaAccess lpwaAccess;
LpwaCtrl pmctrl;
BME280 bme280;

void setup()
{
  Serial.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC
  Serial.println("Startup");

  // set I2C pin
  Wire.setSDA(I2C2_SDA);
  Wire.setSCL(I2C2_SCL);
  
//  if (lpwaAccess.begin() == LPWA_READY) {
//    Serial.println("Leaf Power ON");
//  }

  // enable onboard sensor
  pmctrl.enableSensor(true);

  if(!bme280.init()){
    Serial.println("Device error!");
  }
}

void loop()
{
  float pressure;
  
  //get and print temperatures
  Serial.print("Temp: ");
  Serial.print(bme280.getTemperature());
  Serial.println("C");//The unit for  Celsius because original arduino don't support speical symbols
  
  //get and print atmospheric pressure data
  Serial.print("Pressure: ");
  Serial.print(pressure = bme280.getPressure());
  Serial.println("Pa");

  //get and print altitude data
  Serial.print("Altitude: ");
  Serial.print(bme280.calcAltitude(pressure));
  Serial.println("m");

  //get and print humidity data
  Serial.print("Humidity: ");
  Serial.print(bme280.getHumidity());
  Serial.println("%");

  delay(1000);
}
