#ifndef _KERTABLE_PLAT_H_
#define _KERTABLE_PLAT_H_

#include <kertable_proc.h>

#define PLAT_KER_TABLE		\
/*  1 */ (void*)ker_radio_ack_enable,		\
/*  2 */ (void*)ker_radio_ack_disable,		\
/*  3 */ (void*)ker_led,

#define PLAT_KERTABLE_LEN 3
#define PLAT_KERTABLE_END (PROC_KERTABLE_END+PLAT_KERTABLE_LEN)

#endif
