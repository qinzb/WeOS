/*									tab:4
 * "Copyright (c) 2000-2003 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Copyright (c) 2002-2003 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
#ifndef _EXFLASH_H
#define _EXFLASH_H

#include <plat_msg_types.h>
// EEPROM characteristics
enum {
  EXFLASH_MAX_PAGES = 2048,
  EXFLASH_PAGE_SIZE = 264,
  EXFLASH_PAGE_SIZE_LOG2 = 8 // For those who want to ignore the last 8 bytes
};

//! Used for eraseKind
enum {
  EXFLASH_ERASE,
  EXFLASH_DONT_ERASE,
  EXFLASH_PREVIOUSLY_ERASED
};

typedef uint16_t exflashpage_t;
typedef uint16_t exflashoffset_t; /* 0 to EXFLASH_PAGE_SIZE - 1 */

int8_t exflash_init();
/**
 * @brief read n bytes from external flash
 * @param pid module id
 * @param page flash page number, MAX = EXFLASH_MAX_PAGES - 1
 * @param offset starting byte in the page
 * @param reqdata data buffer
 * @param n buffer size
 * @return errno
 * @event MSG_EXFLASH_READDONE param->byte = success code (0, 1)
 */
int8_t ker_exflash_read(sos_pid_t pid,
		exflashpage_t page, exflashoffset_t offset,
		void *reqdata, exflashoffset_t n);

/**
 * @brief compute crc 
 * @param pid module id
 * @param page flash page number, MAX = EXFLASH_MAX_PAGES - 1
 * @param offset starting byte in the page
 * @param n number of bytes to compute
 * @return errno
 * @event MSG_EXFLASH_CRCDONE param->byte = success code (0, 1) param->word = crc
 * 
 */
int8_t ker_exflash_computeCrc(sos_pid_t pid,
		exflashpage_t page,
		exflashoffset_t offset,
		exflashoffset_t n);
/**
 * @brief write n bytes to external flash
 * @param pid module id
 * @param page flash page number, MAX = EXFLASH_MAX_PAGES - 1
 * @param offset starting byte in the page
 * @param reqdata data buffer
 * @param n buffer size
 * @return errno
 * @event MSG_EXFLASH_WRITEDONE param->byte = success code (0, 1)
 */
int8_t ker_exflash_write(sos_pid_t pid, 
		exflashpage_t page, exflashoffset_t offset,
		void *reqdata, exflashoffset_t n);
/**
 * @brief erase a page
 * @param pid module id
 * @param page flash page number, MAX = EXFLASH_MAX_PAGES - 1
 * @eraseKind type of erase 
 * @event MSG_EXFLASH_ERASEDONE param->byte = success code (0, 1)
 */
int8_t ker_exflash_erase(sos_pid_t pid, exflashpage_t page, uint8_t eraseKind);

/**
 * @brief synchronize a page between buffer and main memory
 * @param pid module id
 * @param page flash page number, MAX = EXFLASH_MAX_PAGES - 1
 * @event MSG_EXFLASH_SYNCDONE param->byte = success code (0, 1)
 *
 * This will first compare the difference between flash buffer and main memory
 * If there are differences, the buffer will be copied to main memory
 */
int8_t ker_exflash_sync(sos_pid_t pid, exflashpage_t page);

/**
 * @brief flush a page from flash buffer to main memory
 * @param pid module id
 * @param page flash page number, MAX = EXFLASH_MAX_PAGES - 1
 * @event MSG_EXFLASH_FLUSHDONE param->byte = success code (0, 1)
 *
 * This function is similiar to ker_exflash_sync except it does not compare
 */
int8_t ker_exflash_flush(sos_pid_t pid, exflashpage_t page);


/**
 * @brief synchronize all buffer with main memory
 * @param pid module id
 * @event MSG_EXFLASH_SYNCDONE param->byte = success code (0, 1)
 */
int8_t ker_exflash_syncAll(sos_pid_t pid);


/**
 * @brief flush all flash buffers to main memory
 * @param pid module id
 * @event MSG_EXFLASH_FLUSHDONE param->byte = success code (0, 1)
 */
int8_t ker_exflash_flushAll(sos_pid_t pid);

#endif
