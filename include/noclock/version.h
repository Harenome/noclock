/**
 * \file version.h
 * \brief Compile time infos.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
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

#ifndef __COMPILE_TIME_H__
#define __COMPILE_TIME_H__

#include <stdlib.h>
#include <stdio.h>

#include <isl/version.h>

/**
 * \defgroup infos_group Various informations
 * \brief Miscelleanous informations.
 * \since version `1.0.0`
 *
 * This module contains various informations such as:
 *
 * - compilation information: see the \ref compilation_group module.
 * - version information: see the \ref version_group module.
 *
 * Do note that some of these informations are gathered at compilation time.
 */

////////////////////////////////////////////////////////////////////////////////
// Static variables.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief The No Clock version major.
 * \ingroup version_group
 * \since version `1.0.0`
 */
static const unsigned int NOCLOCK_VERSION_MAJOR = 1;

/**
 * \brief The No Clock version minor.
 * \ingroup version_group
 * \since version `1.0.0`
 */
static const unsigned int NOCLOCK_VERSION_MINOR = 0;

/**
 * \brief The No Clock version patch.
 * \ingroup version_group
 * \since version `1.0.0`
 */
static const unsigned int NOCLOCK_VERSION_PATCH = 1;

////////////////////////////////////////////////////////////////////////////////
// No Clock version.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the No Clock version as a string.
 * \ingroup version_group
 * \since version `1.0.0`
 *
 * \return String representation of the current version.
 */
const char * noclock_version (void);

////////////////////////////////////////////////////////////////////////////////
// Compilation informations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup compilation_group Compilation informations
 * \ingroup infos_group
 * \brief Compilation characteristics.
 * \since version `1.0.0`
 *
 * This module contains compilation information which can be obtained as
 * strings:
 *
 * - the date of compilation: noclock_compiling_date()
 * - the compiler: noclock_compiler()
 * - the user who compiled: noclock_compiling_user()
 * - the host machine: noclock_compiling_host()
 *
 * These informations can be grouped into a unique string with
 * noclock_compilation_informations().
 *
 * Do note that some of these informations are gathered at compilation time.
 */

//----------------------------------------------------------------------------//

/**
 * \brief Get the compiler name and version as a string.
 * \ingroup compilation_group
 * \since version `1.0.0`
 *
 * \return String representation of the compiler.
 */
const char * noclock_compiler (void);

/**
 * \brief Get the date of compilation as a string.
 * \ingroup compilation_group
 * \since version `1.0.0`
 *
 * \return String representation of the date of compilation.
 */
const char * noclock_compiling_date (void);

/**
 * \brief Get the name of the user as a string.
 * \ingroup compilation_group
 * \since version `1.0.0`
 *
 * \return String representation of the user.
 */
const char * noclock_compiling_user (void);

/**
 * \brief Get the name of the host machine.
 * \ingroup compilation_group
 * \since version `1.0.0`
 *
 * \return String representation of the host machine.
 */
const char * noclock_compiling_host (void);

/**
 * \brief Get all compilation informations into a single string.
 * \ingroup compilation_group
 * \since version `1.0.0`
 *
 * \return String representation of the compilation informations.
 */
const char * noclock_compilation_informations (void);

////////////////////////////////////////////////////////////////////////////////
// Tools and libraries informations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup version_group Versions
 * \ingroup infos_group
 * \brief All the versions.
 * \since version `1.0.0`
 *
 * The versionning of No Clock is done following the rules of
 * Semantic Versionning.
 *
 * One can get the No Clock version as a string with noclock_version() or use
 * the #NOCLOCK_VERSION_MAJOR, #NOCLOCK_VERSION_MINOR and #NOCLOCK_VERSION_PATCH
 * variables.
 *
 * The versions of the various tools and libraries can also be converted into
 * strings:
 *
 * - ISL: noclock_isl_version()
 * - Yacc: noclock_yacc_version()
 * - Lex: noclock_lex_version()
 *
 * These informations can be grouped into a single string with
 * noclock_libraries_informations().
 *
 * Do note that some of these informations are gathered at compilation time.
 */

//----------------------------------------------------------------------------//

/**
 * \brief Get the ISL version.
 * \ingroup version_group
 * \since version `1.0.0`
 *
 * \return String representation of the ISL version.
 */
const char * noclock_isl_version (void);

/**
 * \brief Get the Yacc version.
 * \ingroup version_group
 * \since version `1.0.0`
 *
 * \return String representation of the Yacc version.
 */
const char * noclock_yacc_version (void);

/**
 * \brief Get the Lex version.
 * \ingroup version_group
 * \since version `1.0.0`
 *
 * \return String representation of the Lex version.
 */
const char * noclock_lex_version (void);

/**
 * \brief Get the tools and libraries information into a single string.
 * \ingroup version_group
 * \since version `1.0.0`
 *
 * \return String representation of tools and libraries versions.
 */
const char * noclock_libraries_informations (void);

#endif /* __COMPILE_TIME_H__ */
