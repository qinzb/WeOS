#ifndef _HW_TIMER_H
#define _HW_TIMER_H

#include <types.h>

uint8_t read_counter(void);
void set_counter(uint8_t count);
uint8_t read_interval(void);
void set_interval(uint8_t interval);
void set_scale(uint8_t scale);

void comp_interrupt_enable(void);
void comp_interrupt_disable(void);

#endif





