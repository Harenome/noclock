/**
 * \file string_list.c
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

#include "noclock/string_list.h"

////////////////////////////////////////////////////////////////////////////////
// Static functions declarations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Grow a string list.
 * \since version `1.0.0`
 *
 * \param list String list to grow.
 */
static inline void string_list_grow (string_list * list);

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

void string_list_init (string_list * list)
{
    list->length = 0;
    list->size = 0;
    list->list = NULL;
}

void string_list_clean (string_list * list)
{
    for (size_t i = 0; i < list->length; ++i)
        free (list->list[i]);
    free (list->list);
    string_list_init (list);
}
////////////////////////////////////////////////////////////////////////////////
// Operations.
////////////////////////////////////////////////////////////////////////////////

size_t string_list_append (string_list * list, const char * parameter)
{
    if (list->length >= list->size)
        string_list_grow (list);

    if (list->length < list->size)
        list->list[list->length++] = strdup (parameter);

    return list->length - 1;
}

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

ssize_t string_list_index (string_list * list, const char * parameter)
{
    ssize_t place = -1;

    for (ssize_t i = 0; place == -1 && i < (ssize_t) list->length; ++i)
        if (strcmp (parameter, list->list[i]) == 0)
            place = i;

    return place;
}

const char * string_list_parameter (string_list * list, ssize_t place)
{
    if (place < 0 || place >= (ssize_t) list->length)
        return NULL;

    return list->list[place];
}

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

void string_list_fprint (FILE * f, const string_list * list)
{
    if (list == NULL)
        return;

    for (size_t i = 0; i < list->length; ++i)
        fprintf (f, "%s%s", list->list[i], i < list->length - 1 ? ", " : "");
}

char * string_list_to_string (const string_list * list)
{
    char * buffer = malloc (256);
    char * buffer_i = buffer;
    for (size_t i = 0; i < list->length; ++i)
    {
        sprintf (buffer_i, "%s%s", list->list[i],
                i < list->length - 1 ? ", " : "");
        buffer_i = & buffer_i[strlen(buffer_i)];
    }

    return buffer;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions definitions.
////////////////////////////////////////////////////////////////////////////////

void string_list_grow (string_list * list)
{
    static const size_t step = 8;

    size_t new_size = list->size + step;
    char ** new_list = realloc (list->list,
            (list->size + step) * sizeof * new_list);

    if (new_list != NULL)
    {
        list->list = new_list;
        list->size = new_size;
    }
}

