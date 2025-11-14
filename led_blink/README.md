# Arduino UNO R4 MINIMA LED点滅プロジェクト

## 概要
Arduino UNO R4 MINIMAの内蔵LEDを100ms周期で点滅させ、SunFounder 20x4 I2C LCDディスプレイに状態を表示するプログラムです。

## 仕様
- **ボード**: Arduino UNO R4 MINIMA
- **使用ピン**: LED_BUILTIN (Pin 13)
- **点滅周期**: 100ms (ON: 50ms, OFF: 50ms)
- **周波数**: 10Hz
- **LCD**: SunFounder 20x4 I2C LCD (PCF8574ベース)
- **I2Cアドレス**: 0x27 (または0x3F)

## 必要な機材
- Arduino UNO R4 MINIMA x1
- USBケーブル (Type-C) x1
- SunFounder 20x4 I2C LCDディスプレイ x1
- ジャンパーワイヤ (メス-メス) x4

## 配線
| LCD Pin | Arduino Pin |
|---------|-------------|
| GND     | GND         |
| VCC     | 5V          |
| SDA     | SDA (A4)    |
| SCL     | SCL (A5)    |

## セットアップ手順

### 1. Arduino IDEのインストール
[Arduino公式サイト](https://www.arduino.cc/en/software)から最新版をダウンロードしてインストールしてください。

### 2. ボードの設定
1. Arduino IDEを起動
2. **ツール** → **ボード** → **ボードマネージャ**を開く
3. "Arduino UNO R4"で検索し、**Arduino UNO R4 Boards**をインストール
4. **ツール** → **ボード** → **Arduino UNO R4 Boards** → **Arduino UNO R4 Minima**を選択

### 3. ライブラリのインストール
1. **ツール** → **ライブラリを管理**を開く
2. "LiquidCrystal I2C"で検索
3. **LiquidCrystal_I2C by Martin Kubovčík** (v2.0.0)をインストール

### 4. プログラムのアップロード
1. Arduino UNO R4 MINIMAをUSBケーブルでPCに接続
2. **ツール** → **ポート**から適切なCOMポートを選択
3. `led_blink.ino`を開く
4. **アップロード**ボタン(→)をクリック

## 動作確認
アップロード完了後:
- ボード上の内蔵LED(オレンジ色)が100ms周期で点滅
- LCDに以下のメッセージが表示されます:
```
Arduino UNO R4
LED Blink 100ms
20x4 I2C LCD
Status: Running
```

## カスタマイズ

### I2Cアドレスの変更
LCDが検出されない場合、I2Cアドレスが異なる可能性があります:
```cpp
static const uint8_t LCD_I2C_ADDR = 0x3F;  // 0x27から0x3Fに変更
```

### LCDサイズの変更
16x2 LCDを使用する場合:
```cpp
static const uint8_t LCD_COLS = 16U;
static const uint8_t LCD_ROWS = 2U;
```

### 点滅速度の変更
```cpp
static const uint16_t DELAY_TIME_MS = 100U;  // 200ms周期に変更
```

## トラブルシューティング

### LEDが点滅しない
- USBケーブルが正しく接続されているか確認
- 正しいボードとポートが選択されているか確認
- プログラムが正常にアップロードされたか確認

### LCDに何も表示されない
- I2C配線を確認（SDA、SCL、VCC、GND）
- バックライトの明るさ調整（LCD背面のポテンショメータ）
- I2Cアドレスを0x27から0x3Fに変更して試す
- I2Cスキャナでアドレスを確認

### アップロードエラー
- ボードマネージャで"Arduino UNO R4 Boards"がインストールされているか確認
- 別のUSBポートを試す
- Arduino IDEを再起動

## 使用ライブラリ
- **LiquidCrystal_I2C** (v2.0.0) - I2C LCD制御用

## ライセンス
このプロジェクトはパブリックドメインです。自由に使用・改変できます。
