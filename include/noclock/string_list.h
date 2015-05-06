/**
 * \file string_list.h
 * \brief Parameter list
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

#ifndef __string_list_H__
#define __string_list_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * \defgroup string_list_group String list
 * \brief String lists handling.
 * \since version `1.0.0`
 */

////////////////////////////////////////////////////////////////////////////////
// Structs, ennums, typedefs, etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief String list.
 * \ingroup string_list_group
 * \since version `1.0.0`
 */
typedef struct string_list
{
    size_t length;      /**< Current length of the string list. */
    size_t size;        /**< Current maximum size of the string list. */
    char ** list;       /**< List. */
} string_list;

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup string_list_management Constructors / Destructors
 * \ingroup string_list_group
 * \brief Create or destroy a string list.
 * \since version `1.0.0`
 *
 * To initialize a list, use string_list_init(). To clean it, use
 * string_list_clean().
 */

//----------------------------------------------------------------------------//

/**
 * \brief Initialize a string list.
 * \relates string_list
 * \ingroup string_list_management
 * \since version `1.0.0`
 *
 * \param list List to initialize.
 */
void string_list_init (string_list * list);

/**
 * \brief Clean a string list.
 * \relates string_list
 * \ingroup string_list_management
 * \since version `1.0.0`
 *
 * \param list List to initialize.
 */
void string_list_clean (string_list * list);

////////////////////////////////////////////////////////////////////////////////
// Operations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup string_list_operation Operations
 * \ingroup string_list_group
 * \brief Operations on ::string_list
 * \since version `1.0.0`
 *
 * Operations defined on ::string_list:
 *
 * - string_list_append()
 */

//----------------------------------------------------------------------------//

/**
 * \brief Append a string to a string list.
 *
 * \param list String list.
 * \relates string_list
 * \ingroup string_list_operation
 * \param s String to append.
 *
 * \return Index of the newly added string.
 */
size_t string_list_append (string_list * list, const char * s);

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup string_list_getter Getters
 * \ingroup string_list_group
 * \brief Getters on ::sting_list
 * \since version `1.0.0`
 *
 * Getters on ::string_list:
 *
 * - string_list_index()
 * - string_list_parameter()
 */

//----------------------------------------------------------------------------//

/**
 * \brief Index of a string in a string list.
 * \relates string_list
 * \ingroup string_list_getter
 * \since version `1.0.0`
 *
 * \param list String list.
 * \param s String to append.
 *
 * \return Index of the first occurence of the string if it exists.
 * \retval -1 if the string does not exist.
 */
ssize_t string_list_index (string_list * list, const char * s);

/**
 * \brief Get the string at an index.
 * \relates string_list
 * \ingroup string_list_getter
 * \since version `1.0.0`
 *
 * \param list String list.
 * \param place Index.
 *
 * \return The string at the index.
 * \retval NULL if the index is too big.
 */
const char * string_list_parameter (string_list * list, ssize_t place);

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup string_list_io Input/Output
 * \ingroup string_list_group
 * \brief I/O on ::sting_list
 * \since version `1.0.0`
 *
 * Input/Ouput functions:
 *
 * - string_list_fprint()
 * - string_list_to_string()
 */

//----------------------------------------------------------------------------//


/**
 * \brief Print a string list to a stream.
 * \relates string_list
 * \ingroup string_list_io
 * \since version `1.0.0`
 *
 * \param f Stream.
 * \param list List to print.
 */
void string_list_fprint (FILE * f, const string_list * list);

/**
 * \brief Convert a string list into a string.
 * \relates string_list
 * \ingroup string_list_io
 * \since version `1.0.0`
 *
 * \param list List to print.
 *
 * \return String representation of the string list.
 */
char * string_list_to_string (const string_list * list);

#endif /* __string_list_H__ */
