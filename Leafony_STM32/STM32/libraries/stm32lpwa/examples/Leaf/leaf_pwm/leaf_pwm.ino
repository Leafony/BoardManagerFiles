/*
 * PWM output sketch
 *
 * PWM:
 *  PWM_SP(PA1) -- Leaf pin 28
*/

// libraries
#include <Lpwa.h>
LpwaCtrl pmctrl;

#define PWM_PIN PWM_SP
#define PWM_FREQ 1000 // 1KHz

void setup() {
  pmctrl.enableLeaf(true);  // Leafの電源を入れる
  analogWriteFrequency(PWM_FREQ);
}

void loop() {
  for (int i=0;i<256;i++) {
    analogWrite(PWM_PIN, i);
    delay(100);  
  }
}
