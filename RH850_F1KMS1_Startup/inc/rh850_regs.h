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
#include <stdint.h>

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

/* OSTM (OS Timer) */
#define OSTM0_BASE              (0xFFEC0000UL)

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
