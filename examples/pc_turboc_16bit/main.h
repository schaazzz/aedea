/*!
 * \addtogroup aedea_examples
 * @{
 */


/*!
 * \defgroup PC_TurboC PC TurboC/C++
 * @{
 *
 * An AEDEA example for the PC compiled using TurboC/C++ v3.0.
 */


/*!
 * \file
 * AEDEA PC TurboC/C++ example main header file.
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
 
 
#ifndef __MAIN_H
#define __MAIN_H


/*
 * Keyboard process ID.
 *
 * \hideinitializer
 */
#define PID_KEYBOARD_PROCESS  0x01


/*
 * Logger process ID.
 *
 * \hideinitializer
 */
#define PID_LOGGER_PROCESS  0x02


/*
 * Mouse process ID.
 *
 * \hideinitializer
 */
#define PID_MOUSE_PROCESS  0x03


/*!
 * Animation timer ID.
 *
 * \hideinitializer
 */
#define TMR_ID_ANIMATION_TIMER 0x00


/*!
 * Print timer ID.
 *
 * \hideinitializer
 */
#define TMR_ID_PRINT_TIMER 0x01


/*!
 * Keyboard key event buffer size.
 *
 * \hideinitializer
 */
#define KEYBOARD_EVENT_BUFF_SZ     25


/*!
 * Logger event buffer size.
 *
 * \hideinitializer
 */
#define LOGGER_EVENT_BUFF_SZ       10


/*!
 * Logger entry string maximum size.
 *
 * \hideinitializer
 */
#define LOGGER_ENTRY_STR_MAZ_SZ    60


/*!
 * Keyboard key pressed event.
 *
 * \hideinitializer
 */
#define EVT_KEY_PRESSED  0x00


/*!
 * Keyboard key released event.
 *
 * \hideinitializer
 */
#define EVT_KEY_RELEASED 0x01


/*!
 * Process log entry type.
 *
 * \hideinitializer
 */
#define LOG_PROCESS 0x00


/*!
 * Timer log entry type.
 *
 * \hideinitializer
 */
#define LOG_TIMER 0x01


/*!
 * Print timer interval.
 *
 * \hideinitializer
 */
#define TMR_INTERVAL_PRINT    500


/*!
 * Animation timer interval.
 *
 * \hideinitializer
 */
#define TMR_INTERVAL_ANIMATION     20


/*!
 * Keyboard event structure.
 */
typedef struct
{
     uint8_t event;           //!< Keyboard key event type.
     uint8_t key;             //!< Keyboard key.
}
kbd_evt_t;


/*!
 * Logger event structure.
 */
typedef struct
{
     uint8_t log_type;                            //!< Log type (LOG_TIMER or LOG_PROCESS).
     uint8_t id;                                  //!< ID of the timer of process which generated the log entry.
     char entry_str[LOGGER_ENTRY_STR_MAZ_SZ];     //!< Log entry string.
}
log_evt_t;


/*
 * Function prototypes.
 */
/*!
 * Exit the program.
 */
void exit_pgm(void);


/*!
 * Keyboard process.
 *
 * \param arg_ptr Pointer to the keyboard's print area rectangle.
 */
void keyboard_process(void * arg_ptr);


/*!
 * Logger process.
 *
 * \param arg_ptr Pointer to the log printing area rectangle.
 */
void logger_process(void * arg_ptr);


/*!
 * Mouse process.
 *
 * \param arg_ptr Set to NULL, this process does not have any arguments.
 */
void mouse_process(void * arg_ptr);


/*!
 * Print timer timeout handler. Print a log entry on each timeout.
 *
 * \param timer_id Timer ID (TMR_ID_PRINT_TIMER). 
 * \param arg_ptr Set to NULL, this timer does not have any arguments.
 */
void print_timer(uint8_t timer_id, void * arg_ptr);


/*!
 * Animation timer timeout handler.
 *
 * \param timer_id Timer ID (TMR_ID_ANIMATION_TIMER). 
 * \param arg_ptr Pointer to the animation area rectangle.
 */
void animation_timer(uint8_t timer_id, void * arg_ptr);


#endif    /* __MAIN_H */


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
