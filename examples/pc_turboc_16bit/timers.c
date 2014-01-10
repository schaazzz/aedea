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
 * AEDEA PC TurboC/C++ example timers C file.
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
#include "main.h"
#include "scr_api.h"
#include <stdio.h>
#include <string.h>


/*
 * ----- Constants -----
 */
#define ANIMATION_CHAR   '*'


/*
 * ----- Function: print_timer() -----
 */
void print_timer(uint8_t timer_id, void * arg_ptr)
{
     log_evt_t log_entry;

     // This is done only to avoid any compiler warnings related to unused variables/arguments.
     (void)arg_ptr;
     
     // Refresh the timer.
     aedea_refresh_timer(timer_id, TMR_INTERVAL_PRINT);
     
     // Log timer tick.
     log_entry.log_type = LOG_TIMER;
     log_entry.id = timer_id;
     strcpy(log_entry.entry_str, "Print timer tick...");
     
     // Post log to the logger process.
     aedea_post_event(PID_LOGGER_PROCESS, &log_entry);
}


/*
 * ----- Function: animation_timer() -----
 */
void animation_timer(uint8_t timer_id, void * arg_ptr)
{
     static uint16_t x = 1, y = 0;
     static bool_t inc_x = TRUE, inc_y = TRUE;
     rect_t * ani_rect_ptr;
     
     // Initialize the pointer to animation area rectangle.
     ani_rect_ptr = (rect_t * )arg_ptr;
     
     // Refresh the timer.
     aedea_refresh_timer(timer_id, TMR_INTERVAL_ANIMATION);
     
     // Clear the animation area.
     scr_rect_clear(ani_rect_ptr);
     
     // Print * at the current values of x and y.
     scr_rect_print_char(ani_rect_ptr, x, y, ANIMATION_CHAR);
     
     // Calculate x.
     if(inc_x)
          if(++x == scr_rect_get_width(ani_rect_ptr)) 
               inc_x = FALSE;
     if(!inc_x)
          if(--x == 0)
               inc_x = TRUE;
     
     // Calculate y.
     if(inc_y)
          if(++y == (scr_rect_get_width(ani_rect_ptr) - 1))
               inc_y = FALSE;
     if(!inc_y)
          if(--y == 0)
               inc_y = TRUE;
}


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
