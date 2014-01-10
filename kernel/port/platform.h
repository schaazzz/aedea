/*!
 * \addtogroup aedea
 * @{
 */


/*!
 * \defgroup platform_defs Platform specific definitions
 * @{
 *
 * Contains platform specific datatype definitions used by AEDEA.
 */


/*!
 * \file
 * AEDEA platform specific definitions. This file can be completely replaced with a
 * project/platform specific header file.
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


#ifndef __PLATFORM_H
#define __PLATFORM_H


/*
 * Define platform type below to specify platform datatypes and interrupt locking/unlocking
 * macros.
 */
#define EXAMPLE_PC_TURBOC


/*
 * ----- AVR GCC Example -----
 */
#ifdef EXAMPLE_AVR_GCC

/*!
 * Platform architecture type (8-bit, 16-bit or 32-bit).
 */
#define PLATFORM_ARCH    8

#endif    /* EXAMPLE_AVR_GCC */


/*
 * ----- PC TurboC/C++ Example -----
 */
#ifdef EXAMPLE_PC_TURBOC

#include <dos.h>

/*!
 * Platform specific interrupt locking macro.
 *
 * \hideinitializer
 */
#define PORT_LOCK_INTERRUPTS()     disable()

/*!
 * Platform specific interrupt unlocking macro.
 *
 * \hideinitializer
 */
#define PORT_UNLOCK_INTERRUPTS()   enable()

/*!
 * Platform architecture type (8-bit, 16-bit or 32-bit).
 */
#define PLATFORM_ARCH    16

#endif    /* EXAMPLE_PC_TURBOC */


/*
 * ----- PC OpenWatcom Example -----
 */
#ifdef EXAMPLE_PC_OPEN_WATCOM

/*!
 * Platform architecture type (8-bit, 16-bit or 32-bit).
 */
#define PLATFORM_ARCH    32

#endif    /* EXAMPLE_PC_OPEN_WATCOM */


/*
 * ----- ARM ADS Example -----
 */
#ifdef EXAMPLE_ARM_ADS

/*!
 * Platform architecture type (8-bit, 16-bit or 32-bit).
 */
#define PLATFORM_ARCH    32

#endif    /* EXAMPLE_ARM_ADS */


/*
 * ----- ARM GCC Example -----
 */
#ifdef EXAMPLE_ARM_GCC

/*!
 * Platform architecture type (8-bit, 16-bit or 32-bit).
 */
#define PLATFORM_ARCH    32

#endif    /* EXAMPLE_ARM_GCC */


/*
 * Platform specific data types. These datatypes only apply to the platforms supported
 * in the examples.
 */
/*
 * ----- 8-bit Architecture -----
 */
#if PLATFORM_ARCH == 8

typedef unsigned char uint8_t;               //!< 8-bit unsigned data type.
typedef unsigned int uint16_t;               //!< 16-bit unsigned data type.

typedef char int8_t;                         //!< 8-bit unsigned data type.
typedef int int16_t;                         //!< 16-bit unsigned data type.

typedef int16_t port_int_t;                  //!< Platform signed int data type (AEDEA uses this to allow for different integer widths for different platforms).
typedef uint16_t port_uint_t;                //!< Platform unsigned int data type (AEDEA uses this to allow for different integer widths for different platforms).

#endif    /* PLATFORM_ARCH == 8 */


/*
 * ----- 16-bit Architecture -----
 */
#if PLATFORM_ARCH == 16

typedef unsigned char uint8_t;               //!< 8-bit unsigned data type.
typedef unsigned int uint16_t;               //!< 16-bit unsigned data type.

typedef char int8_t;                         //!< 8-bit unsigned data type.
typedef int int16_t;                         //!< 16-bit unsigned data type.

typedef int16_t port_int_t;                  //!< Platform signed int data type (AEDEA uses this to allow for different integer widths for different platforms).
typedef uint16_t port_uint_t;                //!< Platform unsigned int data type (AEDEA uses this to allow for different integer widths for different platforms).

#endif    /* PLATFORM_ARCH == 16 */


/*
 * ----- 32-bit Architecture -----
 */
#if PLATFORM_ARCH == 32

typedef unsigned char uint8_t;               //!< 8-bit unsigned data type.
typedef unsigned short uint16_t;             //!< 16-bit unsigned data type.
typedef unsigned int uint32_t;               //!< 32-bit unsigned data type.
                                             
typedef char int8_t;                         //!< 8-bit unsigned data type.
typedef short int16_t;                       //!< 16-bit unsigned data type.
typedef int int32_t;                         //!< 32-bit unsigned data type.
                                             
typedef int32_t port_int_t;                  //!< Platform signed int data type (AEDEA uses this to allow for different integer widths for different platforms).
typedef uint32_t port_uint_t;                //!< Platform unsigned int data type (AEDEA uses this to allow for different integer widths for different platforms).

#endif    /* PLATFORM_ARCH == 32 */


typedef unsigned char bool_t;                //!< Boolean type.


/*
 * Constant definitions.
 */
#ifndef NULL
#define NULL   0
#endif    /* NULL */

#ifndef TRUE
#define TRUE   1
#endif    /* TRUE */

#ifndef FALSE
#define FALSE  0
#endif    /* FALSE */


#endif    /* __PLATFORM_H */


/*----------------------------------------------------------------------------*/
/*! @} */
/*! @} */
