/**
 * \file verbose.h
 * \brief Verbose mode.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \version 1.0.0
 *
 * This header file declares verbose printing utilities.
 *
 * For further information, see the \ref verbose_group module.
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

#ifndef __VERBOSE_H__
#define __VERBOSE_H__

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>

/**
 * \defgroup verbose_group Verbose printing
 * \brief Print too many messages.
 *
 * Verbose mode
 * ------------
 *
 * The verbose mode status can be inspected using verbose_mode_state(). It
 * can either be enabled or disabled using verbose_mode_enable() or
 * verbose_mode_disable().
 *
 * Usage
 * -----
 *
 * When the verbose mode is enabled, the following functions behave as
 * their *printf(3) counterparts:
 *
 * verbose printing function    | *printf(3) function
 * -----------------------------|--------------------
 * verbosef()                   | printf(3)
 * fverbosef()                  | fprintf(3)
 * sverbosef()                  | sprintf(3)
 * snverbosef()                 | snprintf(3)
 *
 * On the other hand, if the debugging mode is disabled, these functions do
 * nothing except returning 0.
 *
 */

////////////////////////////////////////////////////////////////////////////////
// Verbose mode state.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Enable the verbose mode.
 * \ingroup verbose_group
 */
void verbose_mode_enable (void);

/**
 * \brief Disable the verbose mode.
 * \ingroup verbose_group
 */
void verbose_mode_disable (void);

/**
 * \brief Get the verbose mode state.
 * \ingroup verbose_group
 * \retval true if the verbose mode is enabled.
 * \retval false otherwise.
 */
bool verbose_mode_state (void);

////////////////////////////////////////////////////////////////////////////////
// Verbose formatted printers.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief printf(3) when the verbose mode is enabled.
 * \param format The format string.
 * \retval 0 if the verbose mode is disabled.
 * \return The number of written characters.
 * \see printf(3)
 * \ingroup verbose_group
 */
int verbosef (const char * format, ...);

/**
 * \brief fprintf(3) when the verbose mode is enabled.
 * \param stream The destination stream.
 * \param format The format string.
 * \retval 0 if the verbose mode is disabled.
 * \return The number of written characters.
 * \see fprintf(3)
 * \ingroup verbose_group
 */
int fverbosef (FILE * stream, const char * format, ...);

/**
 * \brief sprintf(3) when the verbose mode is enabled.
 * \param str The destination string.
 * \param format The format string.
 * \retval 0 if the verbose mode is disabled.
 * \return The number of written characters.
 * \see sprintf(3)
 * \ingroup verbose_group
 */
int sverbosef (char * str, const char * format, ...);

/**
 * \brief snprintf(3) when the verbose mode is enabled.
 * \param str The destination string.
 * \param size The maximum number of characters to write.
 * \param format The format string.
 * \retval 0 if the verbose mode is disabled.
 * \return The number of written characters.
 * \see snprintf(3)
 * \ingroup verbose_group
 */
int snverbosef (char * str, size_t size, const char * format, ...);

#endif /* __VERBOSE_H__ */
