#ifndef LPWA_TYPES_H_
#define LPWA_TYPES_H_

/**
 * モデムとネットワークの状態
 */
enum NetworkStatus {
  /** 内部エラー */
  LPWA_FAIL,
  /** 初期状態 */
  LPWA_IDLE,
  /** LPWAモジュールの準備完了 */
  LPWA_READY,
  /** GPRS接続の準備完了 */
  GPRS_READY,
  /** LPWAモジュールの電源オフ */
  LPWA_POWEROFF,

  /** LPWAモジュール省電力状態 */
  LPWA_POWERDOWN
};

#endif
