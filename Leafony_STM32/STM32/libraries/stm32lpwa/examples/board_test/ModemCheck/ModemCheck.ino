/*
 * Modem Check
 *
*/

// libraries
#include <Lpwa.h>

void setup() {
  // initialize serial communications and wait for port to open:
  Serial.begin(115200);
  Serial3.begin(115200);
#ifdef USBD_USE_CDC
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
#endif //  USBD_USE_CDC
  Serial.println("Starting LPWA leaf modem check");

  // GPIO初期化
  pinMode(LTE_PWR_ON, OUTPUT);
  digitalWrite(LTE_PWR_ON, LOW);
  pinMode(STM_PWR_ON_N, OUTPUT);
  digitalWrite(STM_PWR_ON_N, HIGH);
  pinMode(STM_WAKE_UP, OUTPUT);
  digitalWrite(STM_WAKE_UP, HIGH);
  pinMode(STM_RST_OUT_N, OUTPUT);
  digitalWrite(STM_RST_OUT_N, LOW);
  pinMode(STM_USART3_RTS, OUTPUT);
  digitalWrite(STM_USART3_RTS, HIGH);
  delay(10);
  
  // モデム電源オン
  digitalWrite(LTE_PWR_ON, HIGH);
  delay(100);
  digitalWrite(STM_RST_OUT_N, HIGH);
  delay(100);

  digitalWrite(STM_USART3_RTS, LOW);
  Serial.println("\nModem access...");
}

void loop() {
  if (Serial.available()) {      // If anything comes in Serial (USB),
    Serial3.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
  }

  if (Serial3.available()) {     // If anything comes in Serial1 (pins 0 & 1)
    Serial.write(Serial3.read());   // read it and send it out Serial (USB)
  }
}
