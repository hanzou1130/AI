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
#include "rh850_regs.h"
#include "tauj_timer.h"

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
/* Hardware register constants */
#define MAINOSC_ENABLE              (0x01U)
#define MOSCS_ACT_BIT               (0x04U)
#define PLLC0_ENABLE                (0x00000001U)
#define PLLS_ACT_BIT                (0x04U)

/* CKSC CTL values and offsets */
#define CKSC_CPU_SEL_PLL_DIV2       (0x01U)
#define CKSC_PERI_SEL_PLL_DIV4      (0x02U)
#define CKSC_ACT_MASK               (0x03U)
#define CKSC_PERI_OFFSET            (0x100U)

/* EIC defaults */
#define EIC_COUNT                   (512U)
#define EIC_DEFAULT_MASK            (0x00BFU)

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
  
  /* Initialize TAUJ0 for 1ms interval timer */
  TAUJ0_Init ( 1000 );      /* 1000us = 1ms */
  TAUJ0_Start ();
  
  /* Additional hardware initialization can be added here */
}

/**
 * @brief  Initialize system clocks
 */
void SystemClockInit ( void )
{
  volatile uint32_t timeout;
  CKSC_Type *cpuclk;
  CKSC_Type *periclk;
  
  /*--------------------------------------------------------------------------
   * Step 1: Enable Main Oscillator (MainOSC)
   *------------------------------------------------------------------------*/
  MOSCE = MAINOSC_ENABLE;              /* Enable MainOSC */
  
  /* Wait for MainOSC stabilization */
  for ( timeout = 0; timeout < MOSC_STAB_COUNT; timeout++ )
  {
    if ( ( MOSCS & MOSCS_ACT_BIT ) != 0 )        /* Check MOSCS.ACT bit */
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
  PLLC0 = PLLC0_ENABLE;                 /* Enable PLL with default settings */
  
  /* Wait for PLL stabilization */
  for ( timeout = 0; timeout < PLL_STAB_COUNT; timeout++ )
  {
    if ( ( PLLS0 & PLLS_ACT_BIT ) != 0 )        /* Check PLLS.ACT bit */
    {
      break;
    }
  }
  
  /*--------------------------------------------------------------------------
   * Step 3: Configure Clock Dividers
   *------------------------------------------------------------------------*/
  /* CPU Clock: 240MHz / 2 = 120MHz */
  cpuclk = (CKSC_Type *)(CKSC_BASE + 0x000);
  cpuclk->CTL = CKSC_CPU_SEL_PLL_DIV2;  /* Select PLL / 2 */
  while ( ( cpuclk->ACT & CKSC_ACT_MASK ) != CKSC_CPU_SEL_PLL_DIV2 );
  
  /* Peripheral Clock: 240MHz / 4 = 60MHz */
  periclk = (CKSC_Type *)(CKSC_BASE + CKSC_PERI_OFFSET);
  periclk->CTL = CKSC_PERI_SEL_PLL_DIV4; /* Select PLL / 4 */
  while ( ( periclk->ACT & CKSC_ACT_MASK ) != CKSC_PERI_SEL_PLL_DIV4 );
  
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
  for ( i = 0; i < EIC_COUNT; i++ )
  {
    eic = (volatile uint16_t *)(EIC_BASE + (i * 2));
    *eic = EIC_DEFAULT_MASK;            /* Mask interrupt, priority 15, table reference */
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
      /* Busy wait - volatile access to prevent optimization */
      (void)count;
    }
  }
}

/*** End of file ***/
