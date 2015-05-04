/**
 * \file expression_list.h
 * \brief Expression lists.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \version 1.0.0
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

#ifndef __EXPRESSION_LIST_H__
#define __EXPRESSION_LIST_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sysexits.h>
#include <string.h>

#include "noclock/expression.h"

////////////////////////////////////////////////////////////////////////////////
// Structs, ennums, typedefs, etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief expression list.
 */
typedef struct expression_list
{
    expression * element;                /**< The current element. */
    struct expression_list * next;       /**< The next element. */
} expression_list;

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Allocate an expression list.
 * \return A pointer to the newly allocated list.
 */
expression_list * expression_list_alloc (void);

/**
 * \brief Free an expression list.
 * \param list A pointer to the expression list to free.
 */
void expression_list_free (expression_list * list);

/**
 * \brief Copy an expressio list.
 * \param list List to copy.
 * \return A pointer to the copy.
 */
expression_list * expression_list_copy (const expression_list * list);

////////////////////////////////////////////////////////////////////////////////
// Modifications.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Append an expression to an expression list.
 * \param list List where the expression shall be appended.
 * \param expression Expression to append.
 * \return The resulting list.
 * \warning This function may modify the input list. Please use the returned pointer.
 * \note The pointer returned by this function may be equal to the input list.
 */
expression_list * expression_list_append (expression_list * list,
        expression * expression);

/**
 * \brief Concatenate two expression lists.
 * \param a First list.
 * \param b Second list.
 * \warning This function may modify the input lists. Please use the returned pointer.
 * \note The pointer returned by this function may be equal to one of the input lists.
 */
expression_list * expression_list_cat (expression_list * a, expression_list * b);

expression_list * expression_list_strip (expression_list * list);

expression_list * expression_list_n (expression_list * list, size_t n);

size_t expression_list_size (expression_list * list);

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Write an expression list to an output stream.
 * \param f Output stream.
 * \param list List to write.
 * \param separator Separator.
 */
void expression_list_fprint (FILE * f, const expression_list * list,
        const char * separator);

/**
 * \brief Write an expression list to stdout.
 * \param list List to write.
 * \param separator Separator.
 */
void expression_list_print (const expression_list * list,
        const char * separator);

/**
 * \brief Convert an expression list to a string.
 * \param e The expression list to convert into a string.
 * \param separator Separator.
 * \warning It is up to the user to free the resulting string.
 * \return The resulting string.
 */
char * expression_list_to_string (const expression_list * list,
        const char * separator);

#endif /* __EXPRESSION_LIST_H__ */
