/**
 * \file instruction_to_set.c
 * \brief Instructions
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

#include "noclock/instruction_to_set.h"

////////////////////////////////////////////////////////////////////////////////
// Static functions declarations.
////////////////////////////////////////////////////////////////////////////////

static isl_set_list * instruction_to_set_list (isl_ctx * ctx,
        const string_list * parameters, const instruction * instr,
        string_list * s);

static int _union_set_list (isl_set * el, void * user);

static inline void _shift_map (unsigned int dimension, char * buffer,
        string_list * s, char * identifier);


////////////////////////////////////////////////////////////////////////////////
// No Clock -> ISL.
////////////////////////////////////////////////////////////////////////////////

isl_set_list * program_to_set_list (isl_ctx* ctx,
        const string_list * parameters, const instruction_list * instructions,
        string_list * s)
{
    isl_set_list * list = NULL;

    for (const instruction_list * current = instructions; current != NULL;
            current = current->next)
    {
        isl_set_list * current_list = instruction_to_set_list (
                ctx, parameters, current->element, s);

        if (list == NULL)
            list = current_list;
        else
            list = isl_set_list_concat (list, current_list);
    }

    return list;
}

isl_union_set * union_set_list (isl_set_list * list)
{
    isl_union_set * u = NULL;
    isl_set_list_foreach (list, _union_set_list, & u);
    return u;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions definitions.
////////////////////////////////////////////////////////////////////////////////

void _shift_map (unsigned int dimension, char * buffer,
        string_list * s, char * identifier)
{
    int written = 0;
    unsigned int limit = dimension - 1;
    char internal_buffer[512] = { '\0' };

    /* Fill the buffer with the first variables (x0, x1, x2, etc.) */
    for (unsigned int i = 0; i < limit; ++i)
        written += sprintf (& internal_buffer[written], "x%u%c", i,
                i < limit - 1 ? ',' : '\0');

    /* Find the index of the current instruction's name.
     * Append it to the string list if it is not found.
     */
    ssize_t place = string_list_index (s, identifier);
    if (place == -1)
        place = (ssize_t) string_list_append (s, identifier);

    /* Pass the very last variable to the first place, and add the index of
     * the current instruction's name.
     */
    sprintf (buffer, "{[%s,x]->[x,%s,%ld]}", internal_buffer, internal_buffer,
            place);
}

int _union_set_list (isl_set * el, void * user)
{
    isl_union_set ** u = (isl_union_set **) user;
    isl_union_set * u0 = isl_union_set_from_set (el);
    if (* u == NULL)
        * u = u0;
    else
        * u = isl_union_set_union (* u, u0);

    return 0;
}

isl_set_list * instruction_to_set_list (isl_ctx * ctx,
        const string_list * parameters, const instruction * instr,
        string_list * s)
{
    isl_set_list * list = NULL;

    instruction_type t = instr->type;
    if (t < INSTR_UNKNOWN)
    {
        if (t == INSTR_CALL)
        {
            char * parameters_string = string_list_to_string (parameters);
            char * set_string = malloc (strlen (instr->annotation.level)
                    + strlen (parameters_string)
                    + strlen (instr->annotation.boundaries) + 256);
            char * date_string =
                    expression_to_string (instr->annotation.date);

            sprintf (set_string, "[%s] -> { [%s]: %s and d = %s }",
                    parameters_string, instr->annotation.level,
                    instr->annotation.boundaries, date_string);
            isl_set * set = isl_set_read_from_str (ctx, set_string);

            fverbosef (stderr, "%s:\n", instr->content.call.identifier);
            fverbosef (stderr, "\t%s\n", set_string);

            char shift_map[1024];
            _shift_map (isl_set_dim (set, isl_dim_set), shift_map, s,
                    instr->content.call.identifier);

            isl_map * m = isl_map_read_from_str (ctx, shift_map);
            isl_set * mapped = isl_set_apply (set, m);
            list = isl_set_list_from_set (mapped);

            free (set_string);
            free (parameters_string);
            free (date_string);

        }
        else
            switch (t)
            {
                case INSTR_FOR:
                    list = program_to_set_list (ctx, parameters,
                            instr->content.loop.body, s);
                    break;
                case INSTR_FINISH:
                case INSTR_CLOCKED_FINISH:
                case INSTR_ASYNC:
                case INSTR_CLOCKED_ASYNC:
                    list = program_to_set_list (ctx, parameters,
                            instr->content.block, s);
                    break;
                case INSTR_IF:
                case INSTR_IF_ELSE:
                case INSTR_ADVANCE:
                default:
                    list = isl_set_list_alloc (ctx, 0);
                    break;
            }
    }

    return list;
}
