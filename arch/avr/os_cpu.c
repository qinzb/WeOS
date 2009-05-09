/*
 *
 * Authors:             Jason Hill, Philip Levis, Nelson Lee
 *
 *
 */

/**
 * @author Jason Hill
 * @author Philip Levis
 * @author Nelson Lee
 */


#include <os_cpu.h>

void MCU_wait()
{
  asm volatile("nop");
  asm volatile("nop");
}

// atomic statement runtime support
inline os_atomic_t os_atomic_begin(void) 
{
  os_atomic_t result = inp(SREG);
  cli();
  return result;
}

inline void os_atomic_end(os_atomic_t oldSreg)
{
  outp(oldSreg, SREG);
}


inline void os_sleep()
{
  /* Atomically enable interrupts and sleep */
  sei();  // Make sure interrupts are on, so we can wake up!
  asm volatile ("sleep");
  MCU_wait();
}

inline void os_enable_interrupt() {
  sei();
}

inline void os_disable_interrupt() {
  cli();
}

