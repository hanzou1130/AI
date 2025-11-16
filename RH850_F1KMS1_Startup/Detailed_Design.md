**RH850/F1KM-S1 Detailed Design**

# 概要
この文書は RH850/F1KM-S1 向けに作成したスタートアップ一式（アセンブリ、リンカスクリプト、システム初期化、サンプルアプリ）についての詳細設計を示す。

**目的**: ブートからアプリケーション開始までの動作を明確化し、移植・保守・レビューを支援する。

**対象デバイス**: Renesas RH850/F1KM-S1

---

**ファイル一覧**
- `src/cstartup.asm` : スタートアップ（リセットベクタ、BSSクリア、データコピー、SystemInit 呼び出し）
- `linker_script.ld`  : メモリマップとセクション割当
- `src/system_rh850.c` : システム初期化（MainOSC/PLL/クロック分周、割り込み初期化）
- `inc/system_rh850.h` : システム初期化のヘッダ
- `inc/rh850_regs.h`   : レジスタ定義（例: MOSC/PLL/INTC）
- `src/main.c`        : サンプルアプリ（LED点滅）

---

**メモリマップ**
- Code Flash : 0x0000_0000 - 0x001F_FFFF (2MB)
- Data Flash : 0x0100_0000 - 0x0100_FFFF (64KB)
- RAM        : 0xFEDE_8000 - 0xFEE2_7FFF (256KB)

スタック、ヒープサイズ（リンカスクリプト）
- スタック: 8KB（`__stack_size = 0x2000`）
- ヒープ: 16KB（`__heap_size = 0x4000`）

---

**起動シーケンス（概要）**
1. ブートローダ/ROM によりリセットベクタへジャンプ（`_RESET`）
2. `cstartup.asm::_start` 実行
   - 割り込み無効化 (`di`)
   - スタックポインタ設定 (`SP_INIT` = 0xFEE28000)
   - BSS（`__sbss`〜`__ebss`）をゼロクリア
   - 初期化データ（ROM上の `__sdata_rom`）を RAM の `__sdata` へコピー (`__edata` まで)
   - `SystemInit()` を呼び出し
   - 割り込み有効化 (`ei`)
   - `main()` へ分岐

---

**例外ベクタ**
- `cstartup.asm` に簡易ベクタテーブルを置く（`__exception_table`）。現状はすべて `_dummy_exception` にループするダミーハンドラを割当て。
- 実運用では各ベクタに適切なハンドラを割り当て、必要に応じて優先度や割り込み設定を行うこと。

---

**BSS と Data 初期化アルゴリズム**
- BSS: `r6` を `__sbss`、`r7` を `__ebss` にしてワード単位で 0 を書き込む。ループ終了条件は `r6 >= r7`。
- Data: `r6` をソース `__sdata_rom`、`r7` をデスティネーション `__sdata`、`r8` を `__edata` にしてワードコピー。終了は `r7 >= r8`。
- アセンブリで明示的にワード (4 バイト) 単位で処理しているため、リンタやコンパイラの配置と一致させること。

---

**スタック & ヒープ**
- スタック初期アドレス: `SP_INIT = 0xFEE28000`（リンカスクリプトの RAM 範囲の上位に合わせている）
- ヒープはリンカスクリプト内で領域を確保。必要に応じてサイズを調整。

---

**SystemInit の詳細（`system_rh850.c`）**
- MainOSC を有効化（`MOSCE = 0x01`）して安定化待ち（ポーリング）
- PLL を有効化 & 安定化待ち（`PLLC0`/`PLLS0`）
- CPU クロック、ペリフェラルクロックの分周・切替（`CKSC` 系レジスタ）
- 割り込みコントローラ（EIC）を初期化して全チャネルをマスク＋最低優先度に設定
- グローバル変数 `g_system_clock`, `g_peripheral_clock` を設定

注意: レジスタ書き込みが保護されている領域の場合はプロテクション解除手順を踏む必要がある。

---

**主要レジスタとアドレス（サンプル）**
- `MOSCE` 0xFFF81100
- `MOSCS` 0xFFF81104
- `PLLC0` 0xFFF89000
- `PLLS0` 0xFFF89004
- `INTC1_BASE` 0xFFFC0000（EIC 配列）
- `PORT0` 基点 0xFFC10000

（詳細は `inc/rh850_regs.h` を参照）

---

**ビルド手順（参考/GCC クロスツールチェーン）**
```powershell
# アセンブル
rh850-elf-as -mcpu=g3m -o cstartup.o src/cstartup.asm
# コンパイル
rh850-elf-gcc -mcpu=g3m -O2 -I inc -c src/system_rh850.c -o system_rh850.o
rh850-elf-gcc -mcpu=g3m -O2 -I inc -c src/main.c -o main.o
# リンク
rh850-elf-ld -T linker_script.ld cstartup.o system_rh850.o main.o -o firmware.elf
# HEX 生成
rh850-elf-objcopy -O ihex firmware.elf firmware.hex
```

---

**検証/デバッグ手順**
- JTAG/デバッガで RAM と ROM に正しいイメージを書き込み、リセット後に `main` に到達するか確認
- BSS セクション開始アドレスと終了アドレスが正しいか確認（未初期化変数が 0 になるか）
- Data コピー元アドレス (`__sdata_rom`) がリンカで期待した ROM 上の位置になっているかを検証
- クロック設定後に `SystemGetCpuClock()` の返り値が期待値か確認

---

**カスタマイズ方法**
- スタックサイズ変更: `linker_script.ld` の `__stack_size` を更新
- クロック設定変更: `system_rh850.c` の `PLL_MUL`, `CPU_DIV`, `PERIPHERAL_DIV` を更新し、`SystemClockInit()` 内の設定を調整
- ベクタテーブル拡張: `cstartup.asm` の `__exception_table` に各ハンドララベルを追加

---

**注意点**
- レジスタアドレスは設計参照からコピーしたサンプル値。実際のリビジョン・ボードに合わせて確認が必要
- 実機テスト時はウォッチドッグや保護レジスタに注意すること
- 複数 `.ino` のような自動結合されるソースファイル配置に注意（Arduino 特有のルールは本プロジェクトには該当しないが、ファイル配置ポリシーを守ること）

---

**変更履歴**
- 2025-11-16 : 初版作成（スタートアップアセンブリ、リンカスクリプト、system init、サンプル main を反映）


<!-- End of Detailed Design -->
---

## フローチャート

以下は起動シーケンスのフローチャートです。ブートからアプリケーション開始（main()）までの主要ステップを図示しています。

![Startup Flowchart](diagrams/startup_flowchart.svg)

簡単な説明:
- リセットベクタから `_start` に入り、割り込みを無効化してスタックを初期化します。
- BSS をゼロクリアし、ROM 上の初期化データを RAM にコピーします。
- `SystemInit()` を実行してクロック等の初期化を行い、割り込みを有効化して `main()` に移行します。

