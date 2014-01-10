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
 * AEDEA PC TurboC/C++ example screen API C file.
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


/*
 * ----- Function: scr_rect_init() -----
 */
void scr_rect_init(
                   rect_t * rect_ptr,
                   uint16_t x,
                   uint16_t y,
                   uint16_t width,
                   uint16_t height,
                   uint8_t fg_clr,
                   uint8_t bg_clr
                  )
{
     // Initialize the rectangle.
     rect_ptr->x = x;
     rect_ptr->y = y;
     rect_ptr->height = height;
     rect_ptr->width = width;
     rect_ptr->fg_clr = fg_clr;
     rect_ptr->bg_clr = bg_clr;
     
     // Print and clear the rectangle.
     scr_rect_clear(rect_ptr);
}


/*
 * ----- Function: scr_rect_clear() -----
 */
void scr_rect_clear(rect_t * rect_ptr)
{
     uint8_t x, y;

     // Print a clear rectangle on the screen by filling spaces in the video RAM.
     y = rect_ptr->y;
     while(y < (rect_ptr->height + rect_ptr->y))
     {
          // Fill a single line with spaces.
          for(x = rect_ptr->x; x < (rect_ptr->width + rect_ptr->x); x++)
          {
               scr_rect_print_char(rect_ptr, (x - rect_ptr->x), (y - rect_ptr->y),' ');
          }
          
          // Move to the next line.
          y++;
     }
}


/*
 * ----- Function: scr_rect_print_char() -----
 */
void scr_rect_print_str(rect_t * rect_ptr, uint16_t x, uint16_t y, const char * str)
{
     uint16_t n = 0;
     
     // Print the string.
     while(((n + x) < rect_ptr->width) && (*(str + n) != '\0'))
     {
          scr_rect_print_char(rect_ptr, n + x, y, *(str + n));
          n++;
     }
}


/*
 * ----- Function: scr_rect_print_char() -----
 */
void scr_rect_print_char(rect_t * rect_ptr, uint16_t x, uint16_t y, char c)
{
     uint8_t far * vram_ptr;

     // Return without doing anything if y is greater than the height of the
     // rectangle or x is greater than the width of the rectangle.
     if((x >= rect_ptr->width) || (y >= rect_ptr->height))
     {
          return;
     }

     // Set the video RAM pointer.
     vram_ptr = (uint8_t far *)MK_FP(0xB800, ((((rect_ptr->y + y) * 80) + (rect_ptr->x + x)) * 2));
     
     // Print the character.
     *(vram_ptr + 0) = c;
     
     // Set foreground and background color attributes.
     *(vram_ptr + 1) = rect_ptr->bg_clr | rect_ptr->fg_clr;
}


/*
 * ----- Function: scr_rect_set_fg_clr()
 */
void scr_rect_set_fg_clr(rect_t * rect_ptr, uint8_t fg_clr)
{
     rect_ptr->fg_clr = fg_clr;
}


/*
 * ----- Function: scr_rect_set_bg_clr()
 */
void scr_rect_set_bg_clr(rect_t * rect_ptr, uint8_t bg_clr)
{
     rect_ptr->bg_clr = bg_clr;
}


/*
 * ----- Function: scr_rect_get_height() -----
 */
uint16_t scr_rect_get_height(rect_t * rect_ptr)
{
     return rect_ptr->height;
}


/*
 * ----- Function: scr_rect_get_width() -----
 */
uint16_t scr_rect_get_width(rect_t * rect_ptr)
{
     return rect_ptr->width;
}


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
