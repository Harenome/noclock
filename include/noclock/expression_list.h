/**
 * \file expression_list.h
 * \brief Expression lists.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \version 1.0.0
 *
 * This file declares expression list utilities.
 *
 * For further information, see the \ref expression_list_group module.
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

/**
 * \defgroup expression_list_group Lists of expressions
 * \ingroup expression_global_group
 * \brief Lists of ::expression `struct`s.
 * \since version `1.0.0`
 *
 * This module declares the ::expression_list `struct` which is used to handle
 * lists of ::expression `struct`s.
 *
 * For further information on:
 * - ::expression_list constructors or destructors, see
 *   \ref expression_list_management.
 * - ::expression_list getters, see \ref expression_list_getter
 * - ::expression_list operations, see \ref expression_list_operation
 * - ::expression_list input/output, see \ref expression_list_io
 *
 * Lists of ::expression `struct`s are linked lists. An ::expression_list `struct`
 * is a node which refers to an ::expression and to the next ::expression_list
 * node.
 *
 * Given an ::expression_list named \a list, the recommended way to iterate on
 * its elements is as follows:
 * ~~~C
 * for (expression_list * node = list; node != NULL; node = node->next)
 * {
 *     expression * expr = node->element;
 *     // Use expr.
 * }
 * ~~~
 */

/**
 * \defgroup expression_list_management Constructors / Destructors
 * \ingroup expression_list_group
 * \brief Create or destroy ::expression_list lists.
 *
 * An ::expression_list node can be created using expression_list_alloc().
 *
 * An ::expression_list list must be destroyed using expression_list_free().
 * Lists can be copied using expression_list_copy().
 */

/**
 * \defgroup expression_list_getter Getters
 * \ingroup expression_list_group
 * \brief Get properties of an ::expression_list list.
 */

/**
 * \defgroup expression_list_operation Operations
 * \ingroup expression_list_group
 * \brief Operations ::expression_list lists.
 */

/**
 * \defgroup expression_list_io Input / Output
 * \ingroup expression_list_group
 * \brief I/O on ::expression_list lists.
 */

////////////////////////////////////////////////////////////////////////////////
// Structs, ennums, typedefs, etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief expression list.
 * \ingroup expression_list_group
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
 * \relates expression_list
 * \ingroup expression_list_management
 * \since version `1.0.0`
 *
 * \return A pointer to the newly allocated list.
 */
expression_list * expression_list_alloc (void);

/**
 * \brief Free an expression list.
 * \relates expression_list
 * \ingroup expression_list_management
 * \since version `1.0.0`
 *
 * \param list A pointer to the expression list to free.
 *
 * This function frees an ::expression_list node and its successors.
 */
void expression_list_free (expression_list * list);

/**
 * \brief Copy an expression list.
 * \relates expression_list
 * \ingroup expression_list_management
 * \since version `1.0.0`
 *
 * \param list List to copy.
 * \return A pointer to the copy.
 */
expression_list * expression_list_copy (const expression_list * list);

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the nth expression from an expression list.
 * \relates expression_list
 * \ingroup expression_list_getter
 * \since version `1.0.0`
 *
 * \param list The expression list.
 * \param n The index of the expression.
 * \return The resulting list.
 * \retval NULL if \a n is greater than the size of the list.
 */
expression_list * expression_list_n (expression_list * list, size_t n);

/**
 * \brief Get the size of an expression list.
 * \relates expression_list
 * \ingroup expression_list_getter
 * \since version `1.0.0`
 *
 * \param list The expression list.
 * \return The size of the expression list.
 */
size_t expression_list_size (expression_list * list);

////////////////////////////////////////////////////////////////////////////////
// Modifications.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Append an expression to an expression list.
 * \relates expression_list
 * \ingroup expression_list_operation
 * \since version `1.0.0`
 *
 * \param list List where the expression shall be appended.
 * \param expression Expression to append.
 * \return The resulting list.
 *
 * \note The pointer returned by this function may be equal to the input list.
 *
 * \warning This function may modify the input list but do not assume the
 * pointer to the input list may still be safely used and please use the
 * returned pointer.
 *
 * \warning The correct way to use this function is to store the return value,
 * as follows:
 * ~~~C
 * list = expression_list_append (list, expr);
 * ~~~
 *
 * \warning Whereas discarding the return value as follows is an incorrect use of this
 * function:
 * as follows:
 * ~~~C
 * expression_list_append (list, expr);
 * ~~~
 */
expression_list * expression_list_append (expression_list * list,
        expression * expression);

/**
 * \brief Concatenate two expression lists.
 * \relates expression_list
 * \ingroup expression_list_operation
 * \since version `1.0.0`
 *
 * \param a First list.
 * \param b Second list.
 * \return The resulting list.
 *
 * \note The pointer returned by this function may be equal to one of the input lists.
 *
 * \warning This function may modify the input lists but do not assume the
 * pointer to the input lists may still be safely used and please use the
 * returned pointer.
 *
 * \warning The **correct ways** to use this function is to store the return
 * value, * as follows:
 * ~~~C
 * result = expression_list_cat (a, b);
 * a = expression_list_cat (a, b);
 * b = expression_list_cat (a, b);
 * ~~~
 *
 * \warning Whereas discarding the return value as follows is an
 * **incorrect use** of this function:
 * ~~~C
 * expression_list_cat (a, b);
 * ~~~
 */
expression_list * expression_list_cat (expression_list * a, expression_list * b);

/**
 * \brief Strip an expression list.
 * \relates expression_list
 * \ingroup expression_list_operation
 * \since version `1.0.0`
 *
 * \param list The expression list
 * \return The resulting list.
 */
expression_list * expression_list_strip (expression_list * list);

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Write an expression list to an output stream.
 * \relates expression_list
 * \ingroup expression_list_io
 * \since version `1.0.0`
 *
 * \param f Output stream.
 * \param list List to write.
 * \param separator Separator.
 *
 * This function will print each ::expression found in the list using
 * expression_to_fprint(). If a \a separator is provided, it will be printed
 * between the expressions.
 */
void expression_list_fprint (FILE * f, const expression_list * list,
        const char * separator);

/**
 * \brief Write an expression list to stdout.
 * \relates expression_list
 * \ingroup expression_list_io
 * \since version `1.0.0`
 *
 * \param list List to write.
 * \param separator Separator.
 *
 * \note This is equivalent to:
 * ~~~C
 * expression_list_fprint (stdout, list, separator);
 * ~~~
 */
void expression_list_print (const expression_list * list,
        const char * separator);

/**
 * \brief Convert an expression list to a string.
 * \relates expression_list
 * \ingroup expression_list_io
 * \since version `1.0.0`
 *
 * \param list The expression list to convert into a string.
 * \param separator Separator.
 * \return The resulting string.
 *
 * This function will convert each ::expression found in the string using
 * expression_to_string(). The resulting strings will be concatenated into
 * the string returned by this function. If a \a separator is provided, it
 * will be used to delimit the expressions.
 *
 * \warning It is up to the user to free the resulting string.
 */
char * expression_list_to_string (const expression_list * list,
        const char * separator);

#endif /* __EXPRESSION_LIST_H__ */
