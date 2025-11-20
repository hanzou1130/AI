;******************************************************************************
; File Name    : cstartup.asm
; Device       : RH850/F1KM-S1
; Description  : Startup code for RH850/F1KM-S1
;                - Reset vector
;                - Stack initialization
;                - BSS section clear
;                - Data section copy
;                - System initialization call
;                - Branch to main
;******************************************************************************

;------------------------------------------------------------------------------
; Memory Configuration (RH850/F1KM-S1)
;------------------------------------------------------------------------------
; Code Flash : 0x00000000 - 0x001FFFFF (2MB)
; Data Flash : 0x01000000 - 0x0100FFFF (64KB)
; RAM        : 0xFEDE8000 - 0xFEE27FFF (256KB)
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; Register Definitions
;------------------------------------------------------------------------------
        .set    SP_INIT, 0xFEE28000      ; Stack pointer initial value (top of RAM)
        .set    WORD_SIZE, 4             ; Word size in bytes (32-bit)
        .set    EXC_TABLE_ALIGN, 512     ; Exception table alignment (bytes)
        .set    BSS_CLEAR_VALUE, 0       ; Value used to clear BSS
        
;------------------------------------------------------------------------------
; External Symbols
;------------------------------------------------------------------------------
        .extern _main
        .extern _SystemInit
        .extern _Scheduler_TickISR
        .extern __sdata
        .extern __edata
        .extern __sbss
        .extern __ebss
        .extern __sdata_rom

;------------------------------------------------------------------------------
; Section Definitions
;------------------------------------------------------------------------------
        .section ".reset", text
        .section ".text", text
        .section ".data", data
        .section ".bss", bss

;------------------------------------------------------------------------------
; Reset Vector
;------------------------------------------------------------------------------
        .section ".reset", text
        .align  WORD_SIZE
        .globl  _RESET
_RESET:
        jr      _start                    ; Jump to startup code

;------------------------------------------------------------------------------
; Exception Vector Table (Basic implementation)
;------------------------------------------------------------------------------
        .align  EXC_TABLE_ALIGN           ; Align to exception table alignment
        .globl  __exception_table
__exception_table:
        jr      _exception_handler          ; 0x00: RESET
        jr      _exception_handler          ; 0x10: SYSERR
        jr      _exception_handler          ; 0x20: reserved
        jr      _exception_handler          ; 0x30: FETRAP
        jr      _exception_handler          ; 0x40: TRAP0
        jr      _exception_handler          ; 0x50: TRAP1
        jr      _tick_handler               ; 0x60: RIE -> timer tick handler
        jr      _exception_handler          ; 0x70: reserved
        jr      _exception_handler          ; 0x80: reserved
        jr      _exception_handler          ; 0x90: reserved
        jr      _exception_handler          ; 0xA0: reserved
        jr      _exception_handler          ; 0xB0: reserved
        jr      _exception_handler          ; 0xC0: reserved
        jr      _exception_handler          ; 0xD0: reserved
        jr      _exception_handler          ; 0xE0: reserved
        jr      _exception_handler          ; 0xF0: reserved

;------------------------------------------------------------------------------
; Startup Code
;------------------------------------------------------------------------------
        .section ".text", text
        .align  WORD_SIZE
        .globl  _start
_start:
        ; Disable interrupts
        di

        ; Initialize stack pointer
        movhi   hi(SP_INIT), r0, r3
        movea   lo(SP_INIT), r3, sp

        ; Clear BSS section (RAM の未初期化領域を 0 で埋める)
        ; - 目的: グローバル/静的変数の既知の初期値(0)を保証するため
        ; - 実装: 32ビット (ワード = 32bit) 単位でクリアする (st.w を使用)
        ; - レジスタ: r6 = 現在の書き込み先アドレス(__sbss)
        ;           r7 = BSS 終端アドレス(__ebss)
        ;           r0 = 0 (クリア値)
        ; - 注意: アドレスは 32bit 境界に揃っていることを前提とする
        ;         __sbss / __ebss はリンカスクリプトで定義されるため
        ;         範囲外アクセスが発生しないことを確認すること
        movhi   hi(__sbss), r0, r6        ; r6 = start of BSS
        movea   lo(__sbss), r6, r6
        movhi   hi(__ebss), r0, r7        ; r7 = end of BSS
        movea   lo(__ebss), r7, r7
        br      _bss_check
_bss_loop:
        st.w    r0, 0[r6]                 ; Clear word (zero the RAM)
        add     WORD_SIZE, r6             ; Next word (word size = WORD_SIZE)
_bss_check:
        cmp     r7, r6                    ; Done? (r6 >= r7)
        bl      _bss_loop                 ; Loop if r6 < r7

        ; Copy initialized data from ROM to RAM
        ; - 実装: 32ビット (ワード = 32bit) 単位でコピー (ld.w / st.w を使用)
        ; - 最適化: 2 ワードずつアンローリングして分岐オーバーヘッドを削減
        ; - レジスタ: r6 = source (ROM), r7 = destination (RAM), r8 = end (address)
        movhi   hi(__sdata_rom), r0, r6   ; r6 = source (ROM)
        movea   lo(__sdata_rom), r6, r6
        movhi   hi(__sdata), r0, r7       ; r7 = destination (RAM)
        movea   lo(__sdata), r7, r7
        movhi   hi(__edata), r0, r8       ; r8 = end of data
        movea   lo(__edata), r8, r8
        br      _data_check
_data_loop:
        ld.w    0[r6], r9                 ; Load from ROM
        st.w    r9, 0[r7]                 ; Store to RAM
        add     WORD_SIZE, r6             ; Next source
        add     WORD_SIZE, r7             ; Next destination
_data_check:
        cmp     r8, r7                    ; Check if done
        bl      _data_loop                ; Loop if r7 < r8

        ; Call system initialization
        jarl    _SystemInit, lp

        ; Enable interrupts
        ei

        ; Branch to main
        movhi   hi(_main), r0, r6
        movea   lo(_main), r6, r6
        jmp     [r6]

;------------------------------------------------------------------------------
; Dummy Exception Handler
;------------------------------------------------------------------------------
        .align  WORD_SIZE
_exception_handler:
        br      _exception_handler          ; Infinite loop

/*
 * Simple timer tick handler entry used by example scheduler.
 * This handler calls the C function `Scheduler_TickISR` and returns from
 * interrupt. On RH850 the appropriate return instruction is `rte`.
 */
        .align  WORD_SIZE
        .globl  _tick_handler
_tick_handler:
        ; call C tick function
        jarl    _Scheduler_TickISR, lp
        rte

;------------------------------------------------------------------------------
; End of File
;------------------------------------------------------------------------------
