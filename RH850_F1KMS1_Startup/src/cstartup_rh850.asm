;******************************************************************************
; File Name    : cstartup_csplus.asm
; Device       : RH850/F1KM-S1  
; Description  : Startup code for CS+ CC-RH (Renesas assembler syntax)
;******************************************************************************

SP_INIT         .equ    0xFEE28000
WORD_SIZE       .equ    4

        .extern _main
        .extern _SystemInit
        .extern _Scheduler_TickISR
        .extern __sbss
        .extern __ebss
        .extern __sdata
        .extern __edata
        .extern __sdata_rom

;------------------------------------------------------------------------------
; Reset Vector
;------------------------------------------------------------------------------
        .section        .reset,text
        .align  4
        .public _RESET
_RESET:
        jr      _start

;------------------------------------------------------------------------------
; Exception Vectors
;------------------------------------------------------------------------------
        .align  512
        .public __exception_table
__exception_table:
        jr      _exception_handler          ; 0x00
        jr      _exception_handler          ; 0x10
        jr      _exception_handler          ; 0x20
        jr      _exception_handler          ; 0x30
        jr      _exception_handler          ; 0x40
        jr      _exception_handler          ; 0x50
        jr      _tick_handler               ; 0x60 timer tick
        jr      _exception_handler          ; 0x70
        jr      _exception_handler          ; 0x80
        jr      _exception_handler          ; 0x90
        jr      _exception_handler          ; 0xA0
        jr      _exception_handler          ; 0xB0
        jr      _exception_handler          ; 0xC0
        jr      _exception_handler          ; 0xD0
        jr      _exception_handler          ; 0xE0
        jr      _exception_handler          ; 0xF0

;------------------------------------------------------------------------------
; Startup
;------------------------------------------------------------------------------
        .section        .text,text
        .align  4
        .public _start
_start:
        di
        movhi   #hilo(SP_INIT, 1), r0, r3
        movea   #hilo(SP_INIT, 0), r3, sp

        ; Clear BSS
        movhi   #hilo(__sbss, 1), r0, r6
        movea   #hilo(__sbss, 0), r6, r6
        movhi   #hilo(__ebss, 1), r0, r7
        movea   #hilo(__ebss, 0), r7, r7
        br      bss_check
bss_loop:
        st.w    r0, 0[r6]
        add     WORD_SIZE, r6
bss_check:
        cmp     r7, r6
        bl      bss_loop

        ; Copy data from ROM to RAM
        movhi   #hilo(__sdata_rom, 1), r0, r6
        movea   #hilo(__sdata_rom, 0), r6, r6
        movhi   #hilo(__sdata, 1), r0, r7
        movea   #hilo(__sdata, 0), r7, r7
        movhi   #hilo(__edata, 1), r0, r8
        movea   #hilo(__edata, 0), r8, r8
        br      data_check
data_loop:
        ld.w    0[r6], r9
        st.w    r9, 0[r7]
        add     WORD_SIZE, r6
        add     WORD_SIZE, r7
data_check:
        cmp     r8, r7
        bl      data_loop

        jarl    _SystemInit, lp
        ei
        jr      _main

_exception_handler:
        br      _exception_handler

        .align  4
        .public _tick_handler
_tick_handler:
        jarl    _Scheduler_TickISR, lp
        rte

        .end
