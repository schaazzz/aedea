/*!
 * \addtogroup aedea
 * @{
 */


/*!
 * \defgroup aedea_opt AEDEA options
 * @{
 *
 * Various configuration options for AEDEA.
 */


/*!
 * \file
 * AEDEA options file.
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
 
 
#ifndef __AEDEA_OPT_H
#define __AEDEA_OPT_H


/*!
 * Maximum number of processes.
 *
 * Set this to the maximum number of process callbacks that will be
 * registered with AEDEA by the application. This is used to determine
 * the size of AEDEA's internal event manager buffer.
 *
 * \hideinitializer
 */
#define AEDEA_OPT_MAX_PROCESSES    0x05


/*!
 * Set to 1 to use the software timer.
 *
 * \hideinitializer
 */
#define AEDEA_OPT_USE_SOFT_TMR     1


/*!
 * Maximum number of software timers.
 *
 * Set this to the maximum number of periodic processes to be used by the application.
 *
 * \hideinitializer
 * \note Only used if AEDEA_OPT_USE_SOFT_TMR is set to 1.
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
#define AEDEA_OPT_MAX_SOFT_TMRS    0x05
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


/*!
 * Process ID for the aedea timer process.
 *
 * \hideinitializer
 * \note Only required if AEDEA_OPT_USE_SOFT_TMR is set to 1. By default 0 is reserved as the process ID for
 * the timer process.
 */
#if(AEDEA_OPT_USE_SOFT_TMR == 1)
#define PID_AEDEA_TIMER_PROCESS    0
#endif    /* (AEDEA_OPT_USE_SOFT_TMR == 1) */


#endif    /* __AEDEA_OPT_H */


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
