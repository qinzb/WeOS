/**
 * \file codemem_conf.h
 * \brief Code Memory Configuration for Mica2 platform
 */

#ifndef _CODEMEM_CONF_H
#define _CODEMEM_CONF_H
#include <sos_inttypes.h>
//! the type used to represent code space address
//! since the maximum size of a module is 4K, we only need 16 bits
typedef uint16_t code_addr_t;

//! the type used to represent the page number in the flash
typedef uint16_t code_page_t; 

// Ram - Things to do for the SFI Mode
// 1. Copy CODEMEM_START_PAGE and CODEMEM_END_PAGE to cross_domain_cf_asm.S
// 2. Modify avr/Makerules to change relocation of the SFI Jumptable

enum {
	//! underlying flash page size (bytes)
	FLASH_PAGE_SIZE = 256L,
	//! starting page number that can be used for loadable module
	//CODEMEM_START_PAGE = ((uint16_t)(&__text_end) / 256) + 1,
#ifdef INCL_VM
	CODEMEM_START_PAGE = 224L,
#else
	CODEMEM_START_PAGE = 192L,
#endif
#ifdef SOS_SFI
	//! last page number that ca be used for loadable module
	//! Pages 470 - 479 used for SFI jumptable
	CODEMEM_END_PAGE = 469L,
#else
	//! last page number that ca be used for loadable module
	CODEMEM_END_PAGE = 495L,
#endif // SOS_SFI
	//! starting address of loadable module (bytes) : 48K
	CODEMEM_START_ADDR = FLASH_PAGE_SIZE * CODEMEM_START_PAGE, 
	//! the size of code memory that can be used for loadable module
	//! 76K or 19 modules.
	CODEMEM_SIZE   = FLASH_PAGE_SIZE * (CODEMEM_END_PAGE + 1L) - CODEMEM_START_ADDR,
	//! maximum range of relative addressing (pages)
	//MAX_RANGE_REL_ADDRESSING_PAGES = 16L,
	MAX_RANGE_REL_ADDRESSING_PAGES = 32L,
	//! maximum range of relative addressing (bytes)
	MAX_RANGE_REL_ADDRESSING = FLASH_PAGE_SIZE * MAX_RANGE_REL_ADDRESSING_PAGES,
	//! code space addressing moode
	//! AVR uses word address instead of byte address in the code space
	//! we use 2 bytes to model this.
	CODEMEM_ADDRESSING_BYTES = 2L,
	FLASHMEM_SIZE            = (480L * 256L),
	FLASHMEM_PAGE_SIZE       = 256L,
	NUM_COMPILED_MODULES     = 16,
	CODEMEM_MAX_LOADABLE_MODULES = 16,
};

#endif
