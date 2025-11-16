/******************************************************************************
 * File Name    : system_rh850.c
 * Device       : RH850/F1KM-S1
 * Description  : System initialization implementation for RH850/F1KM-S1
 *                - Clock configuration and PLL setup
 *                - Interrupt controller initialization
 *                - System utilities
 *****************************************************************************/

/*----------------------------------------------------------------------------
 * Includes
 *--------------------------------------------------------------------------*/
#include "system_rh850.h"

/*----------------------------------------------------------------------------
 * Local Definitions
 *--------------------------------------------------------------------------*/
#define PLL_MUL                 15      /* PLL multiplier: 16MHz * 15 = 240MHz */
#define CPU_DIV                 2       /* CPU divider: 240MHz / 2 = 120MHz */
#define PERIPHERAL_DIV          4       /* Peripheral divider: 240MHz / 4 = 60MHz */

/* MainOSC stabilization time (approx 10ms) */
#define MOSC_STAB_COUNT         (10000UL)

/* PLL stabilization time */
#define PLL_STAB_COUNT          (1000UL)

/*----------------------------------------------------------------------------
 * Local Variables
 *--------------------------------------------------------------------------*/
static uint32_t g_system_clock = SYSTEM_CLOCK_HZ;
static uint32_t g_peripheral_clock = PERIPHERAL_CLOCK_HZ;

/*----------------------------------------------------------------------------
 * Function Implementations
 *--------------------------------------------------------------------------*/

/**
 * @brief  System initialization function
 */
void SystemInit ( void )
{
  /* Initialize clock system */
  SystemClockInit ();
  
  /* Initialize interrupt controller */
  InterruptInit ();
  
  /* Additional hardware initialization can be added here */
}

/**
 * @brief  Initialize system clocks
 */
void SystemClockInit ( void )
{
  volatile uint32_t timeout;
  
  /*--------------------------------------------------------------------------
   * Step 1: Enable Main Oscillator (MainOSC)
   *------------------------------------------------------------------------*/
  MOSCE = 0x01;                         /* Enable MainOSC */
  
  /* Wait for MainOSC stabilization */
  for ( timeout = 0; timeout < MOSC_STAB_COUNT; timeout++ )
  {
    if ( ( MOSCS & 0x04 ) != 0 )        /* Check MOSCS.ACT bit */
    {
      break;
    }
  }
  
  /*--------------------------------------------------------------------------
   * Step 2: Configure PLL
   *------------------------------------------------------------------------*/
  /* PLL0 configuration:
   * Input: 16MHz MainOSC
   * Multiplier: x15
   * Output: 240MHz
   */
  PLLC0 = 0x00000001;                   /* Enable PLL with default settings */
  
  /* Wait for PLL stabilization */
  for ( timeout = 0; timeout < PLL_STAB_COUNT; timeout++ )
  {
    if ( ( PLLS0 & 0x04 ) != 0 )        /* Check PLLS.ACT bit */
    {
      break;
    }
  }
  
  /*--------------------------------------------------------------------------
   * Step 3: Configure Clock Dividers
   *------------------------------------------------------------------------*/
  /* CPU Clock: 240MHz / 2 = 120MHz */
  CKSC_Type *cpuclk = (CKSC_Type *)(CKSC_BASE + 0x000);
  cpuclk->CTL = 0x01;                   /* Select PLL / 2 */
  while ( ( cpuclk->ACT & 0x03 ) != 0x01 );
  
  /* Peripheral Clock: 240MHz / 4 = 60MHz */
  CKSC_Type *periclk = (CKSC_Type *)(CKSC_BASE + 0x100);
  periclk->CTL = 0x02;                  /* Select PLL / 4 */
  while ( ( periclk->ACT & 0x03 ) != 0x02 );
  
  /* Update global clock variables */
  g_system_clock = SYSTEM_CLOCK_HZ;
  g_peripheral_clock = PERIPHERAL_CLOCK_HZ;
}

/**
 * @brief  Initialize interrupt controller
 */
void InterruptInit ( void )
{
  volatile uint16_t *eic;
  uint32_t i;
  
  /*--------------------------------------------------------------------------
   * Configure all external interrupt channels
   * Default: Disabled, Priority = 15 (lowest)
   *------------------------------------------------------------------------*/
  /* パフォーマンス最適化: 2 エントリずつアンローリングしてループ回数を半減 */
  eic = (volatile uint16_t *)(EIC_BASE);
  for ( i = 0; i + 1 < 512; i += 2 )
  {
    eic[i]     = 0x00BF;                /* Mask interrupt, priority 15 */
    eic[i + 1] = 0x00BF;                /* Next entry */
  }
  /* ひとつ余りがあれば処理 */
  if ( i < 512 )
  {
    eic[i] = 0x00BF;
  }
  
  /* Additional INTC configuration can be added here */
}

/**
 * @brief  Get current CPU clock frequency
 */
uint32_t SystemGetCpuClock ( void )
{
  return g_system_clock;
}

/**
 * @brief  Get current peripheral clock frequency
 */
uint32_t SystemGetPeripheralClock ( void )
{
  return g_peripheral_clock;
}

/**
 * @brief  Simple delay function (blocking)
 */
void SystemDelay ( uint32_t ms )
{
  volatile uint32_t count;
  uint32_t cycles_per_ms;
  
  /* Approximate cycles per millisecond */
  cycles_per_ms = g_system_clock / 1000 / 10;  /* Divided by 10 for loop overhead */
  
  while ( ms-- )
  {
    for ( count = 0; count < cycles_per_ms; count++ )
    {
      /* Busy wait */
      __asm volatile ( "nop" );
    }
  }
}

/*** End of file ***/
