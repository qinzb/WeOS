/*
 *
 * Authors:		Jason Hill, Philip Levis
 * Revision:		$Id: sched.c,v 1.8 2005/06/20 22:08:24 idgay Exp $
 * Modifications:       Removed unecessary code, cleanup.(5/30/02)
 *
 *                      Moved from non-blocking list to simple
 *                      critical section.  Changed task queue to
 *                      length 8 (more efficient). (3/10/02)
 */




/*
 * Scheduling data structrues
 *
 * There is a list of size MAX_TASKS, stored as an cyclic array buffer.
 * os_sched_full is the index of first used slot (head of list).
 * os_sched_free is the index of first free slot (after tail of list).
 * If free equals full, the list is empty.
 * The list keeps at least one empty slot; one cannot add a task if
 * advancing free would make it equal to full.
 *
 * Each entry consists of a task function pointer.
 *
 */

#ifndef _SCHED_H
#define _SCHED_H

#include <types.h>

extern void os_sched_init(void);
extern bool post_task(void (*tp) ());
extern bool os_run_next_task (void);
extern void os_run_task(); 

#endif

