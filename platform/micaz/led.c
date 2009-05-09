/**
 * @brief    led module for mica2
 * @author   Simon Han (simonhan@ee.ucla.edu)
 * @version  0.1
 *
 */

#include <sos.h>
#include "led.h"

int8_t ker_led(uint8_t action){
	switch (action){
		case LED_RED_ON:        led_red_on();        break;
		case LED_GREEN_ON:      led_green_on();      break;
		case LED_YELLOW_ON:     led_yellow_on();     break;
		case LED_RED_OFF:       led_red_off();       break;
		case LED_GREEN_OFF:     led_green_off();     break;
		case LED_YELLOW_OFF:    led_yellow_off();    break;
		case LED_RED_TOGGLE:    led_red_toggle();    break;
		case LED_GREEN_TOGGLE:  led_green_toggle();  break;
		case LED_YELLOW_TOGGLE: led_yellow_toggle(); break;
	}
	return 0;
}
