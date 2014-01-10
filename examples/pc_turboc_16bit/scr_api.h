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
 * AEDEA PC TurboC/C++ example screen API header file.
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
 
 
#ifndef __SCREEN_API_H
#define __SCREEN_API_H


/*
 * Background color definitions.
 */
#define SCREEN_BG_BLACK       0x00
#define SCREEN_BG_BLUE        0x10
#define SCREEN_BG_GREEN       0x20
#define SCREEN_BG_CYAN        0x30
#define SCREEN_BG_RED         0x40
#define SCREEN_BG_PURPLE      0x50
#define SCREEN_BG_BROWN       0x60
#define SCREEN_BG_LIGHT_GRAY  0x70


/*
 * Foreground color definitions.
 */
#define SCREEN_FG_BLACK            0x00
#define SCREEN_FG_BLUE             0x01
#define SCREEN_FG_GREEN            0x02
#define SCREEN_FG_CYAN             0x03
#define SCREEN_FG_RED              0x04
#define SCREEN_FG_PURPLE           0x05
#define SCREEN_FG_BROWN            0x06
#define SCREEN_FG_LIGHT_GRAY       0x07
#define SCREEN_FG_DARK_GRAY        0x08
#define SCREEN_FG_LIGHT_BLUE       0x09
#define SCREEN_FG_LIGHT_GREEN      0x0A
#define SCREEN_FG_LIGHT_CYAN       0x0B
#define SCREEN_FG_LIGHT_RED        0x0C
#define SCREEN_FG_LIGHT_PURPLE     0x0D
#define SCREEN_FG_YELLOW           0x0E
#define SCREEN_FG_WHITE            0x0F


/*!
 * Rectangle structure used by the screen API.
 */
typedef struct
{
     uint16_t x;              //!< Rectangle's x coordinate.
     uint16_t y;              //!< Rectangle's y coordinate.
     uint16_t height;         //!< Rectangle's height.
     uint16_t width;          //!< Rectangle's width.
     uint8_t fg_clr;          //!< Rectangle's foreground color.
     uint8_t bg_clr;          //!< Rectangle's background color.
}
rect_t;


/*
 * Function prototypes.
 */
/*!
 * Initialize a rectangle and print it on the screen.
 *
 * \param rect_ptr Pointer to a rect_t variable to represent the rectangle.
 * \param x X coordinate of the rectangle.
 * \param y Y coordinate of the rectangle.
 * \param width Width of the rectangle (in units of characters).
 * \param height Height of the rectangle (in unints of characters).
 * \param fg_clr Foreground color of the rectangle.
 * \param bg_clr Background color of the rectangle.
 */
void scr_rect_init(
                   rect_t * rect_ptr,
                   uint16_t x,
                   uint16_t y,
                   uint16_t width,
                   uint16_t height,
                   uint8_t fg_clr,
                   uint8_t bg_clr
                  );


/*!
 * Clear a previously initialized and printed screen rectangle.
 *
 * \param rect_ptr Pointer to the rectangle to be cleared.
 */
void scr_rect_clear(rect_t * rect_ptr);


/*!
 * Print a string in a previously initialized and printed rectangle.
 *
 * \param rect_ptr Pointer to the rectangle in which the string is to be printed.
 * \param x X coordinate of the first character of the string (with respect to the dimensions of the rectangle).
 * \param y Y coordinate of the first character of the string (with respect to the dimensions of the rectangle).
 * \param str String to be printed.
 */
void scr_rect_print_str(rect_t * rect_ptr, uint16_t x, uint16_t y, const char * str);


/*!
 * Print a character in a previously initialized and printed rectangle.
 *
 * \param rect_ptr Pointer to the rectangle in which the character is to be printed.
 * \param x X coordinate of the character (with respect to the dimensions of the rectangle).
 * \param y Y coordinate of the character (with respect to the dimensions of the rectangle).
 * \param c Character to be printed.
 */
void scr_rect_print_char(rect_t * rect_ptr, uint16_t x, uint16_t y, char c);


/*!
 * Set foreground color.
 *
 * \param rect_ptr Pointer to the rectangle.
 * \param fg_clr Foreground color.
 */
void scr_rect_set_fg_clr(rect_t * rect_ptr, uint8_t fg_clr);


/*!
 * Set background color.
 *
 * \param rect_ptr Pointer to the rectangle.
 * \param bg_clr Background color.
 */
void scr_rect_set_bg_clr(rect_t * rect_ptr, uint8_t bg_clr);


/*!
 * Get a rectangle's height.
 *
 * \param rect_ptr Pointer to the rectangle.
 */
uint16_t scr_rect_get_height(rect_t * rect_ptr);


/*!
 * Get a rectangle's width.
 *
 * \param rect_ptr Pointer to the rectangle.
 */
uint16_t scr_rect_get_width(rect_t * rect_ptr);


#endif    /* __SCREEN_API_H */


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
