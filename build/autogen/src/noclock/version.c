/**
 * \file version.c
 * \brief Version.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \version 1.0.0
 * \since version `1.0.0`
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

#include "noclock/version.h"

////////////////////////////////////////////////////////////////////////////////
// No Clock version.
////////////////////////////////////////////////////////////////////////////////

const char * noclock_version (void)
{
    static char __version[128] = { '\0' };
    sprintf (__version, "%u.%u.%u", NOCLOCK_VERSION_MAJOR,
            NOCLOCK_VERSION_MINOR, NOCLOCK_VERSION_PATCH);

    return __version;
}

////////////////////////////////////////////////////////////////////////////////
// Compilation informations.
////////////////////////////////////////////////////////////////////////////////

const char * noclock_compiler (void)
{
    #if defined(__clang__)
        return "clang " __VERSION__;
    #elif defined(__GNUC__) || defined(__GNUG__)
        return "gcc " __VERSION__;
    #else
        return "some obscure compiler";
    #endif
}

const char * noclock_compiling_date (void)
{
    return __DATE__ ", " __TIME__;
}

const char * noclock_compiling_user (void)
{
    return "harenome";
}

const char * noclock_compiling_host (void)
{
    return "hr-edge.home";
}

const char * noclock_compilation_informations (void)
{
    static char __infos[256] = { '\0' };

    sprintf (__infos,
            "Compilation informations:\n"
            "\t%-16s%s\n"
            "\t%-16s%s\n"
            "\t%-16s%s@%s\n",
            "date:", noclock_compiling_date (),
            "compiler:", noclock_compiler (),
            "compiled by:", noclock_compiling_user (),
                noclock_compiling_host ()
            );

    return __infos;
}

////////////////////////////////////////////////////////////////////////////////
// Tools and libraries informations.
////////////////////////////////////////////////////////////////////////////////

const char * noclock_isl_version (void)
{
    return isl_version ();
}

const char * noclock_yacc_version (void)
{
    return "yacc - 1.9 20130925";
}

const char * noclock_lex_version (void)
{
    return "lex 2.5.37";
}

const char * noclock_libraries_informations (void)
{
    static char __libraries[2048];

    sprintf (__libraries,
            "Libraries and tools used:\n"
            "\t%s\n"
            "\t%s\n"
            "\t%s\n",
            noclock_yacc_version (),
            noclock_lex_version (),
            noclock_isl_version ()
            );

    return __libraries;
}
