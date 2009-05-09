/*****************************************************************************
 *
 * Author: Qinzb
 *
 * Description :
 *    This file contains the type- / data- and function definitions
 *    for the high part of the console driver
 *
 ****************************************************************************/

//#ifndef	_IOHIGH_H
//#define	_IOHIGH_H
//#endif	/* _IOHIGH_H */

#ifndef USART_H
#define USART_H

#define Fosc 7372800UL //7.3728MHz Oscillator
#define BAUDRATE 115200UL

/* use the console interface for communication with the application */
#define CONFIG_CONSOLE 1

/* allow some printf output during program execution */
//#define CONFIG_PRINTF 1

/* size in char of the USART's input queue */
//#define CONFIG_CONSOLE_QUEUE_SIZE (16)

/* size in char of the maximal printf line */
//#define CONFIG_PRINTF_LINE_SIZE (80)

/* size in char of the maximal console line */
//#define CONFIG_CONSOLE_LINE_SIZE (80)


#ifdef	CONFIG_CONSOLE
void console_init(void);
//int	kbhit(void);
//char    *read_line(char buf[]);
//void uart0RecvEnable(void);
//void uart0RecvDisable(void);
//void uart0HandshakeMode(void);
int	printString(unsigned char *str ,int count);
#endif	/* CONFIG_CONSOLE */

#endif /* USART_H */

