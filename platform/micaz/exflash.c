/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4 shiftwidth=4 softtabstop=4 cindent: */
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
/*
 *
 * Authors:		Jason Hill, David Gay, Philip Levis
 * Date last modified:  6/25/02
 *
 */

/**
 * Low level hardware access to the onboard Flash 
 * @author Jason Hill
 * @author David Gay
 * @author Philip Levis
 * @author Simon Han
 *    Merge PageEEPROM and HPLFlash
 */
#include <hardware.h>
#include <sos_module_types.h>
#include <sos_sched.h>
#include <message.h>
#include "crc.h"

static inline void TOSH_uwait(uint16_t _u_sec) {
	while (_u_sec > 0) {
		/* XXX SIX nop will give 8 cycles, which is 1088 ns */
		asm volatile  ("nop" ::);
		asm volatile  ("nop" ::);
		asm volatile  ("nop" ::);
		asm volatile  ("nop" ::);
		asm volatile  ("nop" ::);
		asm volatile  ("nop" ::);
		asm volatile  ("nop" ::);
		asm volatile  ("nop" ::);
		_u_sec--;
	}
}

/*---------------------------------------------------------------------*/
/* Typedefs                                                            */
/*---------------------------------------------------------------------*/
enum { // requests
	IDLE,
	R_READ,
	R_READCRC,
	R_WRITE,
	R_ERASE,
	R_SYNC,
	R_SYNCALL,
	R_FLUSH,
	R_FLUSHALL
};                                                          

enum {                                                      
	P_SEND_CMD,                                               
	P_READ,                                                   
	P_READCRC,                                                
	P_WRITE,                                                  
	P_FLUSH,                                                  
	P_FILL,                                                   
	P_ERASE,                                                  
	P_COMPARE,                                                
	P_COMPARE_CHECK                                           
};                                                          

enum { // commands we're executing (all SPI Mode 0 or 3)    
	C_READ_BUFFER1 = 0xd4,                                    
	C_READ_BUFFER2 = 0xd6,                                    
	C_WRITE_BUFFER1 = 0x84,                                   
	C_WRITE_BUFFER2 = 0x87,                                   
	C_FILL_BUFFER1 = 0x53,                                    
	C_FILL_BUFFER2 = 0x55,                                    
	C_FLUSH_BUFFER1 = 0x83,                                   
	C_FLUSH_BUFFER2 = 0x86,                                   
	C_QFLUSH_BUFFER1 = 0x88,                                  
	C_QFLUSH_BUFFER2 = 0x89,                                  
	C_COMPARE_BUFFER1 = 0x60,                                 
	C_COMPARE_BUFFER2 = 0x61,                                 
	C_REQ_STATUS = 0xd7,                                      
	C_ERASE_PAGE = 0x81                                       
};                                                          

/*---------------------------------------------------------------------*/
/* Global Data  Declarations                                           */
/*---------------------------------------------------------------------*/
static int8_t exflash_handler(void *state, Message *e);
static mod_header_t mod_header SOS_MODULE_HEADER ={    
mod_id : EXFLASH_PID,    
state_size : 0,    
num_sub_func : 0,   
num_prov_func : 0,   
module_handler: exflash_handler,    
};
static sos_module_t exflash_module;

static uint8_t request;                                            
static uint8_t *reqBuf;                                            
static exflashoffset_t reqOffset, reqBytes;                     
static exflashpage_t reqPage;                                       

/* deselect of Ex-flash requested (needed between two commands) */ 
static bool deselectRequested; 
static bool broken; // Write failed. Fail all subsequent requests. 
static bool compareOk;                                             

static uint8_t cmdPhase, waiting;                                  
static uint8_t *data, cmd[4];                                      
static uint8_t cmdCount;                 
static exflashoffset_t dataCount;                               
static uint16_t computedCrc;                                       

static struct {                                                    
	exflashpage_t page;                                        
	bool busy : 1;                                            
	bool clean : 1;                                           
	bool erased : 1;                                          
	uint8_t unchecked : 2;                                    
} buffer[2];                                                
static uint8_t selected; // buffer used by the current op          
static uint8_t checking;                                           
static bool flashBusy;                                             
static sos_pid_t reqPid;

/*---------------------------------------------------------------------*/
/* Function Declarations (Internal)                                    */
/*---------------------------------------------------------------------*/
static bool FlashSelect_notifyHigh();
static void FlashIdle_available();
static void requestDone(bool result);
static void execCommand(bool wait, uint8_t reqCmd, uint8_t dontCare, 
		exflashpage_t page, exflashoffset_t offset);
static void handleRWRequest();
static int8_t syncOrFlushAll(sos_pid_t pid, uint8_t newReq);


/*---------------------------------------------------------------------*/
/* Message Handler                                                     */
/*---------------------------------------------------------------------*/
static int8_t exflash_handler(void *state, Message *e)
{
	switch(e->type) {
		case MSG_EXFLASH_DESELECT: 
		{
			FlashSelect_notifyHigh();
			return SOS_OK;	
		}
		case MSG_EXFLASH_AVAIL:
		{
			FlashIdle_available();
			return SOS_OK;

		}
	}
	return SOS_OK;
}

/*---------------------------------------------------------------------*/
/* Function Definitions (Internal)                                     */
/*---------------------------------------------------------------------*/

#define DISABLE_FLASH_INTERRUPT()  (EIMSK &= ~_BV(2))
#define ENABLE_FLASH_INTERRUPT()  (EIMSK |= _BV(2))

#define BITINIT \
uint8_t clrClkAndData = (PORTD & ~0x28)

#define BIT(n) \
	PORTD = clrClkAndData; \
	asm __volatile__ \
        (  "sbrc %2," #n "\n" \
	 "\tsbi 18,3\n" \
	 "\tsbi 18,5\n" \
	 "\tsbic 16,2\n" \
	 "\tori %0,1<<" #n "\n" \
	 : "=d" (spiIn) : "0" (spiIn), "r" (spiOut))

// Select a command for the current buffer                  
#define OPN(n, name) ((n) ? name ## 1 : name ## 2)            
#define OP(name) OPN(selected, name)          

// We use SPI mode 0 (clock low at select time)
/* async command */
static uint8_t FlashSPI_txByte(uint8_t spiOut) {
    uint8_t spiIn = 0;
	HAS_CRITICAL_SECTION;

    // This critical section ensures integrity at the hardware level...
	ENTER_CRITICAL_SECTION();
	{
		BITINIT;

		BIT(7);
		BIT(6);
		BIT(5);
		BIT(4);
		BIT(3);
		BIT(2);
		BIT(1);
		BIT(0);
	}
	LEAVE_CRITICAL_SECTION();
	return spiIn;
}

/**
 * Check FLASH status byte.
 * @return true if the flash is ready, FALSE if not.
 *   In the true case, the full status byte may not have been
 *   read out of the flash, in the FALSE case it is fully read out.
 */

static bool FlashIdle_wait() {
	bool waits;
	HAS_CRITICAL_SECTION;

	// Setup interrupt on rising edge of flash in
	ENTER_CRITICAL_SECTION();
	{
		EIFR = 1 << 2; // clear any pending interrupt
		ENABLE_FLASH_INTERRUPT();
		CLR_FLASH_CLK();
		// We need to wait at least 2 cycles here (because of the signal
		// acquisition delay). It's also good to wait a few microseconds
		// to get the fast ("FAIL") exit from wait (reads are twice as fast
		// with a 2us delay...)
		TOSH_uwait(2);

		if (READ_FLASH_IN())
		{
			// already high
			DISABLE_FLASH_INTERRUPT();
			waits = false;
		}
		else {
			waits = true;
		}
	}
	LEAVE_CRITICAL_SECTION();
    return waits;
  }


SIGNAL(SIG_INTERRUPT2) {
	DISABLE_FLASH_INTERRUPT();
	EIMSK &= ~_BV(2); // disable interrupt
	post_short(EXFLASH_PID, EXFLASH_PID, MSG_EXFLASH_AVAIL, 0, 0, 0);
}

static inline bool getCompareStatus() {
	SET_FLASH_CLK();
	CLR_FLASH_CLK();
	// Wait for compare value to propagate
	asm volatile("nop");
	asm volatile("nop");
	return !READ_FLASH_IN();
}

int8_t exflash_init() 
{
	SET_FLASH_SELECT_DD_OUT();
	SET_FLASH_SELECT();

	SET_FLASH_CLK_DD_OUT();
	CLR_FLASH_CLK();
	SET_FLASH_OUT_DD_OUT();
	SET_FLASH_OUT();
	SET_FLASH_IN_DD_IN();
	CLR_FLASH_IN();

	DISABLE_FLASH_INTERRUPT();
	EICRA |= 0x30; // make flash in a rising-edge interrupt

	request = IDLE;
	waiting = deselectRequested = false;
	flashBusy = true;

	// pretend we're on an invalid non-existent page
	buffer[0].page = buffer[1].page = EXFLASH_MAX_PAGES;
	buffer[0].busy = buffer[1].busy = false;
	buffer[0].clean = buffer[1].clean = true;
	buffer[0].unchecked = buffer[1].unchecked = 0;
	buffer[0].erased = buffer[1].erased = false;

	reqPid = NULL_PID;
	sched_register_kernel_module(&exflash_module, sos_get_header_address(mod_header), NULL);
	return SOS_OK;
}

/* Select Ex-Flash, start a SPI transaction */
static inline void selectFlash()
{
	CLR_FLASH_CLK(); // ensure SPI mode 0
	CLR_FLASH_SELECT();
}

/* Deselect Ex-Flash */
static inline void requestDeselect()
{
	deselectRequested = true;
	SET_FLASH_SELECT();

	post_short(EXFLASH_PID, EXFLASH_PID, MSG_EXFLASH_DESELECT, 0, 0, 0);
}

static void FlashIdle_available()
{
	if (cmdPhase == P_COMPARE_CHECK)
		compareOk = getCompareStatus();
	requestDeselect();
}

static void requestFlashStatus() {
	waiting = true;
	selectFlash();

	FlashSPI_txByte(C_REQ_STATUS);
	if (FlashIdle_wait() == false) // already done
		FlashIdle_available();
}

static void sendFlashCommand() {
	uint8_t in = 0, out = 0;
	uint8_t *ptr = cmd;
	exflashoffset_t count = cmdCount;
	uint16_t crc = 0;
	uint8_t lphase = P_SEND_CMD;

	/* For a 3% speedup, we could use labels and goto *.
But: very gcc-specific. Also, need to do
asm ("ijmp" : : "z" (state))
instead of goto *state
	 */

	selectFlash();

	for (;;)
	{
		if (lphase == P_READCRC)
		{
			crc = crcByte(crc, in);

			--count;
			if (!count)
			{
				computedCrc = crc;
				break;
			}
		}
		else if (lphase == P_SEND_CMD)
		{
			out = *ptr++;
			count--;
			if (!count)
			{
				lphase = cmdPhase;
				ptr = data;
				count = dataCount;
			}
		}
		else if (lphase == P_READ)
		{
			*ptr++ = in;
			--count;
			if (!count)
				break;
		}
		else if (lphase == P_WRITE)
		{
			if (!count)
				break;

			out = *ptr++;
			--count;
		}
		else /* P_FILL, P_FLUSH, P_ERASE, P_COMPARE */
			break;

		in = FlashSPI_txByte(out);
	}

	requestDeselect();
}

static void checkBuffer(uint8_t buf) 
{                               
	cmdPhase = P_COMPARE;                                       
	checking = buf;                                             
	execCommand(true, OPN(buf, C_COMPARE_BUFFER), 0,            
			buffer[buf].page, 0);                                   
}                                                             

static void flushBuffer() 
{                                          
	cmdPhase = P_FLUSH;                                         
	execCommand(true, buffer[selected].erased ?                 
			OP(C_QFLUSH_BUFFER) :                                   
			OP(C_FLUSH_BUFFER), 0,                                  
			buffer[selected].page, 0);                              
}                                                  

static void flashCommandComplete() 
{
	if (waiting)
	{
		waiting = flashBusy = buffer[0].busy = buffer[1].busy = false;

		if (cmdPhase == P_COMPARE_CHECK)
		{
			if (compareOk)
				buffer[checking].unchecked = 0;
			else if (buffer[checking].unchecked < 2)
				buffer[checking].clean = false;
			else
			{
				broken = true; // write failed. refuse all further reqs
				requestDone(false);
				return;
			}
			handleRWRequest();
		}
		else
		{
			// Eager compare - this steals the current command
			if ((buffer[0].unchecked || buffer[1].unchecked) &&
					!(cmdPhase == P_COMPARE || cmdPhase == P_COMPARE_CHECK))
				checkBuffer(buffer[0].unchecked ? 0 : 1);
			else
				sendFlashCommand();
		}
		return;
	}
	switch (cmdPhase)
	{
		default: // shouldn't happen
			requestDone(false);
			break;

		case P_READ: case P_READCRC: case P_WRITE:
			requestDone(true);
			break;

		case P_FLUSH:
			flashBusy = true;
			buffer[selected].clean = buffer[selected].busy = true;
			buffer[selected].unchecked++;
			buffer[selected].erased = false;
			handleRWRequest();
			break;

		case P_COMPARE:
			cmdPhase = P_COMPARE_CHECK;
			flashBusy = true;
			buffer[checking].busy = true;
			// The 10us wait makes old mica motes (Atmega 103) happy, for
			// some mysterious reason (w/o this wait, the first compare
			// always fail, even though the compare after the rewrite
			// succeeds...)
			TOSH_uwait(10);
			requestFlashStatus();
			break;

		case P_FILL: // page load started                         
			flashBusy = true;                                           
			buffer[selected].page = reqPage;                            
			buffer[selected].clean = buffer[selected].busy = true;      
			buffer[selected].erased = false;                            
			handleRWRequest();                                          
			break;                                                      

		case P_ERASE:                                             
			flashBusy = true;                                           
			// The buffer contains garbage, but we don't care about the state
			// of bits on this page anyway (if we do, we'll perform a   
			// subsequent write)                                        
			buffer[selected].page = reqPage;                            
			buffer[selected].clean = true;                              
			buffer[selected].erased = true;                             
			requestDone(true);                                       
			break;                                                      
	}                                                         
}                                          

static bool FlashSelect_notifyHigh()
{                     
	if (deselectRequested)                                      
	{                                                         
		deselectRequested = false;                                  
		flashCommandComplete();                                     
	}                                                         
	return true;                                             
}                                                             

static void execCommand(bool wait, uint8_t reqCmd, uint8_t dontCare, 
		exflashpage_t page, exflashoffset_t offset)
{      
	// page (2 bytes) and highest bit of offset                 
	cmd[0] = reqCmd;                                            
	cmd[1] = page >> 7;                                         
	cmd[2] = page << 1 | offset >> 8;                           
	cmd[3] = offset; // low-order 8 bits                        
	cmdCount = 4 + dontCare;                                    

	if (wait && flashBusy)                                      
		requestFlashStatus();                                     
	else                                                        
		sendFlashCommand();                                       
}                                                             

static void execRWBuffer(uint8_t reqCmd, uint8_t dontCare, exflashoffset_t offset) 
{
	execCommand(buffer[selected].busy, reqCmd, dontCare, 0, offset);
}                                                             

static void handleRWRequest() 
{                                      
	if (reqPage == buffer[selected].page)  {                     
		switch (request)                                          
		{                                                           
			case R_ERASE:                                               
				switch (reqOffset)                                        
				{                                                       
					case EXFLASH_ERASE:                                  
						cmdPhase = P_ERASE;                                   
						execCommand(true, C_ERASE_PAGE, 0, reqPage, 0);       
						break;                                                
					case EXFLASH_PREVIOUSLY_ERASED:                      
						// We believe the user...                             
						buffer[selected].erased = true;                       
						/* Fallthrough */                                     
					case EXFLASH_DONT_ERASE:                             
						// The buffer contains garbage, but we don't care about the state   
						// of bits on this page anyway (if we do, we'll perform a 
						// subsequent write)                                  
						buffer[selected].clean = true;                        
						requestDone(true);                                 
						break;                                                
				}                                                       
				break;                                                    

			case R_SYNC: case R_SYNCALL:                                
				if (buffer[selected].clean && buffer[selected].unchecked) 
				{                                                       
					checkBuffer(selected);                                
					return;                                               
				}                                                       
				/* fall through */                                        
			case R_FLUSH: case R_FLUSHALL:                              
				if (!buffer[selected].clean) {
					flushBuffer();                                          
				} else if (request == R_FLUSH || request == R_SYNC) {
					requestDone(true);
				} else {                                                       
					// Check for more dirty pages                         
					uint8_t oreq = request;                               

					request = IDLE;                                       
					syncOrFlushAll(reqPid, oreq); 
				}                                                       
				break;                                                    

			case R_READ:                                                
				data = reqBuf;                                            
				dataCount = reqBytes;                                     
				cmdPhase = P_READ;                                        
				execRWBuffer(OP(C_READ_BUFFER), 2, reqOffset);            
				break;                                                    

			case R_READCRC:                                             
				dataCount = reqBytes;                                     
				cmdPhase = P_READCRC;                                     
				execRWBuffer(OP(C_READ_BUFFER), 2, 0);                    
				break;                                                    

			case R_WRITE:                                               
				data = reqBuf;                                            
				dataCount = reqBytes;                                     
				cmdPhase = P_WRITE;                                       
				buffer[selected].clean = false;                           
				buffer[selected].unchecked = 0;                           
				execRWBuffer(OP(C_WRITE_BUFFER), 0, reqOffset);           
				break;                                                    
		}                                                           
	} else if (!buffer[selected].clean) {                         
		flushBuffer();                                            
	} else if (buffer[selected].unchecked) {                     
		checkBuffer(selected);                                    
	} else {                                                         
		// just get the new page (except for erase)                 
		if (request == R_ERASE) { 
			buffer[selected].page = reqPage;                        
			handleRWRequest();                                      
		} else { 
			cmdPhase = P_FILL;                                      
			execCommand(true, OP(C_FILL_BUFFER), 0, reqPage, 0);    
		}                                                         
	}                                                         
}                                                             


static void requestDone(bool result)
{                           
	uint8_t orequest = request;                                 
	uint8_t msgtype;

	switch (orequest)                                           
	{   
		case R_READ: msgtype = MSG_EXFLASH_READDONE; break;
		case R_READCRC: 
		{
			msgtype = MSG_EXFLASH_CRCDONE; 
			post_short(reqPid, EXFLASH_PID, MSG_EXFLASH_CRCDONE, result, computedCrc, 0);
			request = IDLE;
			reqPid = NULL_PID;
			return;
		}
		case R_WRITE: msgtype = MSG_EXFLASH_WRITEDONE; break;
		case R_SYNC: msgtype = MSG_EXFLASH_SYNCDONE; break;
		case R_FLUSH: msgtype = MSG_EXFLASH_FLUSHDONE; break;
		case R_ERASE: msgtype = MSG_EXFLASH_ERASEDONE; break;
		default: return;
	}                                                         
	post_short(reqPid, EXFLASH_PID, msgtype, result, 0, 0);
	request = IDLE;
	reqPid = NULL_PID;
}                                                             

static int8_t newRequest(sos_pid_t pid, 
		uint8_t req, exflashpage_t page,           
		exflashoffset_t offset,                        
		void *reqdata, exflashoffset_t n)
{            
	if (page >= EXFLASH_MAX_PAGES || offset >= EXFLASH_PAGE_SIZE ||     
			n > EXFLASH_PAGE_SIZE || offset + n > EXFLASH_PAGE_SIZE)
		return -EINVAL;                                              

	if (request != IDLE || reqPid != NULL_PID)  {                                       return -EBUSY;  
	}

	if (broken)  { 
		return -EIO; 
	}                                            

	reqPid = pid;
	request = req;                                              
	reqBuf = reqdata;                                           
	reqBytes = n;                                               
	reqPage = page;                                             
	reqOffset = offset;                                         

	if (page == buffer[0].page)                                 
		selected = 0;                                             
	else if (page == buffer[1].page)                            
		selected = 1;                                             
	else                                                        
		selected = !selected; // LRU with 2 buffers...            

	handleRWRequest();                                          

	return SOS_OK;                                             
}                                                         

static int8_t syncOrFlush(sos_pid_t pid, exflashpage_t page, uint8_t newReq)
{     
	if (request != IDLE || reqPid != NULL_PID) 
		return -EBUSY;                                              

	if (broken) {  
		return -EIO;                                             
	} else if (buffer[0].page == page)  
		selected = 0;                                             
	else if (buffer[1].page == page)                            
		selected = 1;                                             
	else {                                                         
		reqPid = pid;
		request = newReq;                                           
		requestDone(true);
		return SOS_OK; 
	}                                                         

	reqPid = pid;
	request = newReq;                                           
	buffer[selected].unchecked = 0;                             
	handleRWRequest();                                          

	return SOS_OK;                                             
}                                                             


static int8_t syncOrFlushAll(sos_pid_t pid, uint8_t newReq)
{                     
	if (request != IDLE)                                        
		return -EBUSY;                                              

	if (broken) { 
		return -EIO;                                             
	}                                                         
	else if (!buffer[0].clean)                                  
		selected = 0;                                             
	else if (!buffer[1].clean)                                  
		selected = 1;                                             
	else {  
		reqPid = pid;
		request = newReq;                                           
		requestDone(true);
		return SOS_OK;                                             
	}                                                         

	reqPid = pid;
	request = newReq;                                           
	buffer[selected].unchecked = 0;                             
	handleRWRequest();                                          

	return SOS_OK;                                             
}                                                             



/*---------------------------------------------------------------------*/
/* Function Definitions (External)                                     */
/*---------------------------------------------------------------------*/

int8_t ker_exflash_read(sos_pid_t pid, 
		exflashpage_t page, exflashoffset_t offset,
		void *reqdata, exflashoffset_t n)
{       
	return newRequest(pid, R_READ, page, offset, reqdata, n);        
}                                                             


int8_t ker_exflash_computeCrc(sos_pid_t pid, 
		exflashpage_t page,     
		exflashoffset_t offset,                 
		exflashoffset_t n) 
{                    
	if (n == 0) {  
		reqPid = pid;
		request = R_READCRC;                                        
		computedCrc = 0;                                            
		requestDone(true);
		return SOS_OK;                                             
	}                                                         
	else                                                        
		return newRequest(pid, R_READCRC, page, offset, NULL, n);      
}                                                             

int8_t ker_exflash_write(sos_pid_t pid, 
		exflashpage_t page, exflashoffset_t offset,
		void *reqdata, exflashoffset_t n)
{      
	return newRequest(pid, R_WRITE, page, offset, reqdata, n);       
}                                                             

int8_t ker_exflash_erase(sos_pid_t pid, exflashpage_t page, uint8_t eraseKind)
{
	return newRequest(pid, R_ERASE, page, eraseKind, NULL, 0);       
}                                                             

int8_t ker_exflash_sync(sos_pid_t pid, exflashpage_t page)
{         
	return syncOrFlush(pid, page, R_SYNC);                           
}                                                             

int8_t ker_exflash_flush(sos_pid_t pid, exflashpage_t page)
{        
	return syncOrFlush(pid, page, R_FLUSH);                          
} 

int8_t ker_exflash_syncAll(sos_pid_t pid)
{                       
	return syncOrFlushAll(pid, R_SYNCALL);                           
}                                                             

int8_t ker_exflash_flushAll(sos_pid_t pid)
{                      
	return syncOrFlushAll(pid, R_FLUSHALL);                          
}                                                  

