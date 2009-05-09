/*****************************************************************************
 *
 * Author: Qinzb
 *
 * Description :
 *    This file contains the source code for the highlevel console functions
 *
 ****************************************************************************/
#include 	<avr/io.h>
#include 	<avr/interrupt.h>
#include    <stdio.h>
#include	<usart.h>
//#include	"leds.h"
//#define ENTER_TASK {unsigned char cSREG=SREG; cli();
//#define LEAVE_TASK  SREG=cSREG; sei();}

int	_getchar(FILE *stream);
int	_putchar(char c, FILE *stream);
static FILE stdinouterr = FDEV_SETUP_STREAM (_putchar, _getchar, _FDEV_SETUP_RW );
unsigned char uart_data;
/**
 * interrupt USART0_REC:
 *
 * The interrupt service routine for receiving characters on the console i/f
 *
 * Returns: none
 */
ISR(USART0_RX_vect) 
{
	uart_data = UDR0;
//	printf("abc\n");
//	leds(RED, TOGGLE);
}
/**
 ** console_init:
 **
 ** Initialize console i/o support
 **
 ** Returns: none.
 **/
void console_init(void)
{
	  /*** enable UART0 **/
    UBRR0L=(Fosc/16/BAUDRATE - 1)%256; /* USART0: 11520000 bps */
    UBRR0H=(Fosc/16/BAUDRATE - 1)/256;
    UCSR0C =(1<<UCSZ01)|(1<<UCSZ00);   /* USART0: asynchronous 8N1 */
	UCSR0B= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);/* USART0: TX and RX and RXCIE enabled */
	//UCSR0B= 1<<TXEN0;/* USART0: TX enabled */
	stdout = &stdinouterr;
	stdin  = &stdinouterr;
	stderr = &stdinouterr;
    sei ();
}


/**
 ** getchar:
 **
 ** getchar() reads the next character from the console interface
 **
 ** Returns: the character read in
 **/
int _getchar(FILE *stream)
{
	return uart_data;
}


/**
 * putchar:
 * @c: -input- the byte to be sent
 *
 * putchar() transmits the @c byte via the console interface
 *
 * Returns: none
 */
int	_putchar(char c, FILE *stream)
{
	if(c == '\n')	
	{
		while((UCSR0A & (1<<UDRE0)) == 0);
		UDR0 = '\r';
	}

	while((UCSR0A & (1<<UDRE0)) == 0);
	UDR0 = c;

	return	0;
}

/**
 * putstring:
 * @c: -input- the byte to be sent
 *
 * putchar() transmits the @c byte via the console interface
 *
 * Returns: none
 */
int	printString(unsigned char *str, int count)
{
  unsigned char c;
	int index = 0;
	while(index < count)
	{
		c = str[index] ;
		index++;
		while((UCSR0A & (1<<UDRE0)) == 0);
		UDR0 = c;
	}
	return	0;
}

