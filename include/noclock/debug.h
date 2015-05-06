/**
 * \file debug.h
 * \brief Debug mode.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * This header file declares debugging utilities.
 *
 * For further information, see the \ref debug_group module.
 */

/* The MIT License (MIT)
 *
 * Copyright (c) 2015 Harenome RAZANAJATO RANAIVOARIVONY
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * \defgroup debug_group Debugging
 * \brief Print debugging information.
 * \since version `1.0.0`
 *
 * The debugging mode is determined at compilation time. One can inspect its
 * state using debug_mode_state().
 *
 * When the debugging mode is enabled, the following functions behave as
 * their *printf(3) counterparts:
 *
 * debug function   | *printf(3) function
 * -----------------|--------------------
 * debug()          | printf(3)
 * fdebug()         | fprintf(3)
 * sdebug()         | sprintf(3)
 * sndebug()        | snprintf(3)
 *
 * On the other hand, if the debugging mode is disabled, these functions do
 * nothing except returning 0.
 */

////////////////////////////////////////////////////////////////////////////////
// Debug mode.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Determine whether the debug mode has been enabled at compile time.
 * \ingroup debug_group
 * \since version `1.0.0`
 *
 * \retval true if the debug mode is enabled.
 * \retval false otherwise.
 */
bool debug_mode_state (void);

////////////////////////////////////////////////////////////////////////////////
// Debugging formatted printers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief printf(3) when DEBUG is defined.
 * \ingroup debug_group
 * \since version `1.0.0`
 *
 * \param format The format string.
 * \return The number of written characters.
 * \retval 0 if DEBUG is not defined
 *
 * \see printf(3)
 */
int debug (const char * format, ...);

/**
 * \brief fprintf(3) when DEBUG is defined.
 * \ingroup debug_group
 * \since version `1.0.0`
 *
 * \param stream The destination stream.
 * \param format The format string.
 * \return The number of written characters.
 * \retval 0 if DEBUG is not defined
 *
 * \see fprintf(3)
 */
int fdebug (FILE * stream, const char * format, ...);

/**
 * \brief sprintf(3) when DEBUG is defined.
 * \ingroup debug_group
 * \since version `1.0.0`
 *
 * \param str The destination string.
 * \param format The format string.
 * \return The number of written characters.
 * \retval 0 if DEBUG is not defined
 *
 * \see sprintf(3)
 */
int sdebug (char * str, const char * format, ...);

/**
 * \brief snprintf(3) when DEBUG is defined.
 * \ingroup debug_group
 * \since version `1.0.0`
 *
 * \param str The destination string.
 * \param size The maximum number of characters to write.
 * \param format The format string.
 * \return The number of written characters.
 * \retval 0 if DEBUG is not defined
 *
 * \see snprintf(3)
 */
int sndebug (char * str, size_t size, const char * format, ...);

#endif /* __DEBUG_H__ */
