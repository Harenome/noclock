/**
 * \file instruction_list.c
 * \brief Instruction lists.
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

#include "noclock/instruction_list.h"

instruction_list * instruction_list_alloc (void)
{
    instruction_list * list = malloc (sizeof * list);
    if (list != NULL)
    {
        list->element = NULL;
        list->next = NULL;
    }
    return list;
}

void instruction_list_soft_free (instruction_list * list)
{
    instruction_list * current, * tmp;
    for (current = list; current != NULL; current = tmp)
    {
        tmp = current->next;
        free (current);
    }
}

void instruction_list_free (instruction_list * list)
{
    instruction_list * current, * tmp;
    for (current = list; current != NULL; current = tmp)
    {
        instruction * i = current->element;
        tmp = current->next;
        instruction_free (i);
        free (current);
    }
}


instruction_list * instruction_list_append (instruction_list * list,
        instruction * i)
{
    instruction_list * new_instruction = malloc (sizeof * new_instruction);

    if (new_instruction == NULL)
    {
        perror ("malloc");
        exit (EX_OSERR);
    }

    new_instruction->element = i;
    new_instruction->next = NULL;

    if (list != NULL)
    {
        instruction_list * last;
        for (last = list; last->next != NULL; last = last->next)
            ;
        last->next = new_instruction;
    }
    else
        list = new_instruction;

    return list;
}

instruction_list * instruction_list_cat (instruction_list * a,
        instruction_list * b)
{
    if (a != NULL)
    {
        instruction_list * last;
        for (last = a; last->next != NULL; last = last->next)
            ;
        last->next = b;
    }
    else
        a = b;

    return a;
}


void instruction_list_fprint (FILE * f, const instruction_list * list)
{
    for (const instruction_list * current = list; current != NULL;
            current = current->next )
    {
        instruction_fprint (f, current->element);
        if (current->element->type == INSTR_ADVANCE
                || current->element->type == INSTR_CALL)
            fprintf (f, ";\n");
    }
}

void instruction_list_decorate (instruction_list * list, const char * level,
    const char * boundaries)
{
    size_t position = 0;
    instruction_list * current = list;

    while (current != NULL)
    {
        char * current_level;
        if (level == NULL)
            current_level = malloc (8);
        else
            current_level = malloc (strlen (level) + 8);
        sprintf (current_level, "%s%s%lu", level == NULL ? "" : level,
                level == NULL ? "" : ",", position);
        current->element->annotation.level = current_level;
        current->element->annotation.boundaries =
                boundaries ? strdup (boundaries) : NULL;

        instruction_type t = current->element->type;
        if (t < INSTR_UNKNOWN)
        {
            size_t next_level_strlen = strlen (current_level) + 8;
            char next_level[next_level_strlen];
            char * next_boundaries = NULL;
            char * left_string = NULL;
            char * right_string = NULL;

            switch (t)
            {
                case INSTR_CALL:
                    sprintf (next_level, "%s,d", current_level);
                    sprintf (current_level, "%s", next_level);
                    break;
                case INSTR_FOR:
                    sprintf (next_level, "%s,%s", current_level,
                            current->element->content.loop.identifier);
                    if (boundaries != NULL)
                    {
                        next_boundaries = malloc (strlen (boundaries) + 256);
                        left_string = expression_to_string
                                (current->element->content.loop.left_boundary);
                        right_string = expression_to_string
                                (current->element->content.loop.right_boundary);

                        sprintf (next_boundaries, "%s and %s <= %s <= %s",
                                boundaries,
                                left_string,
                                current->element->content.loop.identifier,
                                right_string
                                );
                    }
                    else
                    {
                        next_boundaries = malloc (256);
                        left_string = expression_to_string
                                (current->element->content.loop.left_boundary);
                        right_string = expression_to_string
                                (current->element->content.loop.right_boundary);

                        sprintf (next_boundaries, "%s <= %s <= %s",
                                left_string,
                                current->element->content.loop.identifier,
                                right_string
                                );
                    }
                    instruction_list_decorate (
                            current->element->content.loop.body,
                            next_level, next_boundaries);
                    break;
                case INSTR_IF:
                case INSTR_IF_ELSE:
                    break;
                case INSTR_ADVANCE:
                    break;
                case INSTR_CLOCKED_FINISH:
                case INSTR_FINISH:
                    sprintf (next_level, "%s,f", current_level);
                    instruction_list_decorate (
                            current->element->content.block,
                            next_level, boundaries);
                    break;
                case INSTR_ASYNC:
                case INSTR_CLOCKED_ASYNC:
                    sprintf (next_level, "%s,a", current_level);
                    instruction_list_decorate (
                            current->element->content.block,
                            next_level, boundaries);
                    break;
                default:
                    break;
            }

            free (next_boundaries);
            free (left_string);
            free (right_string);
        }

        if (t != INSTR_ADVANCE)
            position++;
        current = current->next;
    }
}

static expression * _count_advances (instruction_list * list)
{
    expression * count = expression_alloc ();
    count->type = EXPR_NUMBER;
    count->content.number = 0;

    for (instruction_list * current = list; current != NULL;
            current = current->next)
    {
        instruction_type t = current->element->type;

        if (t == INSTR_ADVANCE)
        {
            if (count->type == EXPR_NUMBER)
                count->content.number++;
            else
            {
                expression * right = expression_alloc ();
                right->type = EXPR_NUMBER;
                right->content.number = 1;

                count = expression_add (count, right);
            }
        }
        else if (t == INSTR_FOR)
        {
            expression * left = expression_copy
                (current->element->content.loop.left_boundary);
            expression * right = expression_copy
                (current->element->content.loop.right_boundary);

            expression * bounds =
                expression_sub (right, left);
            expression * one = expression_from_number (1);
            expression * real_bounds = expression_add (bounds, one);

            expression * for_block_advances = _count_advances
                (current->element->content.loop.body);

            expression * for_advances =
                expression_mult (real_bounds, for_block_advances);

            count = expression_add (count, for_advances);
        }
    }

    return count;
}

void instruction_list_compute_dates (instruction_list * list,
        const expression * e, const char * identifier)
{
    /* Count the advances. */
    expression * advance_count = _count_advances (list);

    /* Compute the dates for the current level. */
    for (instruction_list * current = list; current != NULL;
            current = current->next)
    {
        expression * date;

        if (identifier == NULL)
        {
            /* No identifier? Not in a for loop. */
            date = expression_alloc ();
            date->type = EXPR_NUMBER;
            date->content.number = 0;
        }
        else
        {
            expression * id_expr = expression_alloc ();
            expression * factor = expression_copy (advance_count);

            id_expr->type = EXPR_ID;
            id_expr->content.identifier = strdup (identifier);

            date = expression_mult (id_expr, factor);
        }

        if (e != NULL)
        {
            expression * upper_level = expression_copy (e);

            date = expression_add (date, upper_level);
        }

        current->element->annotation.date = date;
    }

    /* Add the advances. */
    expression * advances = expression_alloc ();
    advances->type = EXPR_NUMBER;
    advances->content.number = 0;
    for (instruction_list * current = list; current != NULL;
            current = current->next)
    {
        instruction * i = current->element;
        instruction_type t = i->type;

        i->annotation.date = expression_add (i->annotation.date,
                expression_copy (advances));

        if (t == INSTR_ADVANCE)
        {
            if (advances->type == EXPR_NUMBER)
                advances->content.number++;
            else
            {
                expression * right = expression_alloc ();
                right->type = EXPR_NUMBER;
                right->content.number = 1;

                advances = expression_add (advances, right);
            }
        }
        else if (t == INSTR_FOR)
        {
            expression * left = expression_copy
                (current->element->content.loop.left_boundary);
            expression * right = expression_copy
                (current->element->content.loop.right_boundary);
            expression * bounds = expression_sub
                (right, left);

            expression * for_block_advances = _count_advances
                (current->element->content.loop.body);

            expression * for_advances =
                expression_mult (bounds, for_block_advances);

            advances = expression_add (advances, for_advances);
        }
    }

    /* Compute the dates for the inner levels. */
    for (instruction_list * current = list; current != NULL;
            current = current->next)
    {
        instruction * i = current->element;
        instruction_type t = i->type;

        if (t != INSTR_CALL && t != INSTR_ADVANCE && t != INSTR_UNKNOWN)
        {
            if (t == INSTR_FOR)
            {
                expression * date = expression_sub
                    (expression_copy (i->annotation.date),
                     expression_copy (i->content.loop.left_boundary));

                instruction_list_compute_dates (i->content.loop.body,
                        date, i->content.loop.identifier);

                expression_free (date);
            }
            else if (t == INSTR_IF)
                instruction_list_compute_dates (i->content.branch.true_body,
                        i->annotation.date, identifier);
            else if (t == INSTR_IF_ELSE)
            {
                instruction_list_compute_dates (i->content.branch.true_body,
                        i->annotation.date, identifier);
                instruction_list_compute_dates (i->content.branch.false_body,
                        i->annotation.date, identifier);
            }
            else
                instruction_list_compute_dates (i->content.block,
                        i->annotation.date, NULL);
        }
    }

    expression_free (advance_count);
    expression_free (advances);
}

instruction_list * instruction_list_n (instruction_list * list, size_t position)
{
    instruction_list * current = list;

    for (size_t i = 0; i < position; ++i)
        if (current != NULL)
        {
            current = current->next;
        }

    return current;
}

void instruction_list_wrap (instruction_list * list, instruction * instr,
        instruction_type t)
{
    instruction_list * nth = instruction_list_find_parent (list, instr);

    if (nth != NULL && nth->element->type != t)
    {
        instruction_list * wrapped = instruction_list_alloc ();
        wrapped->element = nth->element;
        wrapped->next = NULL;

        instruction * wrapper = instruction_alloc ();
        wrapper->type = t;
        wrapper->content.block = wrapped;

        nth->element = wrapper;
    }
}

bool instruction_list_is_indirect_parent (instruction_list * list,
        instruction * instr)
{
    bool success = false;
    for (instruction_list * current = list; current != NULL && ! success;
            current = current->next)
    {
        success = current->element == instr;

        if (! success)
        {
            instruction_type t = current->element->type;
            if (t == INSTR_FOR)
                success = instruction_list_is_indirect_parent (
                        current->element->content.loop.body, instr);
            else if (t == INSTR_IF || t == INSTR_IF_ELSE)
            {
                success = instruction_list_is_indirect_parent (
                        current->element->content.branch.true_body, instr);
                if (! success && current->element->content.branch.has_else)
                    success = instruction_list_is_indirect_parent (
                            current->element->content.branch.false_body, instr);
            }
            else if (t != INSTR_CALL && t != INSTR_UNKNOWN && t != INSTR_ADVANCE)
                success = instruction_list_is_indirect_parent (
                        current->element->content.block, instr);
        }
    }

    return success;
}

instruction_list * instruction_list_find_parent (instruction_list * list,
        instruction * instr)
{
    instruction_list * parent = NULL;

    for (instruction_list * current = list; parent == NULL && current != NULL;
            current = current->next)
    {

        if (instruction_list_is_indirect_parent (current, instr))
        {
            /* Is the current element the actual indirect parent or is it a
             * predecessor of the indirect parent? */
            for (instruction_list * current_again = current;
                    current_again != NULL; current_again = current_again->next)
            {
                if (instruction_list_is_indirect_parent (current_again, instr))
                {
                    parent = current_again;
                }
            }
        }
    }

    return parent;
}

void instruction_list_fill (instruction_list * list, instruction_list * calls)
{
    for (instruction_list * current = calls; current != NULL;
            current = current->next)
    {
        expression_list * expressions =
            current->element->content.call.arguments;

        bool coord = false;
        bool stop = false;
        long int previous_coord = -1;
        instruction_list * scope = list;

        for (expression_list * current_expr = expressions;
                current_expr != NULL && ! stop; current_expr = current_expr->next)
        {
            expression * expr = current_expr->element;
            if (! coord)
            {
                scope = instruction_list_find_parent (scope, current->element);

                if (previous_coord != -1)
                {
                    if (expr->type == EXPR_ID)
                    {
                        instruction_list * to_wrap = instruction_body (
                                scope->element);

                        if (scope->element->type == INSTR_IF
                            && scope->element->content.branch.has_else)
                        {
                            if (instruction_list_is_indirect_parent (
                                scope->element->content.branch.false_body,
                                current->element))
                            {
                                to_wrap =
                                    scope->element->content.branch.false_body;
                            }
                        }

                        if (to_wrap != NULL)
                        {
                            bool wrapped = false;
                            if (! strcmp (expr->content.identifier, "f"))
                            {
                                instruction_list_wrap (to_wrap, current->element,
                                        INSTR_FINISH);
                                wrapped = true;
                            }
                            else if (! strcmp (expr->content.identifier, "a"))
                            {
                                instruction_list_wrap (to_wrap, current->element,
                                        INSTR_ASYNC);
                                wrapped = true;
                            }

                            if (wrapped)
                            {
                                to_wrap = instruction_list_find_parent (to_wrap,
                                        current->element);
                                scope = to_wrap->element->content.block;

                                if (scope->element == current->element)
                                    stop = true;
                            }
                        }
                    }
                }
            }
            else
            {
                previous_coord = expr->content.number;
            }
            coord = ! coord;

            size_t list_size = expression_list_size (current_expr);
            if (list_size <= 3)
                stop = true;
        }
    }
}

void instruction_list_strip (instruction_list * list, string_list * s)
{
    for (instruction_list * current = list; current != NULL;
            current = current->next)
    {
        current->element->content.call.arguments =
            expression_list_strip
                (current->element->content.call.arguments);

        size_t n = expression_list_size (
                current->element->content.call.arguments);
        n = n > 2 ? n - 2 : 0;
        expression_list * new_last = expression_list_n (
                current->element->content.call.arguments, n);

        if (new_last != NULL && new_last->next != NULL
            && new_last->next->element != NULL
            && new_last->next->element->type == EXPR_NUMBER)
        {
            free (current->element->content.call.identifier);
            ssize_t place = new_last->next->element->content.number;
            current->element->content.call.identifier =
                strdup (string_list_parameter (s, place));
            expression_list_free (new_last->next);
            new_last->next = NULL;
        }
    }
}

size_t instruction_list_size (instruction_list * list)
{
    size_t i = 0;
    for (instruction_list * current = list; current != NULL;
            current = current->next)
        ++i;
    return i;
}


instruction_list * call_list (instruction_list * ast)
{
    instruction_list * list = NULL;

    for (instruction_list * current = ast; current != NULL;
            current = current->next)
    {
        instruction_type t = current->element->type;
        if (t == INSTR_CALL)
        {
            instruction_list * current_instr = instruction_list_alloc ();
            current_instr->element = current->element;
            current_instr->next = NULL;
            list = instruction_list_cat (list, current_instr);
        }
        else if (t == INSTR_FOR)
        {
            instruction_list * current_instr =
                call_list (current->element->content.loop.body);
            list = instruction_list_cat (list, current_instr);
        }
        else if (t == INSTR_IF || t == INSTR_IF_ELSE)
        {
            instruction_list * current_instr =
                call_list (current->element->content.branch.true_body);
            list = instruction_list_cat (list, current_instr);
            if (current->element->content.branch.has_else)
            {
                current_instr =
                    call_list (current->element->content.branch.false_body);
                list = instruction_list_cat (list, current_instr);
            }
        }
        else if (t != INSTR_UNKNOWN)
        {
            instruction_list * current_instr =
                call_list (current->element->content.block);
            list = instruction_list_cat (list, current_instr);
        }
    }

    return list;
}

