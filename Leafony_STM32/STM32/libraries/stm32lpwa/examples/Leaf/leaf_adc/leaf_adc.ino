/*
 * Analog Input
 *
 *
 * ADC:
 *  A0_IN -- Leaf pin 6
 *  A3_IN -- Leaf pin 12


*/
#include <Lpwa.h>
LpwaCtrl pmctrl;

// #define ADC_INPUT A0_IN // leaf pin 6
#define ADC_INPUT A3_IN // leaf pin 12

void setup() {
  Serial.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC

  Serial.println("\nADC test");
  pmctrl.enableLeaf(true);  // Leafの電源を入れる
}

void loop() {
  int adc_val = analogRead(ADC_INPUT);
  // ADCは3.3Vフルスケールで10bit分解能
  // 3300 / 1024 -> 207/64
  int mvolt = adc_val * 207 / 64;
  Serial.print("\nADC val=");
  Serial.print(adc_val);
  Serial.print(" volt=");
  Serial.print(mvolt);
  Serial.println(" mV");
  delay(1000);
  
}
