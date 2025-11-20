/******************************************************************************
 * File Name    : rh850_regs.h
 * Device       : RH850/F1KM-S1
 * Description  : Register definitions for RH850/F1KM-S1
 *                Common peripheral register addresses and bit definitions
 *****************************************************************************/

#ifndef RH850_REGS_H
#define RH850_REGS_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 * Includes
 *--------------------------------------------------------------------------*/
#include "fixed_ints.h"

/*----------------------------------------------------------------------------
 * Clock Generator Registers
 *--------------------------------------------------------------------------*/
/* MainOSC Control */
#define MOSCE                   (*(volatile uint32_t *)0xFFF81100)
#define MOSCS                   (*(volatile uint32_t *)0xFFF81104)
#define MOSCST                  (*(volatile uint32_t *)0xFFF8110C)

/* PLL Control */
#define PLLC0                   (*(volatile uint32_t *)0xFFF89000)
#define PLLS0                   (*(volatile uint32_t *)0xFFF89004)
#define PLLC1                   (*(volatile uint32_t *)0xFFF89008)
#define PLLS1                   (*(volatile uint32_t *)0xFFF8900C)

/* Clock Source Selectors */
#define CKSC_CPUCLKS_CTL        (*(volatile uint32_t *)0xFFF8A000)
#define CKSC_CPUCLKS_ACT        (*(volatile uint32_t *)0xFFF8A008)
#define CKSC_CPUCLKD_CTL        (*(volatile uint32_t *)0xFFF8A100)
#define CKSC_CPUCLKD_ACT        (*(volatile uint32_t *)0xFFF8A108)

/*----------------------------------------------------------------------------
 * Port Registers (Port 0-22)
 *--------------------------------------------------------------------------*/
#define PORT_BASE               (0xFFC10000UL)
#define PORT_SIZE               (0x40)

/* Port 0 */
#define P0                      (*(volatile uint16_t *)(PORT_BASE + 0x0000))
#define PSR0                    (*(volatile uint16_t *)(PORT_BASE + 0x0002))
#define PNOT0                   (*(volatile uint16_t *)(PORT_BASE + 0x0004))
#define PPR0                    (*(volatile uint16_t *)(PORT_BASE + 0x0008))
#define PM0                     (*(volatile uint16_t *)(PORT_BASE + 0x000A))
#define PMC0                    (*(volatile uint16_t *)(PORT_BASE + 0x000C))
#define PFC0                    (*(volatile uint16_t *)(PORT_BASE + 0x000E))

/*----------------------------------------------------------------------------
 * Interrupt Controller (INTC1)
 *--------------------------------------------------------------------------*/
#define INTC1_BASE              (0xFFFC0000UL)

/* External Interrupt Control Registers (EIC0-511) */
#define EIC(n)                  (*(volatile uint16_t *)(INTC1_BASE + (n) * 2))

/* Interrupt Bit Definitions */
#define EIC_EIMK                (0x0080)    /* Interrupt mask */
#define EIC_EITB                (0x0040)    /* Table reference */

/*----------------------------------------------------------------------------
 * System Registers (Protected)
 *--------------------------------------------------------------------------*/
/* Protection Command Register */
#define PROTCMD0                (*(volatile uint32_t *)0xFFF80000)
#define PROTCMD1                (*(volatile uint32_t *)0xFFF88000)

/* Protection Status Register */
#define PROTS0                  (*(volatile uint32_t *)0xFFF80004)
#define PROTS1                  (*(volatile uint32_t *)0xFFF88004)

/*----------------------------------------------------------------------------
 * Reset Control
 *--------------------------------------------------------------------------*/
#define RESF                    (*(volatile uint32_t *)0xFFF80760)
#define RESFC                   (*(volatile uint32_t *)0xFFF80768)

/*----------------------------------------------------------------------------
 * Watchdog Timer
 *--------------------------------------------------------------------------*/
#define WDTA0_BASE              (0xFFED0000UL)
#define WDTA0WDTE               (*(volatile uint8_t *)(WDTA0_BASE + 0x00))
#define WDTA0EVAC               (*(volatile uint8_t *)(WDTA0_BASE + 0x04))
#define WDTA0REF                (*(volatile uint8_t *)(WDTA0_BASE + 0x08))
#define WDTA0MD                 (*(volatile uint8_t *)(WDTA0_BASE + 0x0C))

/*----------------------------------------------------------------------------
 * DMA Controller
 *--------------------------------------------------------------------------*/
#define DMA_BASE                (0xFFFEE000UL)

/*----------------------------------------------------------------------------
 * CAN Controller (RS-CAN)
 *--------------------------------------------------------------------------*/
#define CAN0_BASE               (0xFFD00000UL)
#define CAN1_BASE               (0xFFD10000UL)

/*----------------------------------------------------------------------------
 * Serial Communication (UART/LIN)
 *--------------------------------------------------------------------------*/
#define UART0_BASE              (0xFFCE0000UL)
#define UART1_BASE              (0xFFCE0040UL)
#define UART2_BASE              (0xFFCE0080UL)

/*----------------------------------------------------------------------------
 * CSI (Clock Synchronous Serial Interface)
 *--------------------------------------------------------------------------*/
#define CSI0_BASE               (0xFFCF0000UL)

/*----------------------------------------------------------------------------
 * Timer Units
 *--------------------------------------------------------------------------*/
/* TAUD (Timer Array Unit D) */
#define TAUD0_BASE              (0xFFE20000UL)

/* TAUJ (Timer Array Unit J) */
#define TAUJ0_BASE              (0xFFE50000UL)

/*
 * TAUJ0 register definitions for RH850/F1KM-S1
 * TAUJ is a 32-bit timer array unit with 4 channels
 */
#define TAUJ0CDR0               (*(volatile uint32_t *)(TAUJ0_BASE + 0x0000)) /* Channel 0 data register */
#define TAUJ0CDR1               (*(volatile uint32_t *)(TAUJ0_BASE + 0x0004)) /* Channel 1 data register */
#define TAUJ0CDR2               (*(volatile uint32_t *)(TAUJ0_BASE + 0x0008)) /* Channel 2 data register */
#define TAUJ0CDR3               (*(volatile uint32_t *)(TAUJ0_BASE + 0x000C)) /* Channel 3 data register */
#define TAUJ0CNT0               (*(volatile uint32_t *)(TAUJ0_BASE + 0x0010)) /* Channel 0 counter */
#define TAUJ0CNT1               (*(volatile uint32_t *)(TAUJ0_BASE + 0x0014)) /* Channel 1 counter */
#define TAUJ0CNT2               (*(volatile uint32_t *)(TAUJ0_BASE + 0x0018)) /* Channel 2 counter */
#define TAUJ0CNT3               (*(volatile uint32_t *)(TAUJ0_BASE + 0x001C)) /* Channel 3 counter */
#define TAUJ0CMOR0              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0020)) /* Channel 0 mode register */
#define TAUJ0CMOR1              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0021)) /* Channel 1 mode register */
#define TAUJ0CMOR2              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0022)) /* Channel 2 mode register */
#define TAUJ0CMOR3              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0023)) /* Channel 3 mode register */
#define TAUJ0CMUR0              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0024)) /* Channel 0 clock select */
#define TAUJ0CMUR1              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0025)) /* Channel 1 clock select */
#define TAUJ0CMUR2              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0026)) /* Channel 2 clock select */
#define TAUJ0CMUR3              (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0027)) /* Channel 3 clock select */
#define TAUJ0CSR0               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0028)) /* Channel 0 status register */
#define TAUJ0CSR1               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0029)) /* Channel 1 status register */
#define TAUJ0CSR2               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x002A)) /* Channel 2 status register */
#define TAUJ0CSR3               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x002B)) /* Channel 3 status register */
#define TAUJ0CSC0               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x002C)) /* Channel 0 status clear */
#define TAUJ0CSC1               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x002D)) /* Channel 1 status clear */
#define TAUJ0CSC2               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x002E)) /* Channel 2 status clear */
#define TAUJ0CSC3               (*(volatile uint8_t  *)(TAUJ0_BASE + 0x002F)) /* Channel 3 status clear */
#define TAUJ0TE                 (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0030)) /* Timer enable status */
#define TAUJ0TS                 (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0034)) /* Timer start trigger */
#define TAUJ0TT                 (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0038)) /* Timer stop trigger */
#define TAUJ0TO                 (*(volatile uint8_t  *)(TAUJ0_BASE + 0x003C)) /* Timer output register */
#define TAUJ0TOE                (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0040)) /* Timer output enable */
#define TAUJ0TOL                (*(volatile uint8_t  *)(TAUJ0_BASE + 0x0044)) /* Timer output level */

/* TAUJ0 CMOR bit definitions - Channel Mode Register */
#define TAUJ0_CMOR_MD_INTERVAL  (0x00)  /* Interval timer mode */
#define TAUJ0_CMOR_MD_CAPTURE   (0x01)  /* Capture mode */
#define TAUJ0_CMOR_COS          (0x10)  /* Counter clear source (bit 4) */

/* TAUJ0 CMUR bit definitions - Clock Select Register */
#define TAUJ0_CMUR_CKS_PCLK     (0x00)  /* Peripheral clock */
#define TAUJ0_CMUR_CKS_PCLK_2   (0x01)  /* Peripheral clock / 2 */
#define TAUJ0_CMUR_CKS_PCLK_4   (0x02)  /* Peripheral clock / 4 */

/* TAUJ0 channel bit masks */
#define TAUJ0_CH0               (0x01)  /* Channel 0 */
#define TAUJ0_CH1               (0x02)  /* Channel 1 */
#define TAUJ0_CH2               (0x04)  /* Channel 2 */
#define TAUJ0_CH3               (0x08)  /* Channel 3 */

/* OSTM (OS Timer) */
#define OSTM0_BASE              (0xFFEC0000UL)

/*
 * OSTM0 register definitions (offsets are illustrative and should be
 * verified against the specific RH850/F1KM-S1 hardware manual before
 * using on real hardware). The implementation below assumes common
 * control/counter/compare registers at the offsets shown.
 */
#define OSTM0_TCR               (*(volatile uint32_t *)(OSTM0_BASE + 0x0000)) /* Timer control */
#define OSTM0_CNT               (*(volatile uint32_t *)(OSTM0_BASE + 0x0004)) /* Timer counter */
#define OSTM0_COMP              (*(volatile uint32_t *)(OSTM0_BASE + 0x0008)) /* Compare value */
#define OSTM0_ICR               (*(volatile uint32_t *)(OSTM0_BASE + 0x000C)) /* Interrupt control/status */

/*----------------------------------------------------------------------------
 * A/D Converter
 *--------------------------------------------------------------------------*/
#define ADC0_BASE               (0xFFF90000UL)

/*----------------------------------------------------------------------------
 * Utility Macros
 *--------------------------------------------------------------------------*/
#define REG_WRITE_PROTECTED(reg, val) \
    do { \
        PROTCMD0 = 0xA5; \
        (reg) = (val); \
        (reg) = ~(val); \
        (reg) = (val); \
    } while (PROTS0 != 0)

#ifdef __cplusplus
}
#endif

#endif /* RH850_REGS_H */

/*** End of file ***/
