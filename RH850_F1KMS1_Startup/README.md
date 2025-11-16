# RH850/F1KM-S1 Startup Code

RH850/F1KM-S1マイコン用のスタートアップコード一式

## 概要

このプロジェクトは、Renesas RH850/F1KM-S1マイコンのスタートアップコードを提供します。基本的なシステム初期化とアプリケーション実行環境を構築します。

## ファイル構成

```
RH850_F1KMS1_Startup/
├── src/
│   ├── cstartup.asm       # スタートアップアセンブリコード
│   ├── system_rh850.c     # システム初期化実装
│   └── main.c             # サンプルアプリケーション
├── inc/
│   └── system_rh850.h     # システム初期化ヘッダ
├── linker_script.ld       # リンカスクリプト
└── README.md              # このファイル
```

## 主な機能

### 1. スタートアップコード (cstartup.asm)
- **リセットベクタ**: アドレス0x00000000からの起動
- **スタック初期化**: RAMの最上位にスタックポインタを設定
- **BSSセクションクリア**: 未初期化変数領域のゼロクリア
- **データセクションコピー**: ROMからRAMへ初期化データをコピー
- **システム初期化呼び出し**: `SystemInit()`を実行
- **main関数へ分岐**: アプリケーション開始

### 2. リンカスクリプト (linker_script.ld)
- **メモリマップ定義**:
  - Code Flash: 0x00000000 - 0x001FFFFF (2MB)
  - Data Flash: 0x01000000 - 0x0100FFFF (64KB)
  - RAM: 0xFEDE8000 - 0xFEE27FFF (256KB)
- **スタックサイズ**: 8KB
- **ヒープサイズ**: 16KB

### 3. システム初期化 (system_rh850.c/h)
- **クロック設定**:
  - 外部クリスタル: 16MHz
  - PLL: 240MHz (16MHz × 15)
  - CPUクロック: 120MHz (PLL / 2)
  - ペリフェラルクロック: 60MHz (PLL / 4)
- **割り込みコントローラ初期化**: 全割り込みをデフォルト設定
- **ユーティリティ関数**: 遅延関数、クロック取得関数

### 4. サンプルアプリケーション (main.c)
- GPIO初期化 (P0_0をLED出力として設定)
- LED点滅 (500msごとにトグル)
- ループカウンタのインクリメント

## ビルド方法

### 必要なツール
- Renesas CS+ for CC または GHS MULTI (Green Hills Software)
- RH850用GCCツールチェーン (オープンソース版使用時)

### コンパイル例 (GCCの場合)
```bash
# アセンブリファイルのアセンブル
rh850-elf-as -mcpu=g3m -o cstartup.o src/cstartup.asm

# Cファイルのコンパイル
rh850-elf-gcc -mcpu=g3m -O2 -I inc -c src/system_rh850.c -o system_rh850.o
rh850-elf-gcc -mcpu=g3m -O2 -I inc -c src/main.c -o main.o

# リンク
rh850-elf-ld -T linker_script.ld cstartup.o system_rh850.o main.o -o firmware.elf

# HEXファイル生成
rh850-elf-objcopy -O ihex firmware.elf firmware.hex
```

## メモリマップ

| 領域 | 開始アドレス | 終了アドレス | サイズ | 用途 |
|------|-------------|-------------|--------|------|
| Code Flash | 0x00000000 | 0x001FFFFF | 2MB | プログラムコード |
| Data Flash | 0x01000000 | 0x0100FFFF | 64KB | データ保存 |
| RAM | 0xFEDE8000 | 0xFEE27FFF | 256KB | データ・スタック |

## クロック構成

```
外部クリスタル (16MHz)
    ↓
MainOSC (16MHz)
    ↓
PLL × 15
    ↓
PLL出力 (240MHz)
    ├─ CPU Clock ÷2 → 120MHz
    └─ Peripheral Clock ÷4 → 60MHz
```

## カスタマイズ方法

### スタックサイズの変更
`linker_script.ld`の以下の行を編集:
```c
__stack_size = 0x2000;      /* 8KB → 任意のサイズ */
```

### クロック周波数の変更
`system_rh850.c`の以下の定義を編集:
```c
#define PLL_MUL                 15      /* PLL倍率 */
#define CPU_DIV                 2       /* CPU分周比 */
#define PERIPHERAL_DIV          4       /* ペリフェラル分周比 */
```

### GPIO設定の変更
`main.c`の以下の定義を編集:
```c
#define LED_PORT_BASE           (0xFFC10000UL)  /* ポート番号 */
#define LED_PIN                 (0)             /* ピン番号 */
```

## 注意事項

1. **レジスタアドレス**: RH850/F1KM-S1のユーザーズマニュアルに基づいて設定されています
2. **クロック設定**: 実際のハードウェアに合わせて調整が必要な場合があります
3. **割り込みハンドラ**: 必要に応じて`cstartup.asm`の例外ベクタテーブルを拡張してください
4. **デバッグ**: JTAG/デバッガ経由での書き込み・デバッグを推奨

## ライセンス

このコードはサンプルとして提供されています。商用・非商用問わず自由に使用できます。

## 参考資料

- RH850/F1KM-S1 ユーザーズマニュアル ハードウェア編
- RH850 ファミリ ユーザーズマニュアル ソフトウェア編
- CS+ for CC ユーザーズガイド

---

**作成日**: 2025年11月16日  
**対象デバイス**: RH850/F1KM-S1
