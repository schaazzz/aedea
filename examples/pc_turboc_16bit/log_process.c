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
 * AEDEA PC TurboC/C++ example logger process C file.
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
 * ----- Function: logger_process() -----
 */
void logger_process(void * arg_ptr)
{
     static uint16_t y = 0;
     log_evt_t e;
     rect_t * log_rect_ptr;
     char id_str[15];
     
     // Initialize the pointer to log printing area rectangle.
     log_rect_ptr = (rect_t * )arg_ptr;
     
     // Get a log entry from the event queue, return FALSE if the queue is empty.
     if(FALSE == aedea_get_event(&e))
     {
          return;
     }
     
     // Setup ID string.
     if(e.log_type == LOG_PROCESS)
     {
          sprintf(id_str, "[PID %d] ", e.id);
     }
     else if(e.log_type == LOG_TIMER)
     {
          sprintf(id_str, "[TMR_ID %d] ", e.id);
     }
     
     // If y is greater than the log printing area rectangle's height, clear the rectangle
     // and reset y.
     if(y >= scr_rect_get_height(log_rect_ptr))
     {
          y = 0;
          scr_rect_clear(log_rect_ptr);
     }
     
     // Print the ID string.
     scr_rect_print_str(log_rect_ptr, 0, y, id_str);
     scr_rect_print_str(log_rect_ptr, strlen(id_str), y, e.entry_str);
     y++;
}


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
