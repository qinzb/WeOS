
/**
 * @brief    header file for LED
 * @author   Simon Han
 */


#ifndef _LED_H
#define _LED_H

#define LED_RED_ON           1
#define LED_GREEN_ON         2
#define LED_YELLOW_ON        3
#define LED_RED_OFF          4
#define LED_GREEN_OFF        5
#define LED_YELLOW_OFF       6
#define LED_RED_TOGGLE       7
#define LED_GREEN_TOGGLE     8
#define LED_YELLOW_TOGGLE    9


/**
 * @brief led functions
 */
#ifndef DISABLE_LEDS
#define led_red_on()        PORTA &= ~(1<<(2))
#define led_green_on()      PORTA &= ~(1<<(1))
#define led_yellow_on()     PORTA &= ~(1)
#define led_red_off()       PORTA |= (1<<(2))
#define led_green_off()     PORTA |= (1<<(1))
#define led_yellow_off()    PORTA |= 1
#define led_red_toggle()    PORTA ^= (1 << 2)
#define led_green_toggle()  PORTA ^= (1 << 1)
#define led_yellow_toggle() PORTA ^= 1
#define led_init()     {  DDRA |= 0x07; led_green_off(); led_red_off(); led_yellow_off(); }
#else 
#define led_red_on()        
#define led_green_on()     
#define led_yellow_on()   
#define led_red_off()       
#define led_green_off()     
#define led_yellow_off()    
#define led_red_toggle()    
#define led_green_toggle()  
#define led_yellow_toggle() 
#define led_init()     {  DDRA &= ~(0x07); }
#endif

/**
 * kernel writer can just use macros provided by SOS
 *
 * led_red_on()        
 * led_green_on()    
 * led_yellow_on()   
 * led_red_off()      
 * led_green_off()     
 * led_yellow_off()    
 * led_red_toggle() 
 * led_green_toggle() 
 * led_yellow_toggle()
 */
#ifndef _MODULE_
#include <sos.h>
extern int8_t ker_led(uint8_t action);

#endif /* _MODULE_ */
#endif

