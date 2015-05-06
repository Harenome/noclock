/**
 * \file expression_list.c
 * \brief Expression lists
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * This file defines the contents of the \ref expression_list_group module.
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

#include "noclock/expression_list.h"

////////////////////////////////////////////////////////////////////////////////
// Static functions declarations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Delete coordinates from an expression list.
 * \since version `1.0.0`
 * \param list The expression list where coordinates must be deleted.
 * \return The resulting list.
 */
static expression_list * expression_list_strip_coords (expression_list * list);

/**
 * \brief Delete the first expression from an expression list.
 * \since version `1.0.0`
 * \param list The expression list where the first expression must be deleted.
 * \return The resulting list.
 */
static expression_list * expression_list_strip_first (expression_list * list);

/**
 * \brief Delete expressions that refer to keywords.
 * \since version `1.0.0`
 * \param list The expression list where the keywords must be deleted.
 * \return The resulting list.
 */
static expression_list * expression_list_strip_keywords (expression_list * list);

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

expression_list * expression_list_alloc (void)
{
    expression_list * list = malloc (sizeof * list);
    __forbid_value (list, NULL, "malloc", EX_OSERR);
    list->element = NULL;
    list->next = NULL;

    return list;
}

expression_list * expression_list_copy (
        const expression_list * list)
{
    expression_list * copy;

    if (list != NULL)
    {
        copy = expression_list_append (NULL,
                expression_copy (list->element));

        const expression_list * current, * tmp;
        for (current = list->next; current != NULL; current = tmp)
        {
            tmp = current->next;
            copy = expression_list_append (copy,
                    expression_copy (current->element));
        }
    }
    else
        copy = NULL;

    return copy;
}

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

size_t expression_list_size (expression_list * list)
{
    size_t i = 0;

    for (expression_list * current = list; current != NULL;
            current = current->next)
        i++;

    return i;
}

expression_list * expression_list_n (
        expression_list * list, size_t n)
{
    expression_list * nth = list;

    for (size_t i = 0; nth != NULL && i < n; ++i)
        nth = nth->next;

    return nth;
}

////////////////////////////////////////////////////////////////////////////////
// Modifications.
////////////////////////////////////////////////////////////////////////////////

expression_list * expression_list_append
        (expression_list * list,
         expression * const expr)
{
    expression_list * new_expr = malloc (sizeof * new_expr);

    if (new_expr == NULL)
    {
        perror ("malloc");
        exit (EX_OSERR);
    }

    new_expr->element = expr;
    new_expr->next = NULL;

    if (list != NULL)
    {
        expression_list * last;
        for (last = list; last->next != NULL; last = last->next)
            ;
        last->next = new_expr;
    }
    else
        list = new_expr;

    return list;
}

expression_list * expression_list_cat
        (expression_list * a, expression_list * b)
{
    if (a != NULL)
    {
        expression_list * last;
        for (last = a; last->next != NULL; last = last->next)
            ;
        last->next = b;
    }
    else
        a = b;

    return a;
}

void expression_list_free (expression_list * list)
{
    expression_list * current, * tmp;
    for (current = list; current != NULL; current = tmp)
    {
        tmp = current->next;
        expression_free (current->element);
        free (current);
    }
}

expression_list * expression_list_strip
        (expression_list * list)
{
    /* Removing the coordinates must be the very first operation! */
    list = expression_list_strip_coords (list);

    /* It *should* be safe to execute these operations in any order. */
    list = expression_list_strip_first (list);
    list = expression_list_strip_keywords (list);

    return list;
}

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

void expression_list_fprint (FILE * const f,
        const expression_list * list, const char * separator)
{
    char * list_string = expression_list_to_string (list, separator);
    fprintf (f, "%s", list_string);
    free (list_string);
}

void expression_list_print
        (const expression_list * const list, const char * separator)
{
    expression_list_fprint (stdout, list, separator);
}

char * expression_list_to_string (
        const expression_list * list, const char * separator)
{
    /* Default separator. */
    if (separator == NULL)
        separator = "\n";

    /* Default empty buffer. */
    char * buffer = malloc (4);
    __forbid_value (buffer, NULL, "malloc", EX_OSERR);
    buffer[0] = buffer[1] = buffer[2] = buffer[3] = '\0';

    for (const expression_list * current = list; ; )
    {
        char * current_str = expression_to_string (current->element);
        current = current->next;

        char * new_buffer = malloc (strlen (current_str) + strlen (buffer)
                + strlen (separator) + 8);
        __forbid_value (new_buffer, NULL, "malloc", EX_OSERR);

        sprintf (new_buffer, "%s%s%s", buffer, current_str,
                current != NULL ? separator : "");

        /* The previous buffer and the current expression string were used. */
        free (buffer);
        free (current_str);

        buffer = new_buffer;
        if (current == NULL)
            break;
    }

    return buffer;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions definitions.
////////////////////////////////////////////////////////////////////////////////

expression_list * expression_list_strip_coords (expression_list * list)
{
    /* We know that when expression lists that contain coordinates are
     * built, the coordinates are placed at even indexes (starting at 0):
     * this function shall delete expressions at even indexes (when the function
     * starts!) and keep expressions at odd indexes.
     */

    expression_list * current = list;
    while (current != NULL)
    {
        expression_list * next = NULL;
        if (current->next != NULL)
        {
            next = current->next->next;
            expression_free (current->next->element);
            free (current->next);
        }
        current->next = next;
        current = current->next;
    }

    return list;
}

expression_list * expression_list_strip_first (expression_list * list)
{
    expression_list * next = list->next;
    expression_free (list->element);
    free (list);
    return next;
}

expression_list * expression_list_strip_keywords (expression_list * list)
{
    expression_list * current = list;
    while (current != NULL)
    {
        expression_list * next = current->next;

        while (next != NULL && next->element->type == EXPR_ID)
        {
            char * id = next->element->content.identifier;
            if (strcmp (id, "f") == 0 || strcmp (id, "a") == 0)
            {
                current->next = next->next;
                expression_free (next->element);
                free (next);
                next = current->next;
            }
            else
                next = NULL;
        }
        current = current->next;
    }

    return list;
}
