/*
 * Simple cooperative scheduler implementation
 */
#include "scheduler.h"
#include "system_rh850.h"
#include "../inc/fixed_ints.h"

#define MAX_TASKS   8

typedef struct {
  void (*func)(void);
  uint32_t period_ms;
  uint32_t last_run_ms;
  uint8_t used;
} task_t;

static task_t s_tasks[MAX_TASKS];
static volatile uint32_t s_ticks_ms = 0;

void Scheduler_Init ( void )
{
  int i;
  for ( i = 0; i < MAX_TASKS; ++i )
  {
    s_tasks[i].func = 0;
    s_tasks[i].period_ms = 0;
    s_tasks[i].last_run_ms = 0;
    s_tasks[i].used = 0;
  }
  s_ticks_ms = 0;
}

int Scheduler_AddTask ( void (*task)(void), uint32_t period_ms )
{
  int i;
  for ( i = 0; i < MAX_TASKS; ++i )
  {
    if ( s_tasks[i].used == 0 )
    {
      s_tasks[i].func = task;
      s_tasks[i].period_ms = period_ms;
      s_tasks[i].last_run_ms = 0;
      s_tasks[i].used = 1;
      return 0;
    }
  }
  return -1; /* no slot */
}

void Scheduler_Run ( void )
{
  /* Cooperative scheduling loop. Timer ISR advances s_ticks_ms (1ms). */
  for ( ;; )
  {
    uint32_t now = s_ticks_ms;
    int i;
    for ( i = 0; i < MAX_TASKS; ++i )
    {
      if ( s_tasks[i].used && s_tasks[i].func )
      {
        uint32_t last = s_tasks[i].last_run_ms;
        uint32_t period = s_tasks[i].period_ms;
        if ( (now - last) >= period )
        {
          s_tasks[i].last_run_ms = now;
          s_tasks[i].func();
        }
      }
    }

    /* Small yield to avoid busy spin; tick is advanced in ISR */
    SystemDelay ( 1 );
  }
}

/* Called from assembly timer ISR (1ms tick) */
void Scheduler_TickISR ( void )
{
  s_ticks_ms++;
}
