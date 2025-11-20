/*
 * Simple scheduler for RH850 demo
 * - Cooperative, time-based scheduler
 * - Uses a software 1ms tick driven by `SystemDelay(1)` inside scheduler loop
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "../inc/fixed_ints.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize scheduler internals (must be called before adding tasks) */
void Scheduler_Init ( void );

/* Add a periodic task: function pointer and period in ms
 * Returns 0 on success, -1 if no slot available
 */
int Scheduler_AddTask ( void (*task)(void), uint32_t period_ms );

/* Start the scheduler - this function does not return (runs the scheduling loop) */
void Scheduler_Run ( void );

/* Called from timer ISR to advance the scheduler tick (1ms) */
void Scheduler_TickISR ( void );

#ifdef __cplusplus
}
#endif

#endif /* SCHEDULER_H */
