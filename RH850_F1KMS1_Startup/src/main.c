/******************************************************************************
 * File Name    : main.c
 * Device       : RH850/F1KM-S1
 * Description  : Main application for RH850/F1KM-S1
 *                Simple example demonstrating startup code functionality
 *****************************************************************************/

/*----------------------------------------------------------------------------
 * Includes
 *--------------------------------------------------------------------------*/
#include "system_rh850.h"

/*----------------------------------------------------------------------------
 * Local Definitions
 *--------------------------------------------------------------------------*/
#define LED_PORT_BASE           (0xFFC10000UL)  /* Port 0 base address */
#define LED_PIN                 (0)             /* P0_0 for LED */

/*----------------------------------------------------------------------------
 * Port Register Definitions
 *--------------------------------------------------------------------------*/
typedef struct {
  volatile uint16_t P;          /* Port register */
  volatile uint16_t PSR;        /* Port set/reset register */
  volatile uint16_t PNOT;       /* Port NOT register */
  uint16_t reserved0;
  volatile uint16_t PPR;        /* Port pin read register */
  volatile uint16_t PM;         /* Port mode register */
  volatile uint16_t PMC;        /* Port mode control register */
  volatile uint16_t PFC;        /* Port function control register */
  volatile uint16_t PFCE;       /* Port function control expansion register */
  volatile uint16_t PMSR;       /* Port mode set/reset register */
  volatile uint16_t PMCSR;      /* Port mode control set/reset register */
  volatile uint16_t PFCAE;      /* Port function control additional expansion */
  uint16_t reserved1[4];
  volatile uint16_t PINV;       /* Port inversion register */
} PORT_Type;

#define PORT0                   ((PORT_Type *)LED_PORT_BASE)

/*----------------------------------------------------------------------------
 * Global Variables
 *--------------------------------------------------------------------------*/
static uint32_t g_loop_counter = 0;

/*----------------------------------------------------------------------------
 * Local Function Prototypes
 *--------------------------------------------------------------------------*/
static void GPIO_Init ( void );
static void LED_Toggle ( void );

/*----------------------------------------------------------------------------
 * Function Implementations
 *--------------------------------------------------------------------------*/

/**
 * @brief  Main function
 * @note   Entry point after startup code execution
 */
int main ( void )
{
  /* Initialize GPIO for LED control */
  GPIO_Init ();
  
  /* Main loop */
  while ( 1 )
  {
    /* Toggle LED */
    LED_Toggle ();
    
    /* Increment loop counter */
    g_loop_counter++;
    
    /* Delay approximately 500ms */
    SystemDelay ( 500 );
  }
  
  /* Never reached */
  return 0;
}

/**
 * @brief  Initialize GPIO for LED
 */
static void GPIO_Init ( void )
{
  /* Configure P0_0 as output */
  PORT0->PM &= ~(1U << LED_PIN);        /* Set as output mode */
  PORT0->PMC &= ~(1U << LED_PIN);       /* Set as port mode (not alternate function) */
  PORT0->P &= ~(1U << LED_PIN);         /* Initial output: LOW */
}

/**
 * @brief  Toggle LED state
 */
static void LED_Toggle ( void )
{
  PORT0->PNOT = (1U << LED_PIN);        /* Toggle pin using PNOT register */
}

/*** End of file ***/
