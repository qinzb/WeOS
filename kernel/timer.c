#include <os_cpu.h>
#include <timer.h>
#include <stdlib.h>
#include <hardware_timer.h>
#include <sched.h>

//#define TIMER_DBG

#ifdef TIMER_DBG
#include <usart.h>
#include <stdio.h>
#endif

/**timer global variables**/
os_timer_p active_list_head;	//active timer list head 
uint8_t last_interval;
#define MAX_INTERVAL 0xFF


void
os_init_timer (void)
{
  active_list_head = NULL;
  last_interval = 0;
}

bool
os_delete_timer (os_timer_p timer)
{				//    timer -> prev -> next = timer -> next; //       timer -> next -> prev = timer -> prev; if(timer != NULL)
  os_stop_timer (timer);
  if (timer->status == timer_stop)
    {
      free (timer);
      return true;
    }

  return false;
}

os_timer_p
os_create_timer (uint32_t interval, bool repeat, timer_callback_t callback)
{
  os_timer_p new_timer = (os_timer_p) (malloc (sizeof (os_timer_t)));
  new_timer->prev = NULL;
  new_timer->next = NULL;
  new_timer->status = timer_stop;
  new_timer->ticks = interval;
  new_timer->ticks_left = interval;
  new_timer->type = repeat;
  new_timer->func = callback;
  return new_timer;
}

void
os_set_timer_interval (os_timer_p timer, uint32_t interval)
{
  timer->ticks = interval;
  timer->ticks_left = interval;
}

void
os_set_timer_type (os_timer_p timer, bool repeat)
{
  timer->type = repeat;
}

void
os_set_timer_callback (os_timer_p timer, timer_callback_t callback)
{
  timer->func = callback;
}

bool
os_start_timer (os_timer_p timer)
{
  os_timer_p t, bt;
  uint8_t elapse_time;
  uint8_t sreg;

  sreg = os_atomic_begin ();
  if ((timer == NULL) || (timer->status == timer_run))
    {
      os_atomic_end (sreg);
      return false;
    }

  timer->status = timer_run;
  if (active_list_head == NULL)
    {
      active_list_head = timer;
      last_interval = 0;	//last interval should be set to 0.
      os_adjust_timer ();
      comp_interrupt_enable ();		/**Timer0 comp interrupt enable**/
      os_atomic_end (sreg);
      return true;
    }

  elapse_time = read_counter () - read_interval ();	// calculate elapse time
  elapse_time = last_interval - elapse_time;	// calculate elapse time

  timer->ticks_left += elapse_time;

  t = active_list_head;
  bt = NULL;
  while ((timer->ticks_left > t->ticks_left) && (t != NULL))
    {
      bt = t;
      t = t->next;
    }
  if (bt == NULL)
    {
      active_list_head = timer;
      timer->next = t;
      t->prev = timer;
      if (timer->ticks_left < last_interval)
	{
	  last_interval = elapse_time;
	  os_adjust_timer ();
	}
    }
  else if (t == NULL)
    {
      timer->prev = bt;
      bt->next = timer;
    }
  else
    {
      timer->next = t;
      timer->prev = bt;
      bt->next = timer;
      t->prev = timer;
    }
  os_atomic_end (sreg);
  return true;
}

bool
os_restart_timer (os_timer_p timer)
{
  os_timer_p t, bt;
  if ((timer == NULL) || (timer->status == timer_run))
    {
      return false;
    }

  timer->status = timer_run;
  if (active_list_head == NULL)
    {
      active_list_head = timer;
      comp_interrupt_enable ();		/**Timer0 comp interrupt enable**/
      return true;
    }

  t = active_list_head;
  bt = NULL;
  while ((timer->ticks_left > t->ticks_left) && (t != NULL))
    {
      bt = t;
      t = t->next;
    }
  if (bt == NULL)
    {
      active_list_head = timer;
      timer->next = t;
      t->prev = timer;
    }
  else if (t == NULL)
    {
      timer->prev = bt;
      bt->next = timer;
    }
  else
    {
      timer->next = t;
      timer->prev = bt;
      bt->next = timer;
      t->prev = timer;
    }
  return true;
}

bool
os_stop_timer (os_timer_p timer)
{
  uint8_t sreg;

  if ((timer == NULL) && (timer->status == timer_stop))
    return false;

  sreg = os_atomic_begin ();
  if ((timer->prev == NULL) && (timer->next == NULL))
    {
      active_list_head = NULL;
    }
  else if (timer->prev == NULL)
    {
      active_list_head = timer->next;
      active_list_head->prev = NULL;
    }
  else if (timer->next == NULL)
    {
      timer->prev->next = NULL;
    }
  else
    {
      timer->prev->next = timer->next;
      timer->next->prev = timer->prev;
    }

  timer->prev = NULL;
  timer->next = NULL;
  timer->status = timer_stop;
  timer->ticks_left = timer->ticks;

  if (active_list_head == NULL)
    {
      comp_interrupt_disable ();
      last_interval = 0;
    }
  os_atomic_end (sreg);

  return true;
}


void
os_adjust_timer ()
{
  os_timer_p t;
  uint8_t next_interval;
  uint8_t sreg;
  t = active_list_head;
  if (t == NULL)
    {
      return;
    }
  if (last_interval != 0)
    {
      while (t != NULL)
	{
	  t->ticks_left -= last_interval;
	  if (t->ticks_left == 0)
	    {
	      //bypass scheduler
	      //(t -> func)();
	      post_task (t->func);
	    }
	  t = t->next;
	}
    }

  t = active_list_head;
  while (t != NULL)
    {
      if (t->ticks_left == 0)
	{
	  os_stop_timer (t);
	  if (t->type == true)
	    os_restart_timer (t);
	}
      t = t->next;
    }

  t = active_list_head;
  if (t->ticks_left >= MAX_INTERVAL)
    {
      next_interval = MAX_INTERVAL;
      last_interval = next_interval;
    }
  else
    {
      next_interval = t->ticks_left;
      last_interval = next_interval;
    }
  next_interval += read_counter ();
  set_interval (next_interval);
#ifdef TIMER_DBG
  printf ("li:%d\n", last_interval);
#endif
}


void
os_timer_comp_isr ()
{
  os_adjust_timer ();
}
