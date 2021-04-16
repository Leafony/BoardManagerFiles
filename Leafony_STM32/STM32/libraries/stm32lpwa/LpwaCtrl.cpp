#include "LpwaCtrl.h"
  
LpwaCtrl::LpwaCtrl(){
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  pinMode(SENS_PWR_ON, OUTPUT);
  digitalWrite(SENS_PWR_ON, LOW);
  pinMode(PERI_PWR_ON, OUTPUT);
  digitalWrite(PERI_PWR_ON, LOW);
  pinMode(STM_RESET_N, OUTPUT);
  digitalWrite(STM_RESET_N, LOW);
  pinMode(DIP_SW1,INPUT);
  pinMode(DIP_SW2,INPUT);
  pinMode(DIP_SW3,INPUT);
  pinMode(DIP_SW4,INPUT);
  pinMode(USER_BTN,INPUT);
  pinMode(ADC_BAT, INPUT_ANALOG);
}

/**
 * LPWAリーフの消費電力制御を行う
 * @param state 電力制御ステート
 * @return なし
 */
void LpwaCtrl::powerDown(int state) {
  switch(state) {
  case LPWA_OFF: // power off
      Serial.println("<info> power-off LPWA device");
    theLpwaModemCore.power(false);
    break;
  case LPWA_NORMAL: // normal state
    // スリープ状態のときはスリープ解除
    if (theLpwaModemCore.status == LPWA_POWERDOWN) {
      Serial.println("<info> resume from SLEEP");
      theLpwaModemCore.sleep(0);
    }
    // 電源オフ状態の場合は電源オン制御
    if (theLpwaModemCore.status == LPWA_POWEROFF) {
      Serial.println("<info> power-on LPWA device");
      theLpwaModemCore.power(true);
      theLpwaModemCore.reset();
    }
    break;
  case LPWA_RESET: // reset
    Serial.println("<info> reset LPWA device");
    theLpwaModemCore.reset();
    break;
  case LPWA_SLEEP:  // SLEEP
    Serial.println("<info> enter SLEEP");
    theLpwaModemCore.sleep(1);
    break;
  case LPWA_L_HIBERNATE: // L_HIBERNATE
    Serial.println("<info> enter L_HIBERNATE");
    theLpwaModemCore.sleep(2);
    break;
  case LPWA_HIBERNATE: // HIBERNATE
    Serial.println("<info> enter HIBERNATE");
    theLpwaModemCore.sleep(3);
    break;
  }
}

/**
 * LPWAリーフのセンサを有効にする
 * @param enable 
 * @return なし
 */
void LpwaCtrl::enableSensor(bool enable) {
    if (enable) {
      digitalWrite(SENS_PWR_ON, HIGH);
    } else {
      digitalWrite(SENS_PWR_ON, LOW);
    }
}

/**
 * 外部リーフを有効にする
 * @param enable 
 * @return なし
 */
void LpwaCtrl::enableLeaf(bool enable) {
    if (enable) {
      digitalWrite(PERI_PWR_ON, HIGH);
      delay (500);
      digitalWrite(STM_RESET_N, HIGH);
    } else {
      digitalWrite(STM_RESET_N, LOW);
      digitalWrite(PERI_PWR_ON, LOW);
    }
}

/**
 * LPWAリーフのLED制御を行う
 * @param red 赤色LED
 * @param green 緑色LED
 * @param blue 青色LED
 * @return なし
 */
void LpwaCtrl::setLed(bool red, bool green, bool blue) {

  if (red) {
    digitalWrite(LED_RED, HIGH);
  } else {
    digitalWrite(LED_RED, LOW);
  }
  if (green) {
    digitalWrite(LED_GREEN, HIGH);
  } else {
    digitalWrite(LED_GREEN, LOW);
  }
  if (blue) {
    digitalWrite(LED_BLUE, HIGH);
  } else {
    digitalWrite(LED_BLUE, LOW);
  }
}

/**
 * LPWAリーフのLED制御を行う(個別制御)
 * @param led 対象LED選択(LED_RED,LED_GREEN,LED_BLUE)
 * @param enable 点灯選択(true:点灯 false:消灯)
 * @return なし
 */
void LpwaCtrl::setLed(int led, bool enable) {
  switch (led) {
  case LED_RED:
    if (enable) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
    break;
  case LED_GREEN:
    if (enable) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
    break;
  case LED_BLUE:
    if (enable) {
      digitalWrite(led, HIGH);
    } else {
      digitalWrite(led, LOW);
    }
    break;
  }

}

/**
 * LPWAリーフのバッテリ電圧を取得する
 * @param なし
 * @return 電圧値(単位mV)
 */
int LpwaCtrl::getBattLevel() {
  int retval = 0;
  retval += analogRead(ADC_BAT);
  delay(100);
  retval += analogRead(ADC_BAT);
  delay(100);
  retval += analogRead(ADC_BAT);
  delay(100);
  retval += analogRead(ADC_BAT);
  retval = (retval >>2);
  // BATT入力はバッテリ電圧の1/2
  // ADCは3.3Vフルスケールで10bit分解能
  // 3300*2 / 1024 -> 825/64
  retval = retval * 825 / 64;
  return retval >>1;
}
 
/**
 * LPWAリーフのDIP-SWを取得する
 * @param DIP-SWの番号
 * @return スイッチのON(true), OFF(false)
 */
bool LpwaCtrl::getDipSw(int dipsw) {
  bool retval = false;
  if ((dipsw <1) && (dipsw >4)) {
    return false;
  }
  switch(dipsw) {
  case 1:
    if (digitalRead(DIP_SW1) == 0)
      retval = true; 
    break;
  case 2:
    if (digitalRead(DIP_SW2) == 0)
      retval = true; 
    break;
  case 3:
    if (digitalRead(DIP_SW3) == 0)
      retval = true; 
    break;
  case 4:
    if (digitalRead(DIP_SW4) == 0)
      retval = true; 
    break;
  default:
    return false;
  }
  return retval;
}
