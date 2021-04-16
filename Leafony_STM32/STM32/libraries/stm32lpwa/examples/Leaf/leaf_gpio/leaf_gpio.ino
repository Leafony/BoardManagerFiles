/*
 * GPIO test sketch
 *
 * GPIO:
 *  PC2 -- Leaf pin 6
 *  PE11 -- Leaf pin 7
 *  PD5 -- Leaf pin 8
 *  PE12 -- Leaf pin 9
 *  PD6 -- Leaf pin 10
 *  PB6 -- Leaf pin 11
 *  PC3 -- Leaf pin 12
 *  PB7 -- Leaf pin 13
 *  PC9 -- Leaf pin 14
 *  PA4 -- Leaf pin 15
 *  PA7 -- Leaf pin 16
 *  PE15 -- Leaf pin 17
 *  PE11 -- Leaf pin 19
 *  PA5 -- Leaf pin 21
 *  PE7 -- Leaf pin 22
 *  PB11 -- Leaf pin 23
 *  PE8 -- Leaf pin 24
 *  PB10 -- Leaf pin 25
 *  PE13 -- Leaf pin 26
 *  PA1 -- Leaf pin 28
*/

// libraries
#include <Lpwa.h>
LpwaCtrl pmctrl;

#define TEST_GPIO PC2 // Leaf pin 6
//#define TEST_GPIO PE11 // Leaf pin 7
//#define TEST_GPIO PD5 // Leaf pin 8
//#define TEST_GPIO PE12 // Leaf pin 9
//#define TEST_GPIO PD6 // Leaf pin 10
//#define TEST_GPIO PB6 // Leaf pin 11
//#define TEST_GPIO PC3 // Leaf pin 12
//#define TEST_GPIO PB7 // Leaf pin 13
//#define TEST_GPIO PC9 // Leaf pin 14
//#define TEST_GPIO PA4 // Leaf pin 15
//#define TEST_GPIO PA7 // Leaf pin 16
//#define TEST_GPIO PE15 // Leaf pin 17
//#define TEST_GPIO PE14 // Leaf pin 19
//#define TEST_GPIO PA5 // Leaf pin 21
//#define TEST_GPIO PE7 // Leaf pin 22
//#define TEST_GPIO PB11 // Leaf pin 23
//#define TEST_GPIO PE8 // Leaf pin 24
//#define TEST_GPIO PB10 // Leaf pin 25
//#define TEST_GPIO PE13 // Leaf pin 26
//#define TEST_GPIO PA1 // Leaf pin 28

void setup() {
  pinMode(TEST_GPIO, OUTPUT);
}

void loop() {
  digitalWrite(TEST_GPIO, HIGH);   // HIGH is the voltage level
  delay(1000);                       // wait for a second
  digitalWrite(TEST_GPIO, LOW);    // voltage LOW
  delay(1000);                       // wait for a second
}
