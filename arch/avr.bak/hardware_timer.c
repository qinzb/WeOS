#include <os_cpu.h>
#include <hardware_timer.h>

extern void os_timer_comp_isr(void);

uint8_t read_counter()
{
	return TCNT0;
}

void set_counter(uint8_t count)
{
	TCNT0 = count;
}

uint8_t read_interval()
{
	return OCR0;
}

void set_interval(uint8_t interval)
{
	OCR0 = interval;
}

void comp_interrupt_enable()
{
   sbi(ASSR, AS0);        //set Timer/Counter0 to be asynchronous
   sbi(TIMSK, OCIE0);     //Enable TC0 interrupt
}

void comp_interrupt_disable()
{
	cbi(TIMSK, OCIE0);
}

ISR(TIMER0_COMP_vect)
{
 os_timer_comp_isr();
}

