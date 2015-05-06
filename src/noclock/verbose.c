/**
 * \file verbose.c
 * \brief Verbose mode.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * This file defines the contents of the \ref verbose_group module.
 *
 * Internal state
 * --------------
 *
 * The internal state of the verbose mode is stored in the variable
 * #verbose_mode. It is a static variable and external manipulation must be
 * done using exclusively verbose_mode_state(), verbose_mode_enable() and
 * verbose_mode_disable().
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

#include "noclock/verbose.h"

////////////////////////////////////////////////////////////////////////////////
// Static variables.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief The current state of the verbose mode.
 * \since version `1.0.0`
 */
static bool verbose_mode = false;

////////////////////////////////////////////////////////////////////////////////
// Verbose mode state.
////////////////////////////////////////////////////////////////////////////////

void verbose_mode_enable (void)
{
    verbose_mode = true;
}

void verbose_mode_disable (void)
{
    verbose_mode = false;
}

bool verbose_mode_state (void)
{
    return verbose_mode;
}

////////////////////////////////////////////////////////////////////////////////
// Verbose formatted printers.
////////////////////////////////////////////////////////////////////////////////

int verbosef (const char * format, ...)
{
    if (! verbose_mode_state ())
        return 0;

    va_list ap;
    va_start (ap, format);
    int result = vprintf (format, ap);
    va_end (ap);

    return result;
}

int fverbosef (FILE * stream, const char * format, ...)
{
    if (! verbose_mode_state ())
        return 0;

    va_list ap;
    va_start (ap, format);
    int result = vfprintf (stream, format, ap);
    va_end (ap);

    return result;
}

int sverbosef (char * str, const char * format, ...)
{
    if (! verbose_mode_state ())
        return 0;

    va_list ap;
    va_start (ap, format);
    int result = vsprintf (str, format, ap);
    va_end (ap);

    return result;
}

int snverbosef (char * str, size_t size, const char * format, ...)
{
    if (! verbose_mode_state ())
        return 0;

    va_list ap;
    va_start (ap, format);
    int result = vsnprintf (str, size, format, ap);
    va_end (ap);

    return result;
}

