;******************************************************************************
; File Name    : cstartup_csplus.asm
; Device       : RH850/F1KM-S1
; Description  : Startup code for RH850/F1KM-S1 (CS+ CC-RH syntax)
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
SP_INIT         .equ    0xFEE28000      ; Stack pointer initial value (top of RAM)
WORD_SIZE       .equ    4               ; Word size in bytes (32-bit)
EXC_TABLE_ALIGN .equ    512             ; Exception table alignment (bytes)
BSS_CLEAR_VALUE .equ    0               ; Value used to clear BSS
        
;------------------------------------------------------------------------------
; External Symbols
;------------------------------------------------------------------------------
        .extern _main
        .extern _SystemInit
        .extern _Scheduler_TickISR
        .extern __sbss
        .extern __ebss
        .extern __sdata
        .extern __edata
        .extern __sdata_rom

;------------------------------------------------------------------------------
; Reset Vector Section
;------------------------------------------------------------------------------
        .section        .reset,text
        .align  4
        .public _RESET
_RESET:
        jr      _start                    ; Jump to startup code

;------------------------------------------------------------------------------
; Exception Vector Table (Basic implementation)
;------------------------------------------------------------------------------
        .align  EXC_TABLE_ALIGN           ; Align to exception table alignment
        .public __exception_table
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
        .section        .text,text
        .align  4
        .public _start
_start:
        ; Disable interrupts
        di

        ; Initialize stack pointer
        movhi   hi(SP_INIT), r0, r3
        movea   lo(SP_INIT), r3, sp

        ; Clear BSS section
        movhi   hi(__sbss), r0, r6        ; r6 = start of BSS
        movea   lo(__sbss), r6, r6
        movhi   hi(__ebss), r0, r7        ; r7 = end of BSS
        movea   lo(__ebss), r7, r7
        br      _bss_check
_bss_loop:
        st.w    r0, 0[r6]                 ; Clear word
        add     WORD_SIZE, r6             ; Next word
_bss_check:
        cmp     r7, r6                    ; Done?
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
        .align  4
_exception_handler:
        br      _exception_handler          ; Infinite loop

;------------------------------------------------------------------------------
; Timer Tick Handler
;------------------------------------------------------------------------------
        .align  4
        .public _tick_handler
_tick_handler:
        jarl    _Scheduler_TickISR, lp
        rte

;------------------------------------------------------------------------------
; End of File
;------------------------------------------------------------------------------
        .end
