#ifndef _TIMER_H
#define _TIMER_H

#include "types.h"


typedef void (*timer_callback_t)(void);

typedef enum {
	timer_stop = 0,
	timer_run
	}timer_status;


typedef struct os_timer {
	struct os_timer *prev;
	struct os_timer *next;
	uint32_t ticks;
	uint32_t ticks_left;
	bool type; //false: no repeat, one shot timer. true: repeat timer
	timer_callback_t func;
	uint8_t status;
}os_timer_t, *os_timer_p;
	

extern void os_init_timer(void);
extern bool os_delete_timer(os_timer_p timer);
extern os_timer_p os_create_timer(uint32_t interval, bool repeat, timer_callback_t callback);
extern void os_set_timer_interval(os_timer_p timer, uint32_t interval);
extern void os_set_timer_type(os_timer_p timer, bool repeat);
extern void os_set_timer_callback(os_timer_p timer, timer_callback_t callback);
extern bool os_start_timer(os_timer_p timer);
extern bool os_stop_timer(os_timer_p timer);
extern void os_adjust_timer();
extern void os_timer_comp_isr();

#endif
