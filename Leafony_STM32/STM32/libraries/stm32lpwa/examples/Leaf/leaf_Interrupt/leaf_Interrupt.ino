/*
 * leaf_interrupt.ino
 * LPWA leaf interrupt test sample
 * 
 * leafコネクタからの割り込み処理サンプル
 * ハードウェア割り込みが入ると指定したコールバックが呼ばれる
 * コールバック内でLEDを点灯
 * 1秒後にLEDが消灯する 
 * 割り込み入力は次のGPIO２本でGNDに接続すると割り込みが発生する
 * D2_INT_N(leaf pin22)
 * D3_INT_N(leaf pin24)
 * なお割り込み処理中は他の割り込み処理が行えないことからデバッグ文字出力等は行わないこと
 */

#include "Lpwa.h"

LpwaCtrl pmctrl;

int count = 0;
 
void setup() {
  pmctrl.enableLeaf(true);  // Leafの電源を入れる
  pmctrl.setLed(false, false, false);

  pinMode(D2_INT_N,INPUT_PULLUP);    // D2_INT_N(leaf pin22) プルアップ入力
  pinMode(D3_INT_N,INPUT_PULLUP);    // D3_INT_N(leaf pin24) プルアップ入力

//  pinMode(D2_INT_N,INPUT_PULLDOWN);    // D2_INT_N(leaf pin22) プルダウン入力
//  pinMode(D3_INT_N,INPUT_PULLDOWN);    // D3_INT_N(leaf pin24) プルダウン入力

  attachInterrupt(D2_INT_N, d2Irq, FALLING); // ピンの状態がHIGHからLOWに変わったときに発生
  attachInterrupt(D3_INT_N, d3Irq, FALLING); // ピンの状態がHIGHからLOWに変わったときに発生

//  attachInterrupt(D2_INT_N, d2Irq, RISING); // ピンの状態がLOWからHIGHに変わったときに発生
//  attachInterrupt(D3_INT_N, d3Irq, RISING); // ピンの状態がLOWからHIGHに変わったときに発生

//  attachInterrupt(D2_INT_N, d2Irq, CHANGE); // ピンの状態が変化したときに発生
//  attachInterrupt(D3_INT_N, d3Irq, CHANGE); // ピンの状態が変化したときに発生
}
 
void loop() {
  while(1){

    if (count > 0) {
      count--;
    } else {
      pmctrl.setLed(false, false, false);  // LED全消灯
    }
      delay(100);
    }
}

// 割り込みコールバック(D2_INT_N)
void d2Irq() {
    pmctrl.setLed(false, true, false); // 緑LED点灯
    count = 10;
}

// 割り込みコールバックD3_INT_N)
void d3Irq() {
    pmctrl.setLed(false, false, true); // 青LED全点灯
    count = 10;
}
