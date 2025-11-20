;/**********************************************************************************************************************
; * DISCLAIMER
; * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
; * other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
; * applicable laws, including copyright laws.
; * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
; * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
; * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
; * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
; * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO
; * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
; * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
; * this software. By using this software, you agree to the additional terms and conditions found by accessing the
; * following link:
; * http://www.renesas.com/disclaimer
; *
; * Copyright (C) 2020, 2021 Renesas Electronics Corporation. All rights reserved.
; *********************************************************************************************************************/
;   NOTE       : THIS IS A TYPICAL EXAMPLE.
;   DATE       : Wed, Sep 08, 2021

	; External symbols
	.extern	_Scheduler_TickISR
	.extern	_SystemInit
	.extern	_main
	.extern	_TAUJ0_InterruptHandler

	; if using eiint as table reference method,
	; enable next line's macro.

	;USE_TABLE_REFERENCE_METHOD .set 1

;-----------------------------------------------------------------------------
;	exception vector table
;-----------------------------------------------------------------------------
	.section "RESET", text
	.align	512
	jr32	__start ; RESET

	.align	16
	syncp
	jr32	_Dummy ; SYSERR

	.align	16
	jr32	_Dummy

	.align	16
	jr32	_Dummy ; FETRAP

	.align	16
	jr32	_Dummy_EI ; TRAP0

	.align	16
	jr32	_Dummy_EI ; TRAP1

	.align	16
	jr32	_Dummy ; RIE

	.align	16
	syncp
	jr32	_Dummy_EI ; FPP/FPI

	.align	16
	jr32	_Dummy ; UCPOP

	.align	16
	jr32	_Dummy ; MIP/MDP

	.align	16
	jr32	_Dummy ; PIE

	.align	16
	jr32	_Dummy

	.align	16
	jr32	_Dummy ; MAE

	.align	16
	jr32	_Dummy

	.align	16
	syncp
	jr32	_Dummy ; FENMI

	.align	16
	syncp
	jr32	_Dummy ; FEINT

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority0)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority1)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority2)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority3)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority4)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority5)

	.align	16
	syncp
	jr32	_tick_handler ; INTn(priority6)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority7)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority8)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority9)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority10)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority11)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority12)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority13)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority14)

	.align	16
	syncp
	jr32	_Dummy_EI ; INTn(priority15)

	.section "EIINTTBL", const
	.align	512
	.dw	#_Dummy_EI ; INT0
	.dw	#_Dummy_EI ; INT1
	.dw	#_Dummy_EI ; INT2
	.dw	#_Dummy_EI ; INT3
	.dw	#_Dummy_EI ; INT4
	.dw	#_Dummy_EI ; INT5
	.dw	#_Dummy_EI ; INT6
	.rept	73
	.dw	#_Dummy_EI ; INTn
	.endm
	.dw	#_tauj0_tick_handler ; INT80 (TAUJ0I0 interrupt)
	.rept	512 - 81
	.dw	#_Dummy_EI ; INTn
	.endm

	.section ".text", text
	.align	2
_Dummy:
	br	_Dummy

_Dummy_EI:
	br	_Dummy_EI

	.align	2
_tick_handler:
	add	-12, sp
	st.w	lp, 8[sp]
	st.w	r1, 4[sp]
	st.w	r2, 0[sp]
	jarl	_Scheduler_TickISR, lp
	ld.w	0[sp], r2
	ld.w	4[sp], r1
	ld.w	8[sp], lp
	add	12, sp
	eiret

	.align	2
_tauj0_tick_handler:
	add	-12, sp
	st.w	lp, 8[sp]
	st.w	r1, 4[sp]
	st.w	r2, 0[sp]
	jarl	_TAUJ0_InterruptHandler, lp
	ld.w	0[sp], r2
	ld.w	4[sp], r1
	ld.w	8[sp], lp
	add	12, sp
	eiret

;-----------------------------------------------------------------------------
;	startup
;-----------------------------------------------------------------------------
	.align	2
__start:
$if 1	; initialize register
	$nowarning
	mov	r0, r1
	$warning
	mov	r0, r2
	mov	r0, r3
	mov	r0, r4
	mov	r0, r5
	mov	r0, r6
	mov	r0, r7
	mov	r0, r8
	mov	r0, r9
	mov	r0, r10
	mov	r0, r11
	mov	r0, r12
	mov	r0, r13
	mov	r0, r14
	mov	r0, r15
	mov	r0, r16
	mov	r0, r17
	mov	r0, r18
	mov	r0, r19
	mov	r0, r20
	mov	r0, r21
	mov	r0, r22
	mov	r0, r23
	mov	r0, r24
	mov	r0, r25
	mov	r0, r26
	mov	r0, r27
	mov	r0, r28
	mov	r0, r29
	mov	r0, r30
	mov	r0, r31
	ldsr	r0, 0, 0		;  EIPC
	ldsr	r0, 16, 0		;  CTPC
$endif

$if 1
	; jump to entry point of each PE
	stsr	0, r10, 2		; get HTCFG0
	shr	16, r10			; get PEID
	cmp	1, r10
	bz	.L.entry_PE1
	cmp	2, r10
	bz	.L.entry_PE2
	cmp	3, r10
	bz	.L.entry_PE3
	cmp	4, r10
	bz	.L.entry_PE4
	cmp	5, r10
	bz	.L.entry_PE5
	cmp	6, r10
	bz	.L.entry_PE6
	cmp	7, r10
	bz	.L.entry_PE7
__exit:
	br	__exit

.L.entry_PE1:
	jarl	_hdwinit_PE1, lp	; initialize hardware
$ifdef USE_TABLE_REFERENCE_METHOD
	mov	#__sEIINTTBL_PE1, r6
	jarl	_set_table_reference_method, lp ; set table reference method
$endif
	jr32	__cstart_pm1

.L.entry_PE2:
	jarl	_hdwinit_PE2, lp	; initialize hardware
;$ifdef USE_TABLE_REFERENCE_METHOD
;	mov	#__sEIINTTBL_PE2, r6
;	jarl	_set_table_reference_method, lp ; set table reference method
;$endif
;	jr32	__cstart_pm2
	br	__exit

.L.entry_PE3:
	br	__exit
.L.entry_PE4:
	br	__exit
.L.entry_PE5:
	br	__exit
.L.entry_PE6:
	br	__exit
.L.entry_PE7:
	br	__exit
$endif

;-----------------------------------------------------------------------------
;	hdwinit_PE1
; Specify RAM addresses suitable to your system if needed.
;-----------------------------------------------------------------------------
	GLOBAL_RAM_ADDR	.set	0
	GLOBAL_RAM_END	.set	0
	LOCAL_RAM_PE1_ADDR	.set	0
	LOCAL_RAM_PE1_END	.set	0

	.align	2
_hdwinit_PE1:
	mov	lp, r14			; save return address

	; clear Global RAM
	mov	GLOBAL_RAM_ADDR, r6
	mov	GLOBAL_RAM_END, r7
	jarl	_zeroclr4, lp

	; clear Local RAM PE1
	mov	LOCAL_RAM_PE1_ADDR, r6
	mov	LOCAL_RAM_PE1_END, r7
	jarl	_zeroclr4, lp

	; Initialize stack pointer
	mov	#__stack, sp

	; Clear BSS section
	mov	#__sbss, r6
	mov	#__ebss, r7
	jarl	_zeroclr4, lp

	; Copy DATA section from ROM to RAM
	mov	#__sdata_rom, r6
	mov	#__sdata, r7
	mov	#__edata, r8
	jarl	_copy_data, lp

	; Call SystemInit
	jarl	_SystemInit, lp

	; Enable interrupts
	ei

	; Jump to main
	mov	r14, lp
	jr32	_main

;-----------------------------------------------------------------------------
;	hdwinit_PE2
; Specify RAM addresses suitable to your system if needed.
;-----------------------------------------------------------------------------
	LOCAL_RAM_PE2_ADDR	.set	0
	LOCAL_RAM_PE2_END	.set	0

	.align	2
_hdwinit_PE2:
	mov	lp, r14			; save return address

	; clear Local RAM PE2
	mov	LOCAL_RAM_PE2_ADDR, r6
	mov	LOCAL_RAM_PE2_END, r7
	jarl	_zeroclr4, lp

	mov	r14, lp
	jmp	[lp]

;-----------------------------------------------------------------------------
;	zeroclr4
;-----------------------------------------------------------------------------
	.align	2
_zeroclr4:
	br	.L.zeroclr4.2
.L.zeroclr4.1:
	st.w	r0, [r6]
	add	4, r6
.L.zeroclr4.2:
	cmp	r6, r7
	bh	.L.zeroclr4.1
	jmp	[lp]

;-----------------------------------------------------------------------------
;	copy_data (r6=src, r7=dest, r8=dest_end)
;-----------------------------------------------------------------------------
	.align	2
_copy_data:
	br	.L.copy_data.2
.L.copy_data.1:
	ld.w	[r6], r9
	st.w	r9, [r7]
	add	4, r6
	add	4, r7
.L.copy_data.2:
	cmp	r7, r8
	bh	.L.copy_data.1
	jmp	[lp]

$ifdef USE_TABLE_REFERENCE_METHOD
;-----------------------------------------------------------------------------
;	set table reference method
;-----------------------------------------------------------------------------
	; interrupt control register address
	ICBASE	.set	0xfffeea00

	.align	2
_set_table_reference_method:
	ldsr	r6, 4, 1		; set INTBP

	; Some interrupt channels use the table reference method.
	mov	ICBASE, r10		; get interrupt control register address
	set1	6, 0[r10]		; set INT0 as table reference
	set1	6, 2[r10]		; set INT1 as table reference
	set1	6, 4[r10]		; set INT2 as table reference

	jmp	[lp]
$endif
;-------------------- end of start up module -------------------;
