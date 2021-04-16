/*
 * Serial Loopback sketch
 *
 * USART1:
 *  TX(PB_6) -- Leaf pin 11
 *  RX(PB_7) -- Leaf pin 13
 * USART2:
 *  TX(PD_5) -- Leaf pin 8
 *  RX(PD_6) -- Leaf pin 10
*/

// libraries
#include <Lpwa.h>
LpwaCtrl pmctrl;

HardwareSerial Serial1(PB_7, PB_6);
HardwareSerial Serial2(PD_6, PD_5);

void setup() {
  pmctrl.enableLeaf(true);  // Leafの電源を入れる
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial1.println("Starting Serial loopback - serial 1");
  Serial2.println("Starting Serial loopback - serial 2");
}

void loop() {
  if (Serial1.available()) {
    Serial1.write(Serial1.read());
  }

  if (Serial2.available()) {
    Serial2.write(Serial2.read());
  }
}
