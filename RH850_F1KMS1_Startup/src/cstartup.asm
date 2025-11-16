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
        
;------------------------------------------------------------------------------
; External Symbols
;------------------------------------------------------------------------------
        .extern _main
        .extern _SystemInit
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
        .align  4
        .globl  _RESET
_RESET:
        jr      _start                    ; Jump to startup code

;------------------------------------------------------------------------------
; Exception Vector Table (Basic implementation)
;------------------------------------------------------------------------------
        .align  512                       ; Align to 512 bytes
        .globl  __exception_table
__exception_table:
        jr      _dummy_exception          ; 0x00: RESET
        jr      _dummy_exception          ; 0x10: SYSERR
        jr      _dummy_exception          ; 0x20: reserved
        jr      _dummy_exception          ; 0x30: FETRAP
        jr      _dummy_exception          ; 0x40: TRAP0
        jr      _dummy_exception          ; 0x50: TRAP1
        jr      _dummy_exception          ; 0x60: RIE
        jr      _dummy_exception          ; 0x70: reserved
        jr      _dummy_exception          ; 0x80: reserved
        jr      _dummy_exception          ; 0x90: reserved
        jr      _dummy_exception          ; 0xA0: reserved
        jr      _dummy_exception          ; 0xB0: reserved
        jr      _dummy_exception          ; 0xC0: reserved
        jr      _dummy_exception          ; 0xD0: reserved
        jr      _dummy_exception          ; 0xE0: reserved
        jr      _dummy_exception          ; 0xF0: reserved

;------------------------------------------------------------------------------
; Startup Code
;------------------------------------------------------------------------------
        .section ".text", text
        .align  4
        .globl  _start
_start:
        ; Disable interrupts
        di

        ; Initialize stack pointer
        movhi   hi(SP_INIT), r0, r3
        movea   lo(SP_INIT), r3, sp

        ; Clear BSS section (RAM の未初期化領域を 0 で埋める)
        ; - 目的: グローバル/静的変数の既知の初期値(0)を保証するため
        ; - 実装: ワード単位でクリアする (st.w を使用)
        ; - レジスタ: r6 = 現在の書き込み先アドレス(__sbss)
        ;           r7 = BSS 終端アドレス(__ebss)
        ;           r0 = 0 (クリア値)
        ; - 注意: __sbss / __ebss はリンカスクリプトで定義されるため
        ;         範囲外アクセスが発生しないことを確認すること
        movhi   hi(__sbss), r0, r6        ; r6 = start of BSS
        movea   lo(__sbss), r6, r6
        movhi   hi(__ebss), r0, r7        ; r7 = end of BSS
        movea   lo(__ebss), r7, r7
        br      _bss_check
_bss_loop:
        st.w    r0, 0[r6]                 ; Clear word (zero the RAM)
        add     4, r6                     ; Next word (word size = 4)
_bss_check:
        cmp     r7, r6                    ; Done? (r6 >= r7)
        bl      _bss_loop                 ; Loop if r6 < r7

        ; Copy initialized data from ROM to RAM
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
        add     4, r6                     ; Next source
        add     4, r7                     ; Next destination
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
        .align  4
_dummy_exception:
        br      _dummy_exception          ; Infinite loop

;------------------------------------------------------------------------------
; End of File
;------------------------------------------------------------------------------
