/* -*- Mode: C; tab-width:2 -*- */
/* ex: set ts=2 shiftwidth=2 softtabstop=2 cindent: */
/** 
 * @brief Header file for mica2 specific modules
 */
#ifndef _MODULE_PLAT_H
#define _MODULE_PLAT_H

#include <kertable_plat.h>

#ifdef _MODULE_

/**
 * Radio Link Layer Ack
 */
typedef void (*ker_radio_ack_func_t)(void);
/** Enable Ack */
static inline void ker_radio_ack_enable(void)
{   
	ker_radio_ack_func_t func = (ker_radio_ack_func_t)get_kertable_entry(PROC_KERTABLE_END+1);
	func();
	return;
}
/** Disable Ack */
static inline void ker_radio_ack_disable(void)
{
	ker_radio_ack_func_t func = (ker_radio_ack_func_t)get_kertable_entry(PROC_KERTABLE_END+2);
	func();
	return;
}  


/**
 * @brief led functions
 * @param action can be following
 *    LED_RED_ON
 *    LED_GREEN_ON
 *    LED_YELLOW_ON
 *    LED_RED_OFF
 *    LED_GREEN_OFF
 *    LED_YELLOW_OFF
 *    LED_RED_TOGGLE
 *    LED_GREEN_TOGGLE
 *    LED_YELLOW_TOGGLE
 */
typedef int8_t (*ledfunc_t)(uint8_t action);
static inline int8_t ker_led(uint8_t action){
	ledfunc_t func = (ledfunc_t)get_kertable_entry(PROC_KERTABLE_END+3);
	return func(action);
}

#endif /* _MODULE_ */
#endif /* _MODULE_PLAT_H */

