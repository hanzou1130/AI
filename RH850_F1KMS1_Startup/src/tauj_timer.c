/******************************************************************************
 * File Name    : tauj_timer.c
 * Device       : RH850/F1KM-S1
 * Description  : TAUJ0 Timer driver implementation for RH850/F1KM-S1
 *                - Interval timer mode using channel 0
 *                - Generates periodic interrupts for system tick
 *****************************************************************************/

/*----------------------------------------------------------------------------
 * Includes
 *--------------------------------------------------------------------------*/
#include "tauj_timer.h"
#include "rh850_regs.h"
#include "system_rh850.h"
#include "scheduler.h"

/*----------------------------------------------------------------------------
 * Local Definitions
 *--------------------------------------------------------------------------*/
/* TAUJ0 interrupt channel number (EIC)
 * NOTE: This is device-specific. Verify with hardware manual.
 * Assuming TAUJ0I0 interrupt is mapped to EIC channel 80.
 */
#define TAUJ0_EIC_CHANNEL       (80)

/*----------------------------------------------------------------------------
 * Function Implementations
 *--------------------------------------------------------------------------*/

/**
 * @brief  Initialize TAUJ0 timer for interval operation
 */
void TAUJ0_Init ( uint32_t interval_us )
{
  uint32_t pclk_hz;
  uint32_t compare_value;
  
  /* Get peripheral clock frequency */
  pclk_hz = SystemGetPeripheralClock ();
  
  /* Calculate compare value for desired interval
   * compare_value = (PCLK_Hz / 1,000,000) * interval_us
   * For 1ms interval at 60MHz: (60,000,000 / 1,000,000) * 1000 = 60,000
   */
  compare_value = (pclk_hz / 1000000UL) * interval_us;
  
  /* Ensure timer is stopped before configuration */
  TAUJ0TT = TAUJ0_CH0;
  
  /* Configure Channel 0 operating mode
   * - MD[1:0] = 00: Interval timer mode
   * - COS = 0: Counter clear source (auto-clear on match)
   */
  TAUJ0CMOR0 = TAUJ0_CMOR_MD_INTERVAL;
  
  /* Configure clock source
   * - CKS[1:0] = 00: Peripheral clock (no division)
   */
  TAUJ0CMUR0 = TAUJ0_CMUR_CKS_PCLK;
  
  /* Set compare/reload value */
  TAUJ0CDR0 = compare_value;
  
  /* Clear any pending interrupt flags */
  TAUJ0CSC0 = 0x01;
  
  /* Enable TAUJ0 interrupt in INTC
   * Clear EIC_EIMK bit to unmask interrupt
   */
  EIC(TAUJ0_EIC_CHANNEL) &= (uint16_t)~EIC_EIMK;
}

/**
 * @brief  Start TAUJ0 timer
 */
void TAUJ0_Start ( void )
{
  /* Start channel 0 by setting bit 0 in TS register */
  TAUJ0TS = TAUJ0_CH0;
}

/**
 * @brief  Stop TAUJ0 timer
 */
void TAUJ0_Stop ( void )
{
  /* Stop channel 0 by setting bit 0 in TT register */
  TAUJ0TT = TAUJ0_CH0;
}

/**
 * @brief  Get current TAUJ0 counter value
 */
uint32_t TAUJ0_GetCount ( void )
{
  return TAUJ0CNT0;
}

/**
 * @brief  TAUJ0 interrupt handler
 * @note   Called when TAUJ0 channel 0 compare match occurs
 */
void TAUJ0_InterruptHandler ( void )
{
  /* Clear interrupt flag */
  TAUJ0CSC0 = 0x01;
  
  /* Update scheduler tick counter */
  Scheduler_TickISR ();
}

/*** End of file ***/
