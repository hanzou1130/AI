/******************************************************************************
 * File Name    : tauj_timer.h
 * Device       : RH850/F1KM-S1
 * Description  : TAUJ0 Timer driver header for RH850/F1KM-S1
 *                Provides interval timer functionality using TAUJ0 Channel 0
 *****************************************************************************/

#ifndef TAUJ_TIMER_H
#define TAUJ_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------
 * Includes
 *--------------------------------------------------------------------------*/
#include "fixed_ints.h"

/*----------------------------------------------------------------------------
 * Public Function Prototypes
 *--------------------------------------------------------------------------*/

/**
 * @brief  Initialize TAUJ0 timer for interval operation
 * @param  interval_us  Interval period in microseconds
 * @note   Maximum interval depends on peripheral clock frequency
 *         At 60MHz PCLK, max interval is approximately 71 seconds
 */
void TAUJ0_Init ( uint32_t interval_us );

/**
 * @brief  Start TAUJ0 timer
 */
void TAUJ0_Start ( void );

/**
 * @brief  Stop TAUJ0 timer
 */
void TAUJ0_Stop ( void );

/**
 * @brief  Get current TAUJ0 counter value
 * @return Current counter value
 */
uint32_t TAUJ0_GetCount ( void );

/**
 * @brief  TAUJ0 interrupt handler (called from vector table)
 * @note   This function should be referenced in boot.asm interrupt vector
 */
void TAUJ0_InterruptHandler ( void );

#ifdef __cplusplus
}
#endif

#endif /* TAUJ_TIMER_H */

/*** End of file ***/
