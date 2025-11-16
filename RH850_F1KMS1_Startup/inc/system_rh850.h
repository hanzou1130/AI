/******************************************************************************
 * File Name    : system_rh850.h
 * Device       : RH850/F1KM-S1
 * Description  : System initialization header for RH850/F1KM-S1
 *                - Clock configuration
 *                - Peripheral initialization
 *                - Interrupt controller setup
 *****************************************************************************/

#ifndef SYSTEM_RH850_H
#define SYSTEM_RH850_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 * Includes
 *--------------------------------------------------------------------------*/
#include <stdint.h>

/*----------------------------------------------------------------------------
 * System Clock Configuration
 *--------------------------------------------------------------------------*/
#define SYSTEM_CLOCK_HZ         (120000000UL)   /* 120MHz CPU clock */
#define PERIPHERAL_CLOCK_HZ     (60000000UL)    /* 60MHz peripheral clock */

/*----------------------------------------------------------------------------
 * External Crystal Frequency
 *--------------------------------------------------------------------------*/
#define EXTERNAL_XTAL_HZ        (16000000UL)    /* 16MHz external crystal */

/*----------------------------------------------------------------------------
 * Register Base Addresses
 *--------------------------------------------------------------------------*/
/* Clock Generator */
#define CKSC_BASE               (0xFFF89000UL)
#define MOSCE                   (*(volatile uint32_t *)(0xFFF81100UL))
#define MOSCS                   (*(volatile uint32_t *)(0xFFF81104UL))
#define MOSCST                  (*(volatile uint32_t *)(0xFFF8110CUL))

/* PLL Control */
#define PLLC0                   (*(volatile uint32_t *)(0xFFF89000UL))
#define PLLS0                   (*(volatile uint32_t *)(0xFFF89004UL))

/* Port Control */
#define PORT_BASE               (0xFFC10000UL)

/* Interrupt Controller (INTC1) */
#define INTC1_BASE              (0xFFFC0000UL)
#define EIC_BASE                (INTC1_BASE)

/*----------------------------------------------------------------------------
 * Clock Register Definitions
 *--------------------------------------------------------------------------*/
typedef struct {
  volatile uint32_t CTL;        /* Clock control register */
  volatile uint32_t ACT;        /* Clock active register */
  volatile uint32_t STAT;       /* Clock status register */
} CKSC_Type;

/*----------------------------------------------------------------------------
 * Function Prototypes
 *--------------------------------------------------------------------------*/

/**
 * @brief  System initialization function
 * @note   Called from startup code before main()
 *         - Initializes clock system
 *         - Configures PLL
 *         - Sets up interrupt controller
 * @param  None
 * @retval None
 */
void SystemInit ( void );

/**
 * @brief  Initialize system clocks
 * @note   Configures MainOSC and PLL to achieve target frequencies
 * @param  None
 * @retval None
 */
void SystemClockInit ( void );

/**
 * @brief  Initialize interrupt controller
 * @note   Sets default priority levels and enables interrupt processing
 * @param  None
 * @retval None
 */
void InterruptInit ( void );

/**
 * @brief  Get current CPU clock frequency
 * @param  None
 * @retval CPU clock frequency in Hz
 */
uint32_t SystemGetCpuClock ( void );

/**
 * @brief  Get current peripheral clock frequency
 * @param  None
 * @retval Peripheral clock frequency in Hz
 */
uint32_t SystemGetPeripheralClock ( void );

/**
 * @brief  Simple delay function (blocking)
 * @note   Approximate delay, not cycle-accurate
 * @param  ms: Delay time in milliseconds
 * @retval None
 */
void SystemDelay ( uint32_t ms );

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_RH850_H */

/*** End of file ***/
