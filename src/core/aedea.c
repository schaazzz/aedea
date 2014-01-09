/*!
 * \defgroup aedea API for Event-Driven Embedded Applications
 * @{
 *
 * AEDEA (pronounced "Idea") stands for "API for Event-Driven Embedded Applications".
 * This API provides a simple setup for run-to-completion (RTC) processes in an embedded
 * application with the added advantage of inter-process communication using events and
 * a delta queue implementation of software timers.
 */


/*!
 * \file
 * AEDEA implementation file.
 *
 * \author
 * Shahzeb Ihsan <shahzeb_ihsan@users.sourceforge.net>
 */


/*
 * Copyright (c) 2007, Shahzeb Ihsan.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *     
 *     2. Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *
 *     3. Neither the name of the author nor the names of its contributors may be
 *        used to endorse or promote products derived from this software without
 *        specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * This file is part of the AEDEA distribution.
 */


/*
 * ----- Header files -----
 */
#include "platform.h"
#include "options.h"
#include "aedea.h"


/*!
 * Queue structure.
 */
typedef struct
{
     void * buff_ptr;                        //!< Pointer to the queue.       
     port_uint_t num_items;                  //!< Number of items in the queue.                   
     port_uint_t item_size;                  //!< Size of a single queue item.               
     port_uint_t count;                      //!< Number of unread items in the queue.
     port_uint_t head;                       //!< Head pointer for the queue.          
     port_uint_t tail;                       //!< Tail pointer for the queue.          
}                                            
queue_t;


/*!
 * Process manager structure.
 */
typedef struct
{
     process_callback_t * callback;          //!< Pointer to the task callback function.
     void * process_arg_ptr;                 //!< Pointer to the argument to be passed to the process.
     uint8_t pid;                            //!< Process ID.
     port_int_t exec_delay;                  //!< Execution delay, the number of iterations of the process manager per process invocation.
     port_int_t iterations_to_exec;          //!< Stores the number of iterations until the next time the process is invoked.
     queue_t event_queue;                    //!< Process event queue.
}
proc_mgr_t;


/*!
 * Software timer structure.
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
typedef struct
{
     timeout_handler_t * handler;            //!< Pointer to the timeout handler function.
     void * handler_arg_ptr;                 //!< Pointer to the argument to be passed to the handler.
     uint8_t timer_id;                       //!< Timer ID.
     port_uint_t num_ticks;                  //!< Number of ticks after which this timer should timeout.
}
sw_tmr_t;
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- File specific variables -----
 */
static port_uint_t num_processes = 0;                       // Contains a count of the number of added processes.
static proc_mgr_t * active_proc_mgr = NULL;                 // Contains the pointer to the currently active process manager.

#if(AEDEA_OPT_USE_SOFT_TMR == 0)
static proc_mgr_t proc_mgrs[AEDEA_OPT_MAX_PROCESSES];       // Array of process managers for all added processes.
#endif

#if(AEDEA_OPT_USE_SOFT_TMR == 1)
static proc_mgr_t proc_mgrs[AEDEA_OPT_MAX_PROCESSES + 1];   // Array of process managers for all added processes ("+ 1" for the timer process).
#endif

#if(AEDEA_OPT_USE_SOFT_TMR == 1)
static port_uint_t num_timers = 0;                          // Contains a count of the number of installed timeout handlers.
static sw_tmr_t sw_tmrs[AEDEA_OPT_MAX_SOFT_TMRS];           // Array of software timers for all installed timeout handlers.
static port_uint_t active_tmrs_index = 0;                   // Contains the sw_tmrs index of the active software timer with lowest timeout value.

static sw_tmr_t exp_tmrs[AEDEA_OPT_MAX_SOFT_TMRS];          // Contains a list of all expired timers.
static queue_t exp_tmr_queue;                               // Queue for expired timers.
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- Local function prototypes -----
 */
static bool_t queue_push_item(queue_t * queue_ptr,  const void * item_ptr);
static bool_t queue_pop_item(queue_t * queue_ptr,  void * item_ptr);
static void queue_copy_item(const void * src_ptr, void * dest_ptr, port_uint_t item_size);

#if(AEDEA_OPT_USE_SOFT_TMR == 1)
static void timer_process(void * arg_ptr);
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- Function: aedea_init() -----
 */
void aedea_init(void)
{
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
     // Initialize the expired timers queue
     exp_tmr_queue.buff_ptr = exp_tmrs;
     exp_tmr_queue.num_items = AEDEA_OPT_MAX_SOFT_TMRS;
     exp_tmr_queue.item_size = sizeof(sw_tmr_t);
     exp_tmr_queue.count = 0;
     exp_tmr_queue.head = 0;
     exp_tmr_queue.tail = 0;
     
     // Add the timer process.
     aedea_add_process(timer_process, NULL, PID_AEDEA_TIMER_PROCESS, NULL, 0, 0);
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */
}


/*
 * ----- Function: aedea_start() -----
 */
void aedea_start(void)
{
     port_uint_t n = 0;

     // Call all processes one by one.
     while(TRUE)
     {
          // If no processes have been added, continue.
          if(0 == num_processes)
          {
               continue;
          }
          
          // Store pointer to the active process manager.
          active_proc_mgr = &(proc_mgrs[n]);
          
          // Invoke the process if the iteration count is zero.
          if(0 == active_proc_mgr->iterations_to_exec)
          {
               // Reset the iteration count.
               active_proc_mgr->iterations_to_exec = active_proc_mgr->exec_delay;
          
               // Call process.
               active_proc_mgr->callback(active_proc_mgr->process_arg_ptr);
          }
          else
          {
               // Decrement the execution count if the process is not disabled.
               if(AEDEA_PROCESS_DISABLED != active_proc_mgr->exec_delay)
               {
                   active_proc_mgr->iterations_to_exec--;
               }
          }
          
          // Increment loop counter.
          n = (n + 1) % num_processes;
     }
}


/*
 * ----- Function: aedea_add_process() -----
 */
bool_t aedea_add_process(
                         process_callback_t * process_callback,
                         void * process_arg_ptr,
                         uint8_t pid,
                         void * evt_queue_ptr,
                         port_uint_t evt_queue_size,
                         port_uint_t evt_item_size
                        )
{
     // Return FALSE if the proc_mgrs array is full.
     if(AEDEA_OPT_MAX_PROCESSES == num_processes)
     {
          return FALSE;
     }

     AEDEA_ENTER_CRITICAL_SECTION();
     
     // Initialize the process manager.
     proc_mgrs[num_processes].callback = process_callback;
     proc_mgrs[num_processes].process_arg_ptr = process_arg_ptr;
     proc_mgrs[num_processes].pid = pid;
     proc_mgrs[num_processes].exec_delay = 0;
     proc_mgrs[num_processes].iterations_to_exec = 0;
     proc_mgrs[num_processes].event_queue.buff_ptr = evt_queue_ptr;
     proc_mgrs[num_processes].event_queue.num_items = evt_queue_size;
     proc_mgrs[num_processes].event_queue.item_size = evt_item_size;
     proc_mgrs[num_processes].event_queue.count = 0;
     proc_mgrs[num_processes].event_queue.head = 0;
     proc_mgrs[num_processes].event_queue.tail = 0;
     
     // Increment the number of added processes.
     num_processes++;

     AEDEA_EXIT_CRITICAL_SECTION();

     return TRUE;
}


/*
 * ----- Function: aedea_set_exec_delay() -----
 */
bool_t aedea_set_exec_delay(uint8_t pid, port_int_t exec_delay)
{
     proc_mgr_t * proc_mgr_ptr;    // Used to store the pointer to the process manager for the process with the specified process ID.
     port_uint_t n = 0;

     // Search for the process with the specified process ID.
     proc_mgr_ptr = NULL;
     for(n = 0; n < AEDEA_OPT_MAX_PROCESSES; n++)
     {
          if(proc_mgrs[n].pid == pid)
          {
               proc_mgr_ptr = &(proc_mgrs[n]);
               break;
          }
     }
     
     // Return FALSE if a process with the specified ID was not found.
     if(NULL == proc_mgr_ptr)
     {
          return FALSE;
     }
     
     proc_mgr_ptr->exec_delay = exec_delay;
     proc_mgr_ptr->iterations_to_exec = active_proc_mgr->exec_delay;
     
     return TRUE;
}


/*
 * ----- Function: timer_process() -----
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
static void timer_process(void * arg_ptr)
{
     sw_tmr_t expired_tmr;    // Stores an expired software timer popped from the expired timers queue.
     
     // This is done only to avoid any compiler warnings related to unused variables/arguments.
     (void)arg_ptr;
     
     // Pop expired timers from the expired timers queue and call the timeout
     // handlers one by one.
     while(TRUE == queue_pop_item(&exp_tmr_queue, &expired_tmr))
     {
          // Call the expired_tmr timer's timeout handler.
          expired_tmr.handler(expired_tmr.timer_id, expired_tmr.handler_arg_ptr);
     }
}
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- Function: aedea_timer_tick() -----
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
void aedea_timer_tick(void)
{
     port_uint_t n = 0;

     // If all timers have expired, return
     if(AEDEA_OPT_MAX_SOFT_TMRS == active_tmrs_index)
     {
          return;
     }

     // Decrement the top timer's number of ticks
     sw_tmrs[active_tmrs_index].num_ticks--;

     // Check to see if decrementing the top timer's number of ticks above caused
     // it to expire. If that was the case, add this timer and any below it with
     // a timeout value of zero to the expired timers list and increment the
     // active_tmrs_index variable.
     for(n = active_tmrs_index; n < num_timers; n++)
     {
          // If the number of ticks are zero, add the timer to the expired
          // timers list and increment active_tmrs_index.
          if(0 == sw_tmrs[n].num_ticks)
          {
               active_tmrs_index++;
               queue_push_item(&exp_tmr_queue, &sw_tmrs[n]);
          }
          // The timer hasn't expired, break out of the loop.
          else
          {
               break;
          }
     }
}
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- Function: aedea_add_periodic_process() -----
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
bool_t aedea_install_timeout_handler(
                                     timeout_handler_t * handler,
                                     void * handler_arg_ptr,
                                     uint8_t timer_id,
                                     port_uint_t num_ticks
                                    )
{
     port_int_t n = 0;
     port_uint_t sum_ticks = 0;
     port_uint_t insert_index = 0;
     
     // Return FALSE if the sw_tmrs array is full.
     if(AEDEA_OPT_MAX_SOFT_TMRS == num_timers)
     {
          return FALSE;
     }
     
     AEDEA_ENTER_CRITICAL_SECTION();

     // If there are no timeout handlers installed yet, just add the timeout handler
     // to the top of the sw_tmrs array.
     if(0 == num_timers)
     {
          insert_index = 0;
     }
     // There are active timers already present, search the sw_tmrs array
     // for a suitable location for the new timer.
     else
     {
          // A cumulative sum of the number of ticks for each active timer is calculated
          // in sum_ticks at each iteration (n) of the following loop, which is then compared
          // with the timeout value for the new timer. If, at any point, the timeout value
          // for the new timer is less than the cumulatvie sum sum_ticks, the new timer is
          // inserted at n - 1. If at the end of the loop, the cumulative sum was not greater
          // than the timeout value for the new timer, the new timer is inserted at n + 1.
          for(n = active_tmrs_index; n < (port_int_t)num_timers; n++)
          {
               sum_ticks += sw_tmrs[n].num_ticks;

               if(sum_ticks > num_ticks)
               {
                    insert_index = n;
                    break;
               }
               else if(n == (port_int_t)(num_timers - 1))
               {
                    insert_index = n + 1;
                    break;
               }
          }
     }
     
     // If required, shift all timers downwards one index. 
     for(n = (num_timers - 1); n >= (port_int_t)insert_index; n--)
     {
          sw_tmrs[n + 1].handler = sw_tmrs[n].handler;
          sw_tmrs[n + 1].handler_arg_ptr = sw_tmrs[n].handler_arg_ptr;
          sw_tmrs[n + 1].timer_id = sw_tmrs[n].timer_id;
          sw_tmrs[n + 1].num_ticks = sw_tmrs[n].num_ticks;
     }

     // Insert the new timer.
     sw_tmrs[insert_index].handler = handler;
     sw_tmrs[insert_index].handler_arg_ptr = handler_arg_ptr;
     sw_tmrs[insert_index].timer_id = timer_id;
     
     // Increment the number of installed timers.
     num_timers++;
     
     // Adjust the timeout value for the new timer by calculating the cumulative
     // sum of all timers before this one and subtracting it from the specified
     // timeout value for it.
     sum_ticks = 0;
     for(n = active_tmrs_index; n < (char)insert_index; n++)
     {
          sum_ticks += sw_tmrs[n].num_ticks;
     }
     
     sw_tmrs[insert_index].num_ticks = num_ticks - sum_ticks;

     // Adjust the timeout value for the timer present immediatetely below the
     // new timer (if any).
     if((insert_index + 1) < num_timers)
     {
          sw_tmrs[insert_index + 1].num_ticks -= sw_tmrs[insert_index].num_ticks;
     }
     
     // Reset active_tmrs_index if insert_index is zero. This handles a corner case
     // when all timers have expired.
     if(insert_index == 0)
     {
          active_tmrs_index = 0;
     }
     
     AEDEA_EXIT_CRITICAL_SECTION();

     return TRUE;
}
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- Function; aedea_refresh_timer() -----
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
bool_t aedea_refresh_timer(uint8_t timer_id, port_uint_t num_ticks)
{
     port_uint_t n = 0;
     timeout_handler_t * handler;       // Used to store the pointer to timeout handler function of the specified timer.
     void * handler_arg_ptr;            // Used to store the argument pointer of the specified timer.
     
     // Return FALSE if no timers have been installed.
     if(0 == num_timers)
     {
          return FALSE;
     }

     // Search for the timer with the specified timer ID.
     for(n = 0; n < AEDEA_OPT_MAX_SOFT_TMRS; n++)
     {
          if(timer_id == sw_tmrs[n].timer_id)
          {
               break;
          }
     }
     
     // Return FALSE if no timer was found.
     if(n == AEDEA_OPT_MAX_SOFT_TMRS)
     {
          return FALSE;
     }
     
     // Make a temporary copy of the timer's pointers to the timeout handler function
     // and argument.
     handler = sw_tmrs[n].handler;
     handler_arg_ptr = sw_tmrs[n].handler_arg_ptr;
     
     // Delete the timer.
     aedea_delete_timer(timer_id);

     // Re-install the timer with the new timeout value.
     return aedea_install_timeout_handler(handler, handler_arg_ptr, timer_id, num_ticks);
}
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- Function; aedea_delete_timer() -----
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
bool_t aedea_delete_timer(uint8_t timer_id)
{
     port_uint_t n = 0, i = 0;
     
     // Return FALSE if no timers have been installed.
     if(0 == num_timers)
     {
          return FALSE;
     }

     // Search for the timer with the specified timer ID.
     for(n = 0; n < AEDEA_OPT_MAX_SOFT_TMRS; n++)
     {
          if(timer_id == sw_tmrs[n].timer_id)
          {
               break;
          }
     }
     
     // Return FALSE if no timer was found.
     if(AEDEA_OPT_MAX_SOFT_TMRS == n)
     {
          return FALSE;
     }

     AEDEA_ENTER_CRITICAL_SECTION();

     // If the first timer is to be deleted and there is only timer installed, just
     // decrement the total number of timers and return. This is also applies when
     // the timer to be deleted is the last timer. In any other case, other timers'
     // indices have to be adjusted to delete the specified timer.
     if(((0 == n) && (1 == num_timers)) || ((AEDEA_OPT_MAX_SOFT_TMRS - 1) == n))
     {
          // Decrement the number of timers.
          num_timers--;
     }
     else
     {
          // Adjust the number of ticks for the timer immediately below the specified timer
          // by adding the number of ticks of the timer being deleted.
          sw_tmrs[n + 1].num_ticks += sw_tmrs[n].num_ticks;
          
          // Decrement the number of timers.
          num_timers--;
          
          // Shift all timers below the specified timer one index upwards, effectively
          // deleting the timer.
          for(i = n; i < num_timers; i++)
          {
               sw_tmrs[i].handler = sw_tmrs[i + 1].handler;
               sw_tmrs[i].handler_arg_ptr = sw_tmrs[i + 1].handler_arg_ptr;
               sw_tmrs[i].timer_id = sw_tmrs[i + 1].timer_id;
               sw_tmrs[i].num_ticks = sw_tmrs[i + 1].num_ticks;
          }
     }
     
     // If the deleted timer was expired, the active_tmrs_index index needs to
     // be adjusted.
     if(n < active_tmrs_index)
     {
          active_tmrs_index--;
     }
     
     AEDEA_EXIT_CRITICAL_SECTION();

     return TRUE;
}
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*
 * ----- Function: aedea_post_event() -----
 */
bool_t aedea_post_event(port_uint_t pid, void * evt_item_ptr)
{
     proc_mgr_t * proc_mgr_ptr;    // Used to store the pointer to the process manager for the process with the specified process ID.
     port_uint_t n = 0;
     
     // Search for the process with the specified process ID.
     proc_mgr_ptr = NULL;
     for(n = 0; n < AEDEA_OPT_MAX_PROCESSES; n++)
     {
          if(proc_mgrs[n].pid == pid)
          {
               proc_mgr_ptr = &(proc_mgrs[n]);
               break;
          }
     }
     
     // Return FALSE if a process with the specified ID was not found.
     if(NULL == proc_mgr_ptr)
     {
          return FALSE;
     }

     // Push the new event item on to the event queue.
     return queue_push_item(&(proc_mgr_ptr->event_queue), evt_item_ptr);
}


/*
 * ----- Function: aedea_get_event() -----
 */
bool_t aedea_get_event(void * evt_item_ptr)
{
     // Pop an event item on from the event queue.
     return queue_pop_item(&(active_proc_mgr->event_queue), evt_item_ptr);
}


/*
 * ----- Function: aedea_critical_nesting() -----
 */
void aedea_critical_nesting(uint8_t mode)
{
     static port_uint_t nesting_level = 0;        // Stores the critical section nesting level.
     
     // Critical section start.
     if(AEDEA_CRITICAL_SECTION_START == mode)
     {
          // Lock interrupts and increment the nesting level.
          PORT_LOCK_INTERRUPTS();
          nesting_level++;
     }
     // Critical section end.
     else if(AEDEA_CRITICAL_SECTION_END == mode)
     {
          // Decrement nesting level
          nesting_level--;
          
          // If nesting level is zero, unlock interrupts.
          if(0 == nesting_level)
          {
               PORT_UNLOCK_INTERRUPTS();
          }
     }
}


 /*
 * ----- Function: queue_push_item() -----
 */
static bool_t queue_push_item(queue_t * queue_ptr, const void * item_ptr)
{
     void * empty_slot_ptr;        // Used to store the empty slot pointer.

     // If there is no space availabe in the event queue, return FALSE.
     if(queue_ptr->count == queue_ptr->num_items)
     {
          return FALSE;
     }
     
     AEDEA_ENTER_CRITICAL_SECTION();

     // Store the pointer to the empty item slot.
     empty_slot_ptr = (port_uint_t *)((queue_ptr->head * queue_ptr->item_size) + (uint8_t *)queue_ptr->buff_ptr);
     
     // Copy the item on to the queue.
     queue_copy_item(item_ptr, empty_slot_ptr, queue_ptr->item_size);

     // Increment the head pointer.
     queue_ptr->head = (queue_ptr->head + 1) % queue_ptr->num_items;

     // Increment the item count.
     queue_ptr->count++;
     
     AEDEA_EXIT_CRITICAL_SECTION();
     
     return TRUE;
}


/*
 * ----- Function: queue_pop_item() -----
 */
static bool_t queue_pop_item(queue_t * queue_ptr,  void * item_ptr)
{
     void * pop_item_ptr;     // Pointer to the item to be popped off the queue.

     // If no un-popped item is present, return FALSE.
     if(0 == queue_ptr->count)
     {
          return FALSE;
     }
     
     AEDEA_ENTER_CRITICAL_SECTION();

     // Get a pointer to the item to be popped off the queue.
     pop_item_ptr = (port_uint_t *)((queue_ptr->tail * queue_ptr->item_size) + (uint8_t *)queue_ptr->buff_ptr);
     
     // Copy item from the queue.
     queue_copy_item(pop_item_ptr, item_ptr, queue_ptr->item_size);

     // Increment the tail pointer.
     queue_ptr->tail = (queue_ptr->tail + 1) % queue_ptr->num_items;

     // Decrement the item count.
     queue_ptr->count--;
     
     AEDEA_EXIT_CRITICAL_SECTION();

     return TRUE;
}


/*
 * ----- Function: queue_copy_item() -----
 */
static void queue_copy_item(const void * src_ptr, void * dest_ptr, port_uint_t item_size)
{
     port_uint_t i;
     
     // Copy event item.
     for(i = 0; i < item_size; i++)
     {
          *((uint8_t *)dest_ptr + i) = *((uint8_t *)src_ptr + i);
     }
}


/*----------------------------------------------------------------------------*/
/*! @} */
