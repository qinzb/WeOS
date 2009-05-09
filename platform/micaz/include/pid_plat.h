/**
 * @brief    Micaz device related routines 
 * @author   Ram Kumar {ram@ee.ucla.edu}
 */

#ifndef _PID_PLAT_H
#define _PID_PLAT_H

#include <sos_types.h>
#include <pid_proc.h>

/**
 * @brief device pid list
 */
/*
 * Communication PID's should match with cyclops 
 * so that it can interact with micaz.
 * As a result, offsets 3 and 4 are free and can be used
 * for any other process.
 */
enum
  {
    MICASB_PID           = (PROC_MAX_PID + 1),
    EXFLASH_PID          = (PROC_MAX_PID + 2),
    KER_UART_PID         = (PROC_MAX_PID + 5),
    KER_I2C_PID          = (PROC_MAX_PID + 6),
    KER_I2C_MGR_PID      = (PROC_MAX_PID + 7), // pid for I2C Messaging manager 
  };
// Note:- Upate PLAT_MAX_PID

#define PLAT_MAX_PID         (PROC_MAX_PID + 7)
//#define PLAT_MAX_PID         (PROC_MAX_PID + 5)
#endif

