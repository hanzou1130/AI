// Arduino UNO R4 MINIMA - 20x4 I2C LCD Display Demo
// SunFounder 20x4 I2C LCDにカウンタを表示
// 1秒ごとにカウンタをインクリメントして4行目に表示

#include <Arduino.h>
#include "led_blink.h"

// 定数定義
static const uint8_t LED_PIN        = LED_BUILTIN;                              // LED出力ピン
static const uint16_t DELAY_TIME_MS = 50U;                                      // 遅延時間 (ミリ秒)
static const uint8_t LCD_I2C_ADDR   = 0x27;                                     // LCDのI2Cアドレス (通常0x27または0x3F)
static const uint8_t LCD_COLS       = 20U;                                      // LCD列数
static const uint8_t LCD_ROWS       = 4U;                                       // LCD行数

// グローバルオブジェクト
LiquidCrystal_I2C lcd ( LCD_I2C_ADDR, LCD_COLS, LCD_ROWS );                     // LCDオブジェクト

// グローバル変数
static uint32_t loopCounter         = 0U;                                       // ループカウンタ

// 実装部
void setup ( void )
{
  pinMode       ( LED_PIN, OUTPUT );                                            // LEDピンを出力モードに設定

  // LCD初期化
  lcd.init      ();                                                             // LCD初期化（UNO R4対応）
  lcd.backlight ();                                                             // バックライトON
  lcd.clear     ();                                                             // 画面クリア

  // 起動メッセージ表示
  lcd.setCursor ( 0, 0 );                                                       // 1行目
  lcd.print     ( "Arduino UNO R4" );
  lcd.setCursor ( 0, 1 );                                                       // 2行目
  lcd.print     ( "LED Blink 100ms" );
  lcd.setCursor ( 0, 2 );                                                       // 3行目
  lcd.print     ( "20x4 I2C LCD" );
  lcd.setCursor ( 0, 3 );                                                       // 4行目
  lcd.print     ( "Status: Running" );
}

void loop ( void )
{
  // ループカウンタをインクリメント
  loopCounter++;

  // LCD表示更新（4行目にカウンタ表示）
  lcd.setCursor ( 0, 3 );                                                       // 4行目
  lcd.print     ( "Count: " );
  lcd.print     ( loopCounter );
  lcd.print     ( "        " );                                                 // 余分な文字をクリア

  // 待機
  delay         ( 1000 );                                                       // 1秒待機
}
