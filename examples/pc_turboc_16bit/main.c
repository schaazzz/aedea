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
 * AEDEA PC TurboC/C++ example main C file.
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


/*!
 * Intel 8253/8254 clock frequency in hertz.
 */
#define TIMER_825X_FREQ  1193181


/*!
 * Timer vector number.
 */
#define TIMER_VECT_NUM   0x08


/*!
 * Keyboard vector number.
 */
#define KEYBOARD_VECT_NUM     0x09


/*!
 * Tick interval in milliseconds.
 *
 * \hideinitializer
 */
#define TICK_INTERVAL    10


/*
 * ----- Header files -----
 */
#include "platform.h"
#include "options.h"
#include "aedea.h"
#include "main.h"
#include "scr_api.h"
#include <dos.h>
#include <conio.h>
#include <graphics.h>


/*
 * ----- Local function prototypes -----
 */
static void timer_setup(uint16_t tick_interval, void interrupt (*timer_isr)(void));
static void keyboard_setup(void interrupt (*key_isr)(void));
static void screen_setup(void);
static void interrupt tick_handler(void);
static void interrupt key_handler(void);


/*
 * ----- Event buffers -----
 */
kbd_evt_t kbd_evt_buff[KEYBOARD_EVENT_BUFF_SZ];
log_evt_t log_evt_buff[LOGGER_EVENT_BUFF_SZ];


/*
 * ----- Interface variables -----
 */
rect_t bg_rect;          // Interface background rectangle.
rect_t kbd_rect;         // Keyboard's print area rectangle.
rect_t log_rect;         // Log printing area rectangle.
rect_t info_rect;        // Information area rectangle.
rect_t ani_rect;         // Animation area rectangle.


/*
 * ----- Default interrupt vectors -----
 */
static void interrupt (*default_timer_isr)();
static void interrupt (*default_keyboard_isr)();


/*
 * ----- Function: main() -----
 */
int16_t main(void)
{
     // Call AEDEA initialization function.
     aedea_init();

     // Setup the screen.
     screen_setup();

     // Setup timer for a 10 millisecond tick.
     timer_setup(TICK_INTERVAL, tick_handler);
     
     // Setup the keyboard interrupt handler.
     keyboard_setup(key_handler);
     
     // Add the keyboard process. The keyboard's print area rectangle is passed to the keyboard process
     // each time it is invoked as the default parameter.
     aedea_add_process(keyboard_process, &kbd_rect, PID_KEYBOARD_PROCESS, kbd_evt_buff, KEYBOARD_EVENT_BUFF_SZ, sizeof(kbd_evt_t));
     
     // Add the logger process. The log printing area rectangle is passed to the logger process
     // each time it is invoked as the default parameter.
     aedea_add_process(logger_process, &log_rect, PID_LOGGER_PROCESS, log_evt_buff, LOGGER_EVENT_BUFF_SZ, sizeof(log_evt_t));

     // Add the mouse process. The mouse process contains no default argument and no events, hence,
     // all related fields are set to NULL.
     aedea_add_process(mouse_process, NULL, PID_MOUSE_PROCESS, NULL, NULL, NULL);

     // Install the print timer timeout handler.
     aedea_install_timeout_handler(print_timer, NULL, TMR_ID_PRINT_TIMER, TMR_INTERVAL_PRINT);

     // Install the animation timer timeout handler.
     aedea_install_timeout_handler(animation_timer, &ani_rect, TMR_ID_ANIMATION_TIMER, TMR_INTERVAL_ANIMATION);     

     // Start the AEDEA process manager.
     aedea_start();

     return 0;
}


/*
 * ----- Function: exit_pgm() -----
 */
void exit_pgm(void)
{
     // Set the keyboard and timer interrupt vectors to their
     // default values.
     setvect(TIMER_VECT_NUM, default_timer_isr);
     setvect(KEYBOARD_VECT_NUM, default_keyboard_isr);
     
     // Clear the screen and exit the program.
     clrscr();
     exit(0);
}


/*
 * ----- Function: timer_setup() -----
 */
static void timer_setup(uint16_t tick_interval, void interrupt (*timer_isr)(void))
{
     uint16_t timer_count = 0;

     // Calculate timer count.
     timer_count = (uint16_t)((TIMER_825X_FREQ / 1000)* tick_interval);

     // Set mode 3 for timer 0.
     outportb(0x43, 0x36);

     // Write low byte of count for timer 0.
     outportb(0x40, (timer_count & 0xFF));

     // Write high byte of count for timer 0.
     outportb(0x40, ((timer_count >> 8) & 0xFF));

     // Save the default timer interrupt vector.
     default_timer_isr = getvect(TIMER_VECT_NUM);

     // Set the interrupt vector.
     setvect(TIMER_VECT_NUM, timer_isr);
}


/*
 * ----- Function: keyboard_setup() -----
 */
static void keyboard_setup(void interrupt (*key_isr)(void))
{
     // Save the default keyboard interrupt vector.
     default_keyboard_isr = getvect(KEYBOARD_VECT_NUM);

     // Set the interrupt vector.
     setvect(KEYBOARD_VECT_NUM, key_isr);
}


/*
 * ----- Function: screen_setup() -----
 */
static void screen_setup(void)
{
     // Clear the screen.
     clrscr();
     
     // No cursor.
     _setcursortype(_NOCURSOR);
     
     // Setup the interface background rectangle.
     scr_rect_init(&bg_rect, 0, 0, 80, 25, SCREEN_FG_LIGHT_RED, SCREEN_BG_BLACK);

     // Setup the keyboard's print area rectangle.
     scr_rect_init(&kbd_rect, 0, 4, 80, 11, SCREEN_FG_YELLOW, SCREEN_BG_BLUE);
     
     // Setup the log printing area rectangle.
     scr_rect_init(&log_rect, 0, 15, 70, 9, SCREEN_FG_LIGHT_GREEN, SCREEN_BG_BLACK);
     
     // Setup the animation area rectangle.
     scr_rect_init(&ani_rect, 70, 15, 10, 9, SCREEN_FG_LIGHT_GRAY, SCREEN_BG_RED);
     
     // Setup the information area rectangle.
     scr_rect_init(&info_rect, 0, 24, 80, 1, SCREEN_FG_BLACK, SCREEN_BG_LIGHT_GRAY);
     
     // Print title and copyright notice on the top of the background rectangle.
     scr_rect_print_str(&bg_rect, 25, 1, "AEDEA PC TurboC/C++ Example");
     scr_rect_print_str(&bg_rect, 25, 2, "Copyright (c) 2007, Shahzeb Ihsan");
     
     // Print key shortcuts in the information area.
     scr_rect_set_fg_clr(&info_rect, SCREEN_FG_RED);
     scr_rect_print_str(&info_rect, 1, 0, "ESC:");
     
     scr_rect_set_fg_clr(&info_rect, SCREEN_FG_BLACK);
     scr_rect_print_str(&info_rect, 6, 0, "Exit");
     
     scr_rect_set_fg_clr(&info_rect, SCREEN_FG_RED);
     scr_rect_print_str(&info_rect, 12, 0, "ENTER:");
     
     scr_rect_set_fg_clr(&info_rect, SCREEN_FG_BLACK);
     scr_rect_print_str(&info_rect, 19, 0, "Clear keystrokes");
}


/*
 * ----- Function: tick_handler() -----
 */
static void interrupt tick_handler(void)
{
     AEDEA_ENTER_ISR();
     
     // Call the AEDEA timer tick handler.
     aedea_timer_tick();
     
     // Acknowledge the end of the interrupt.
     outportb(0x20, 0x20);
     
     AEDEA_EXIT_ISR();
}


/*
 * ----- Function: key_handler() -----
 */
static void interrupt key_handler(void)
{
     kbd_evt_t e;
     uint8_t keycode;

     AEDEA_ENTER_ISR();

     // Read key code from the keyboard controller.
     keycode = inportb(0x60);
     
     // Copy the key which caused the event.
     e.key = keycode & 0x7F;
     
     // Specify event type.
     if((keycode & 0x80) == 0)
     {
          e.event = EVT_KEY_PRESSED;
     }
     else
     {
          e.event = EVT_KEY_RELEASED;
     }
     
     // Post event to the keyboad.
     aedea_post_event(PID_KEYBOARD_PROCESS, &e);
     
     // Acknowledge the end of the interrupt.
     outportb(0x20, 0x20);
     
     AEDEA_EXIT_ISR();
}


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
