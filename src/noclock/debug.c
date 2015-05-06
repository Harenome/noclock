/**
 * \file debug.c
 * \brief Debug mode.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * This file defines the contents of the \ref debug_group module.
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

#include "noclock/debug.h"

////////////////////////////////////////////////////////////////////////////////
// Debug mode.
////////////////////////////////////////////////////////////////////////////////

bool debug_mode_state (void)
{
    #ifdef DEBUG
        return true;
    #else
        return false;
    #endif
}

////////////////////////////////////////////////////////////////////////////////
// Debugging formatted printers.
////////////////////////////////////////////////////////////////////////////////

int debug (const char * format, ...)
{
    #ifdef DEBUG
        va_list ap;
        va_start (ap, format);
        int result = vprintf (format, ap);
        va_end (ap);
        return result;
    #else
        return 0;
    #endif
}

int fdebug (FILE * stream, const char * format, ...)
{
    #ifdef DEBUG
        va_list ap;
        va_start (ap, format);
        int result = vfprintf (stream, format, ap);
        va_end (ap);
        return result;
    #else
        return 0;
    #endif
}

int sdebug (char * str, const char * format, ...)
{
    #ifdef DEBUG
        va_list ap;
        va_start (ap, format);
        int result = vsprintf (str, format, ap);
        va_end (ap);
        return result;
    #else
        return 0;
    #endif
}

int sndebug (char * str, size_t size, const char * format, ...)
{
    #ifdef DEBUG
        va_list ap;
        va_start (ap, format);
        int result = vsnprintf (str, size, format, ap);
        va_end (ap);
        return result;
    #else
        return 0;
    #endif
}

