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
#ifndef _OS_CPU_H
#define _OS_CPU_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#ifndef sbi
/* avr-libc 1.2.3 doesn't include these anymore. */
#define sbi(port, bit) ((port) |= _BV(bit))
#define cbi(port, bit) ((port) &= ~_BV(bit))
#define inp(port) (port)
#define inb(port) (port)
#define outp(value, port) ((port) = (value))
#define outb(port, value) ((port) = (value))
#define inw(port) (*(volatile uint16_t *)&(port))
#define outw(port, value) ((*(volatile uint16_t *)&(port)) = (value))
#define PRG_RDB(addr) pgm_read_byte(addr)
#endif

#if ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 3))
#define __outw(val, port) outw(port, val);
#endif

#ifndef __inw
#ifndef __SFR_OFFSET
#define __SFR_OFFSET 0
#endif /* !__SFR_OFFSET */
#define __inw(_port) inw(_port)

#define __inw_atomic(__sfrport) ({				\
	uint16_t __t;					\
	bool bStatus;					\
	bStatus = bit_is_set(SREG,7);			\
	cli();						\
	__t = inw(__sfrport);				\
	if (bStatus) sei();				\
	__t;						\
 })

#endif /* __inw */

#define MCU_ASSIGN_PIN(name, port, bit) \
static inline void MCU_SET_##name##_PIN() {sbi(PORT##port , bit);} \
static inline void MCU_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
static inline int MCU_READ_##name##_PIN() \
  {return (inp(PIN##port) & (1 << bit)) != 0;} \
static inline void MCU_MAKE_##name##_OUTPUT() {sbi(DDR##port , bit);} \
static inline void MCU_MAKE_##name##_INPUT() {cbi(DDR##port , bit);} 

#define MCU_ASSIGN_OUTPUT_ONLY_PIN(name, port, bit) \
static inline void MCU_SET_##name##_PIN() {sbi(PORT##port , bit);} \
static inline void MCU_CLR_##name##_PIN() {cbi(PORT##port , bit);} \
static inline void MCU_MAKE_##name##_OUTPUT() {;} 

#define MCU_ALIAS_OUTPUT_ONLY_PIN(alias, connector)\
static inline void MCU_SET_##alias##_PIN() {MCU_SET_##connector##_PIN();} \
static inline void MCU_CLR_##alias##_PIN() {MCU_CLR_##connector##_PIN();} \
static inline void MCU_MAKE_##alias##_OUTPUT() {} \

#define MCU_ALIAS_PIN(alias, connector) \
static inline void MCU_SET_##alias##_PIN() {MCU_SET_##connector##_PIN();} \
static inline void MCU_CLR_##alias##_PIN() {MCU_CLR_##connector##_PIN();} \
static inline char MCU_READ_##alias##_PIN() {return MCU_READ_##connector##_PIN();} \
static inline void MCU_MAKE_##alias##_OUTPUT() {MCU_MAKE_##connector##_OUTPUT();} \
static inline void MCU_MAKE_##alias##_INPUT()  {MCU_MAKE_##connector##_INPUT();} 


/* Watchdog Prescaler*/
enum {
  MCU_period16 = 0x00, // 47ms
  MCU_period32 = 0x01, // 94ms
  MCU_period64 = 0x02, // 0.19s
  MCU_period128 = 0x03, // 0.38s
  MCU_period256 = 0x04, // 0.75s
  MCU_period512 = 0x05, // 1.5s
  MCU_period1024 = 0x06, // 3.0s
  MCU_period2048 = 0x07 // 6.0s
};

void MCU_wait();
// atomic statement runtime support

typedef uint8_t os_atomic_t;

inline os_atomic_t os_atomic_begin(void);

inline void os_atomic_end(os_atomic_t oldSreg);
inline void os_sleep();

inline void os_enable_interrupt();

inline void os_disable_interrupt();

#endif //OS_CPU_H
