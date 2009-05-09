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

#include <sched.h>
#include <os_cpu.h>

typedef struct {
  void (*tp) ();
} sched_entry_t;

enum {
#ifdef OS_MAX_TASKS_LOG2
#if OS_MAX_TASKS_LOG2 > 8
#error "Maximum of 256 tasks, OS_MAX_TASKS_LOG2 must be <= 8"
#endif
  OS_MAX_TASKS = 1 << OS_MAX_TASKS_LOG2,
#else
  OS_MAX_TASKS = 8,
#endif
  OS_TASK_BITMASK = (OS_MAX_TASKS - 1)
};

volatile sched_entry_t os_queue[OS_MAX_TASKS];
uint8_t os_sched_full;
volatile uint8_t os_sched_free;

void os_sched_init(void)
{
  int i;
  os_sched_free = 0;
  os_sched_full = 0;
  for (i = 0; i < OS_MAX_TASKS; i++)
    os_queue[i].tp = NULL;
}

bool post_task(void (*tp) ());


/*
 * post_task (thread_pointer)
 *  
 * Put the task pointer into the next free slot.
 * Return 1 if successful, 0 if there is no free slot.
 *
 * This function uses a critical section to protect os_sched_free.
 * As tasks can be posted in both interrupt and non-interrupt context,
 * this is necessary.
 */
bool post_task(void (*tp) ()) {
  uint8_t tmp;
  os_atomic_t sreg;
  //  dbg(DBG_SCHED, ("os_post: %d 0x%x\n", os_sched_free, (int)tp));
  sreg = os_atomic_begin();
  tmp = os_sched_free;
  if (os_queue[tmp].tp == NULL) {
    os_sched_free = (tmp + 1) & OS_TASK_BITMASK;
    os_queue[tmp].tp = tp;
  os_atomic_end(sreg);
    return true;
  }
  else {	
  os_atomic_end(sreg);
    return false;
  }
}


/*
 * os_schedule_task()
 *
 * Remove the task at the head of the queue and execute it, freeing
 * the queue entry. Return 1 if a task was executed, 0 if the queue
 * is empty.
 */

bool os_run_next_task ()
{
  uint8_t old_full;
  void (*func)(void);
  os_atomic_t sreg;
  sreg = os_atomic_begin(); 
  old_full = os_sched_full;
  func = os_queue[old_full].tp;
  if (func == NULL)
    {
      os_sleep();
      return 0;
    }

  os_queue[old_full].tp = NULL;
  os_sched_full = (old_full + 1) & OS_TASK_BITMASK;
  os_atomic_end(sreg);
  func();

  return 1;
}

void os_run_task() {
  for (;;)
    os_run_next_task();
}
