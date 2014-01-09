/*!
 * \addtogroup aedea
 * @{
 */


/*!
 * \file
 * AEDEA main header file.
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


#ifndef __AEDEA_H
#define __AEDEA_H


/*!
 * Constant used to disable a process.
 *
 * \hideinitializer
 */
#define AEDEA_PROCESS_DISABLED     -1


/*!
 * Constant passed to aedea_critical_nesting() to start a critical section.
 *
 * \hideinitializer
 */
#define AEDEA_CRITICAL_SECTION_START    0


/*!
 * Constant passed to aedea_critical_nesting() to end a critical section.
 *
 * \hideinitializer
 */
#define AEDEA_CRITICAL_SECTION_END      1


/*!
 * Critical section entry macro.
 *
 * \hideinitializer
 */
#define AEDEA_ENTER_CRITICAL_SECTION()  aedea_critical_nesting(AEDEA_CRITICAL_SECTION_START)


/*!
 * Critical section exit macro.
 *
 * \hideinitializer
 */
#define AEDEA_EXIT_CRITICAL_SECTION()   aedea_critical_nesting(AEDEA_CRITICAL_SECTION_END)


/*!
 * ISR entry macro.
 *
 * \hideinitializer
 * \note Only required if AEDEA API calls are used from within an ISR.
 */
#define AEDEA_ENTER_ISR()     AEDEA_ENTER_CRITICAL_SECTION()


/*!
 * ISR exit macro.
 *
 * \hideinitializer
 * \note Only required if AEDEA API cals are used from within an ISR and AEDEA_ENTER_ISR was
 * used on entry to the ISR.
 */
#define AEDEA_EXIT_ISR()      AEDEA_EXIT_CRITICAL_SECTION()


/*
 * Process callback function type definition.
 */
typedef void process_callback_t(void *);          //!< Defines the prototype a process callback should follow.


/*
 * Timeout handler function type definition.
 */
typedef void timeout_handler_t(uint8_t, void *);  //!< Defines the prototype a timeout handler should follow.


/*
 * AEDEA API prototypes.
 */
/*!
 * Initialize AEDEA. 
 */
void aedea_init(void);
 
 
/*!
 * Start AEDEA process manager.
 *
 * \return This function never returns.
 */
void aedea_start(void);


/*!
 * Add a new process.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param process_callback Pointer to the process callback function.
 * \param process_arg_ptr Pointer to the argument to be passed to the process.
 * \param pid Integer value used to identify the process.
 * \param evt_queue_ptr Pointer to the process' event queue.
 * \param evt_queue_size Size of the event queue (the number of events that can be stored in this queue).
 * \param evt_item_size Size of a single event item.
 *
 * \return TRUE if the process was successfully added, FALSE otherwise.
 */
bool_t aedea_add_process(
                         process_callback_t * process_callback,
                         void * process_arg_ptr,
                         uint8_t pid,
                         void * evt_queue_ptr,
                         port_uint_t evt_queue_size,
                         port_uint_t evt_item_size
                        );


/*!
 * Set the number of AEDEA process manager iterations after which the process is invoked. This function can be
 * used to slow down or speed up the process. By default the execution delay is set to zero, this means that the
 * process will be invoked on each iteration of the process manager. Increasing this value will cause the specified
 * number of iterations to pass before each call to the process callback, in effect, slowing down the process.
 *
 * Setting exec_delay to AEDEA_PROCESS_DISABLED will disable the process. To enable a disabled process pass any
 * value greater than or equal to zero.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param pid Integer value used to identify the process.
 * \param exec_delay Specifies the number of iterations of the AEDEA process manager after which the process callback is called.
 *
 * \return TRUE if the delay was successfully set, FALSE otherwise.
 */
 bool_t aedea_set_exec_delay(uint8_t pid, port_int_t exec_delay);


/*!
 * Post an event to a process.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param pid Process ID.
 * \param evt_item_ptr Pointer to the event item.
 *
 * \return TRUE if the event was successfully posted, FALSE otherwise.
 */
bool_t aedea_post_event(port_uint_t pid, void * evt_item_ptr);


/*!
 * Used by a process to get a single event from its event queue.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param evt_item_ptr Void pointer to an event item, used to return the event.
 *
 * \return FALSE if no event was present, TRUE otherwise.
 */
bool_t aedea_get_event(void * evt_item_ptr);


/*!
 * Install a timeout handler.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param handler Timeout handler function.
 * \param handler_arg_ptr Pointer to the argument to be passed to the timeout handler.
 * \param timer_id Integer value used to identify this timer.
 * \param num_ticks Number of ticks after which to timeout.
 *
 * \return TRUE if the timeuot handler was successfully installed, FALSE otherwise.
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
bool_t aedea_install_timeout_handler(
                                     timeout_handler_t * handler,
                                     void * handler_arg_ptr,
                                     uint8_t timer_id,
                                     port_uint_t num_ticks
                                    );
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*!
 * Refresh the timeout interval for a previously installed timeout handler.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param timer_id Timer ID of the timer to refresh.
 * \param num_ticks Number of ticks after which to timeout.
 *
 * \return TRUE if the timer was successfully refreshed, FALSE otherwise.
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
bool_t aedea_refresh_timer(uint8_t timer_id, port_uint_t num_ticks);
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*!
 * Delete a previously installed timeout handler.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param timer_id Timer ID of the timer to be deleted.
 *
 * \return TRUE if the timer was successfully deleted, FALSE otherwise.
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
bool_t aedea_delete_timer(uint8_t timer_id);
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*!
 * Timer tick handler, to be called from the system's timer ISR on each tick.
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
void aedea_timer_tick(void);
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*!
 * Handles nested critical sections. This function should not be called directly, instead
 * the AEDEA_ENTER_CRITICAL_SECTION() and AEDEA_EXIT_CRITICAL_SECTION() macros should be
 * used.
 *
 * Usage:
 * \code
 * \endcode
 *
 * \param mode AEDEA_CRITICAL_SECTION_START or AEDEA_CRITICAL_SECTION_END.
 */
void aedea_critical_nesting(uint8_t mode);


#endif    /* __AEDEA_H */


/*----------------------------------------------------------------------------*/
/*! @} */
