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
 * AEDEA PC TurboC/C++ example keyboard process C file.
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


/*!
 * Key character structure.
 */
typedef struct
{
     char c;                  //!< Character to be printed without the shift key pressed.
     char c_shift;            //!< Character to be printed with the shift key pressed.
}
key_char_t;


/*
 * Special key codes.
 */
#define KEY_ESCAPE  0x01
#define KEY_SHIFT   0x2A
#define KEY_ENTER   0x1C


/*
 * Printable key code lookup table. "-1" is used to indicate a non-printable character.
 */
static const key_char_t keycode_table[] =
{
     {-1, -1}, {-1, -1},
     {'1', '!'}, {'2', '@'}, {'3', '#'}, {'4', '$'}, {'5', '%'}, {'6', '^'}, {'7', '&'},
     {'8', '*'}, {'9', '('}, {'0', ')'}, {'-', '_'}, {'=', '+'},
     {-1, -1}, {-1, -1},
     {'q', 'Q'}, {'w', 'W'}, {'e', 'E'}, {'r', 'R'}, {'t', 'T'}, {'y', 'Y'}, {'u', 'U'},
     {'i', 'I'}, {'o', 'O'}, {'p', 'P'}, {'[', '{'}, {']', '}'},
     {-1, -1}, {-1, -1},
     {'a', 'A'}, {'s', 'S'}, {'d', 'D'}, {'f', 'F'}, {'g', 'G'}, {'h', 'H'}, {'j', 'J'},
     {'k', 'K'}, {'l', 'L'}, {';', ':'}, {'\'', '"'},
     {-1, -1}, {-1, -1},
     {'\\', '|'}, {'z', 'Z'}, {'x', 'X'}, {'c', 'C'}, {'v', 'V'}, {'b', 'B'}, {'n', 'N'},
     {'m', 'M'}, {',', '<'}, {'.', '>'}, {'/', '?'},
     {-1, -1}, {-1, -1}, {-1, -1},
     {' ', ' '}
};


/*
 * ----- Function: keyboard_process() -----
 */
void keyboard_process(void * arg_ptr)
{
     static bool_t shift_on = FALSE;
     static uint16_t x = 0, y = 0;
     char char_to_print;
     kbd_evt_t e;
     log_evt_t log_entry;
     rect_t * kbd_rect_ptr;
     
     // Initialize the pointer to keyboard's print area rectangle.
     kbd_rect_ptr = (rect_t * )arg_ptr;
     
     // Get a key from the event queue, return FALSE if the queue is empty.
     if(FALSE == aedea_get_event(&e))
     {
          return;
     }

     switch(e.event)
     {
          case EVT_KEY_PRESSED:
               // If the key to be pressed is the escape key, exit the program.
               if(e.key == KEY_ESCAPE)
               {
                    exit_pgm();
               }
               // If the enter/return key is pressed, clear the keyboard's print area.
               if(e.key == KEY_ENTER)
               {
                    x = 0;
                    y = 0;
                    scr_rect_clear(kbd_rect_ptr);
               }
               // If the shift key is pressed, set the shift_on flag to TRUE.
               else if(e.key == KEY_SHIFT)
               {
                    shift_on = TRUE;
               }
               // Print the key character if the key falls within the character lookup
               // table (keycode_table) and the key isn't a non-printable character.
               else if((e.key < (sizeof(keycode_table) / sizeof(key_char_t))) &&
                       (keycode_table[e.key].c != -1) &&
                       (keycode_table[e.key].c_shift != -1))
               {
                    if(shift_on)
                    {
                         char_to_print = keycode_table[e.key].c_shift;
                    }
                    else
                    {
                         char_to_print = keycode_table[e.key].c;
                    }
                    
                    // Print the character in the keyboard's print area.
                    scr_rect_print_char(kbd_rect_ptr, x, y, char_to_print);
                    
                    // Adjust the character's x and y coordinates.
                    x++;
                    if(x == scr_rect_get_width(kbd_rect_ptr))
                    {
                         // Reset x.
                         x = 0;
                         
                         // Increment y if it is less than the keyboard's print area rectangle's
                         // height.
                         if(y <= scr_rect_get_height(kbd_rect_ptr))
                         {
                              y++;
                         }
                    }
               }
               // Unsupported key, log event.
               else
               {
                    // Setup the log entry.
                    log_entry.log_type = LOG_PROCESS;
                    log_entry.id = PID_KEYBOARD_PROCESS;
                    sprintf(log_entry.entry_str, "%s 0x%02X", "Unsupported key:", e.key);

                    // Post log to the logger process.
                    aedea_post_event(PID_LOGGER_PROCESS, &log_entry);
               }
               break;
               
          case EVT_KEY_RELEASED:
               // If the shift key is released, set the shift_on flag to FALSE.
               if(e.key == KEY_SHIFT)
               {
                    shift_on = FALSE;
               }
               break;
               
          default:
               break;
     }
}


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
