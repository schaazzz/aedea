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
 * AEDEA PC TurboC/C++ mouse process C file.
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
#include <dos.h>


/*
 * ----- Function: mouse_process() -----
 */
void mouse_process(void * arg_ptr)
{
     static bool_t mouse_prev_clicked = FALSE;
     union REGS in_regs, out_regs;
     log_evt_t log_entry;

     // This is done only to avoid any compiler warnings related to unused variables/arguments.
     (void)arg_ptr;
     
     // Issue the mouse software interrupt with service number 3 for getting the mouse
     // position and button status.
     in_regs.x.ax = 0x03;
     int86(51, &in_regs, &out_regs);

     // If the mouse was previously clicked, wait for the button to be released
     // before logging the next click. This is done to avoid multiple reports for
     // a single click.     
     if((out_regs.x.bx == 0) && mouse_prev_clicked)
     {
          // Reset mouse_prev_clicked to FALSE.
          mouse_prev_clicked = FALSE;
     }
     // Report the mouse click.
     else if((out_regs.x.bx != 0) && !mouse_prev_clicked)
     {
          // Log the mouse click;
          log_entry.log_type = LOG_PROCESS;
          log_entry.id = PID_MOUSE_PROCESS;
          sprintf(log_entry.entry_str, "Mouse click @ {%d, %d}", out_regs.x.cx, out_regs.x.dx);

          // Post log to the logger process.
          aedea_post_event(PID_LOGGER_PROCESS, &log_entry);
          
          // Set mouse_prev_clicked to TRUE to indicate that the mouse was clicked.
          mouse_prev_clicked = TRUE;
     }     
}


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
