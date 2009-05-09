/* -*- Mode: C; tab-width:4 -*- */
/* ex: set ts=4 shiftwidth=4 softtabstop=4 cindent: */
/*
 * Copyright (c) 2003 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgement:
 *       This product includes software developed by Networked &
 *       Embedded Systems Lab at UCLA
 * 4. Neither the name of the University nor that of the Laboratory
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * @brief    define bit operations
 * @author   Hubert Wu {hubertwu@cs.ucla.edu}
 */

#ifndef _BITS_OP_H
#define _BITS_OP_H

/*************************************************************************
 * define the bit operations for port/variable                           *
 *-----------------------------------------------------------------------* 
 *  SETBITLOW(X,I):   set the Ith bit of X to 0                          *
 *  SETBITHIGH(X,I):  set the Ith bit of X to 1                          *
 *  GETBIT(X,I):      get the Ith bit of X                               *
 *  MASKBITS(I,J):    get a mask for bits from I to J                    *  
                      MASKBITS(3,0) returns 0xfff0                       * 
 *  GETBITS(X,I,J):   get the bits from I to J of variable X             *
 *                    X=0xffff; GETBITS(X,3,0) returns 0x000f            *
 *  SETBITS(X,C,I,J): put C on the postion from I to J in X              *
 *                    X=0xffff; C=1; SETBITS(X,C,15,12) makes X=0x1fff   *
 *************************************************************************/
#define SETBITLOW(X,I)		( (X) &= ~(1<<I) )
#define SETBITHIGH(X,I)		( (X) |= (1<<I) )
#define GETBIT(X,I)		( ( (X) & (1<<I) ) >> I ) 
#define MASKBITS(I,J)		( ( 0xffff << (I+1) ) | ( 0xffff >> (16-J) ) )
#define GETBITS(X,I,J)		( ( (X) & (~MASKBITS(I,J)) ) >> J )  
#define SETBITS(X,C,I,J)	( ( (X) &= MASKBITS(I,J) ) && ( (X) |= ( C << J ) ) ) 

#endif //_BITS_OP_H

