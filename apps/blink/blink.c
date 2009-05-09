#include <sched.h>
#include <timer.h>
#include <leds.h>
#include <types.h>
#include <usart.h>
#include <stdio.h>

#define blink_interval1 500 //milliseconds
#define blink_interval2 500 //milliseconds
#define blink_interval3 500 //milliseconds

void blink_task(void)
{
	leds(RED, TOGGLE);
//printf("RT\n");
}

void blink(void)
{
	post_task(blink_task);
}


void blink_yellow(void)
{
	leds(YELLOW, TOGGLE);
}


void blink_green(void)
{
	leds(GREEN, TOGGLE);
}

int main(void)
{
	os_timer_p timer_blink_red;
	os_timer_p timer_blink_yellow;
	os_timer_p timer_blink_green;
	/*on borad leds init*/
	leds_init();	
	/*user timer init*/
	os_init_timer();
	
	/*uart0 init*/
	console_init();
	
	leds(RED, OFF);
	leds(GREEN, OFF);
	leds(YELLOW, OFF);

	timer_blink_red = os_create_timer(blink_interval1,true, blink);
	timer_blink_green = os_create_timer(blink_interval2,true, blink_green);
	timer_blink_yellow = os_create_timer(blink_interval3,true, blink_yellow);

	os_start_timer(timer_blink_red);
	os_start_timer(timer_blink_green);
	os_start_timer(timer_blink_yellow);

	os_run_task();
	return 0;
}

