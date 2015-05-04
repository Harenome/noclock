/**
 * \file instruction.c
 * \brief Instructions
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

#include "noclock/instruction.h"

////////////////////////////////////////////////////////////////////////////////
// Extern functions.
////////////////////////////////////////////////////////////////////////////////

/* These functions should be found in "noclock/instruction_list.h". */

extern instruction_list * instruction_list_alloc (void);
extern void instruction_list_free (instruction_list *);
extern size_t instruction_list_size (instruction_list *);
extern void instruction_list_fprint (FILE *, const instruction_list *);

////////////////////////////////////////////////////////////////////////////////
// Static functions declarations.
////////////////////////////////////////////////////////////////////////////////

static const char * instruction_type_to_string (instruction_type t);

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

instruction * instruction_alloc (void)
{
    instruction * i = malloc (sizeof * i);
    if (i == NULL)
    {
        perror ("malloc");
        exit (EX_OSERR);
    }
    memset (i, 0, sizeof * i);
    return i;
}

void instruction_free (instruction  * i)
{
    switch (i->type)
    {
        case INSTR_CALL:
            free (i->content.call.identifier);
            expression_list_free (i->content.call.arguments);
            break;
        case INSTR_FOR:
            free (i->content.loop.identifier);
            expression_free (i->content.loop.left_boundary);
            expression_free (i->content.loop.right_boundary);
            instruction_list_free (i->content.loop.body);
            break;
        case INSTR_IF:
        case INSTR_IF_ELSE:
            expression_free (i->content.branch.condition);
            instruction_list_free (i->content.branch.true_body);
            if (i->content.branch.has_else)
                instruction_list_free (i->content.branch.false_body);
            break;
        case INSTR_ADVANCE:
            break;
        case INSTR_FINISH:
        case INSTR_ASYNC:
        case INSTR_CLOCKED_FINISH:
        case INSTR_CLOCKED_ASYNC:
            instruction_list_free (i->content.block);
            break;
        default:
            break;
    }

    free (i->annotation.level);
    free (i->annotation.boundaries);
    expression_free (i->annotation.date);

    free (i);
}

////////////////////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////////////////////

instruction * instruction_function_call (char * identifier,
        expression_list * arguments)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_CALL);
    instruction_function_call_set_identifier (i, identifier);
    instruction_function_call_set_arguments (i, arguments);

    return i;
}

instruction * instruction_for_loop (char * identifier, expression * left,
        expression * right, instruction_list * body)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_FOR);
    instruction_for_loop_set_identifier (i, identifier);
    instruction_for_loop_set_left_boundary (i, left);
    instruction_for_loop_set_right_boundary (i, right);
    instruction_for_loop_set_body (i, body);

    return i;
}

instruction * instruction_if_then_else (bool has_else, expression * condition,
        instruction_list * true_body, instruction_list * false_body)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_IF);
    instruction_if_then_else_set_has_else (i, has_else);
    instruction_if_then_else_set_condition (i, condition);
    instruction_if_then_else_set_true_body (i, true_body);
    instruction_if_then_else_set_false_body (i, false_body);

    return i;
}

instruction * instruction_advance (void)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_ADVANCE);

    return i;
}

instruction * instruction_finish (instruction_list * block)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_FINISH);
    instruction_other_set_block (i, block);

    return i;
}

instruction * instruction_async (instruction_list * block)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_ASYNC);
    instruction_other_set_block (i, block);

    return i;
}

instruction * instruction_clocked_finish (instruction_list * block)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_CLOCKED_FINISH);
    instruction_other_set_block (i, block);

    return i;
}

instruction * instruction_clocked_async (instruction_list * block)
{
    instruction * i = instruction_alloc ();

    instruction_set_type (i, INSTR_CLOCKED_ASYNC);
    instruction_other_set_block (i, block);

    return i;
}

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

instruction_type instruction_get_type (const instruction * instr)
{
    if (instr == NULL)
        return INSTR_UNKNOWN;

    return instr->type;
}

char * instruction_identifier (instruction * instr)
{
    instruction_type t = instruction_get_type (instr);

    switch (t)
    {
        case INSTR_CALL:
            return instruction_function_call_get_identifier (instr);
        case INSTR_FOR:
            return instruction_for_loop_get_identifier (instr);
        default:
            return NULL;
    }

    return NULL;
}

instruction_list * instruction_body (instruction * instr)
{
    instruction_type t = instruction_get_type (instr);

    switch (t)
    {
        case INSTR_FOR:
            return instruction_for_loop_get_body (instr);
        case INSTR_IF:
        case INSTR_IF_ELSE:
            return instruction_if_then_else_get_true_body (instr);
        case INSTR_FINISH:
        case INSTR_ASYNC:
        case INSTR_CLOCKED_FINISH:
        case INSTR_CLOCKED_ASYNC:
            return instruction_other_get_block (instr);
        default:
            return NULL;
    }

    return NULL;

}

char * instruction_function_call_get_identifier (instruction * instr)
{
    if (instr == NULL)
        return NULL;

    return instr->content.call.identifier;
}

expression_list * instruction_function_call_get_arguments (instruction * instr)
{
    if (instr == NULL)
        return NULL;

    return instr->content.call.arguments;
}

char * instruction_for_loop_get_identifier (instruction * instr)
{
    if (instr == NULL)
        return NULL;

    return instr->content.loop.identifier;
}

expression * instruction_for_loop_get_left_boundary (instruction * instr)
{
    if (instr == NULL)
        return NULL;

    return instr->content.loop.left_boundary;
}

expression * instruction_for_loop_get_right_boundary (instruction * instr)
{
    if (instr == NULL)
        return NULL;

    return instr->content.loop.right_boundary;
}

instruction_list * instruction_for_loop_get_body (instruction * instr)
{
    if (instr == NULL)
        return NULL;

    return instr->content.loop.body;
}

bool instruction_if_then_else_get_has_else (instruction * instr)
{
    if (instr == NULL)
        return false;

    return instr->content.branch.has_else;
}

expression * instruction_if_then_else_get_condition (instruction * instr)
{
    if (instr == NULL)
        return false;

    return instr->content.branch.condition;
}

instruction_list * instruction_if_then_else_get_true_body (instruction * instr)
{
    if (instr == NULL)
        return false;

    return instr->content.branch.true_body;
}

instruction_list * instruction_if_then_else_get_false_body (instruction * instr)
{
    if (instr == NULL)
        return false;

    return instr->content.branch.false_body;
}

instruction_list * instruction_other_get_block (instruction * instr)
{
    if (instr == NULL)
        return NULL;

    return instr->content.block;
}

////////////////////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////////////////////

void instruction_set_type (instruction * instr, instruction_type t)
{
    if (instr == NULL)
        return;

    instr->type = t;
}

void instruction_function_call_set_identifier (instruction * instr,
        char * identifier)
{
    if (instr == NULL)
        return;

    instr->content.call.identifier = identifier;
}

void instruction_function_call_set_arguments (instruction * instr,
        expression_list * arguments)
{
    if (instr == NULL)
        return;

    instr->content.call.arguments = arguments;
}

void instruction_for_loop_set_identifier (instruction * instr,
        char * identifier)
{
    if (instr == NULL)
        return;

    instr->content.loop.identifier = identifier;
}

void instruction_for_loop_set_left_boundary (instruction * instr,
        expression * left)
{
    if (instr == NULL)
        return;

    instr->content.loop.left_boundary = left;
}

void instruction_for_loop_set_right_boundary (instruction * instr,
        expression * right)
{
    if (instr == NULL)
        return;

    instr->content.loop.right_boundary = right;
}

void instruction_for_loop_set_body (instruction * instr,
        instruction_list * body)
{
    if (instr == NULL)
        return;

    instr->content.loop.body = body;
}

void instruction_if_then_else_set_has_else (instruction * instr, bool has_else)
{
    if (instr == NULL)
        return;

    instr->content.branch.has_else = has_else;
}

void instruction_if_then_else_set_condition (instruction * instr,
        expression * condition)
{
    if (instr == NULL)
        return;

    instr->content.branch.condition = condition;
}

void instruction_if_then_else_set_true_body (instruction * instr,
        instruction_list * true_body)
{
    if (instr == NULL)
        return;

    instr->content.branch.true_body = true_body;
}

void instruction_if_then_else_set_false_body (instruction * instr,
        instruction_list * false_body)
{
    if (instr == NULL)
        return;

    instr->content.branch.false_body = false_body;
}

void instruction_other_set_block (instruction * instr, instruction_list * block)
{
    if (instr == NULL)
        return;

    instr->content.block = block;
}
////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

void instruction_fprint (FILE * f, const instruction * instr)
{
    if (instr == NULL)
        return;

    instruction_type t = instr->type;

    switch (t)
    {
        case INSTR_CALL:
            pretty_print_indent_fprint (f);
            instruction_call_fprint (f, & instr->content.call);
            break;
        case INSTR_FOR:
            for_loop_fprint (f, & instr->content.loop);
            break;
        case INSTR_IF:
        case INSTR_IF_ELSE:
            if_then_else_fprint (f, & instr->content.branch);
            break;
        case INSTR_ADVANCE:
            pretty_print_indent_fprint (f);

            if (pretty_print_colour_state ())
                fprintf (f, PP_KEYWORD_NOT_WANTED "%s" PP_RESET,
                        instruction_type_to_string (t));
            else
                fprintf (f, "%s", instruction_type_to_string (t));

            break;
        case INSTR_CLOCKED_FINISH:
        case INSTR_CLOCKED_ASYNC:
            pretty_print_indent_fprint (f);

            if (pretty_print_colour_state ())
                fprintf (f, PP_KEYWORD_NOT_WANTED "clocked " PP_RESET);
            else
                fprintf (f, "clocked ");
            /* No break here! */
        case INSTR_FINISH:
        case INSTR_ASYNC:
            /* Check whether the finish or async was clocked!
                * (In which case the indent is already printed!)
                */
            if (t == INSTR_FINISH || t == INSTR_ASYNC)
                pretty_print_indent_fprint (f);

            /* Workaround for clocked versions of finish and async!
                * This could be dangerous, but we are pretty sure the
                * instruction_type_to_string() call below is the last time
                * we use the variable t, so it is fine to do this...
                */
            if (t == INSTR_CLOCKED_FINISH)
                t = INSTR_FINISH;
            else if (t == INSTR_CLOCKED_ASYNC)
                t = INSTR_ASYNC;

            if (pretty_print_colour_state ())
                fprintf (f, PP_KEYWORD_SPECIAL "%s" PP_RESET "\n",
                        instruction_type_to_string (t));
            else
                fprintf (f, "%s\n", instruction_type_to_string (t));

            if (instruction_list_size (instr->content.block) > 1)
            {
                pretty_print_indent_fprint (f);
                fprintf (f, "{\n");
            }

            pretty_print_indent_increase ();
            instruction_list_fprint (f, instr->content.block);
            pretty_print_indent_decrease ();

            if (instruction_list_size (instr->content.block) > 1)
            {
                pretty_print_indent_fprint (f);
                fprintf (f, "}\n");
            }
            break;
        default:
            break;
    }
}

void if_then_else_fprint (FILE * f, const if_then_else * instr)
{
    if (instr == NULL)
        return;

    pretty_print_indent_fprint (f);

    if (pretty_print_colour_state ())
        fprintf (f, PP_KEYWORD "if" PP_RESET " (");
    else
        fprintf (f, "if (");

    expression_fprint (f, instr->condition);
    fprintf (f, ")\n");

    bool print_braces = instruction_list_size (instr->true_body) > 1;

    if (print_braces)
    {
        pretty_print_indent_fprint (f);
        fprintf (f, "{\n");
    }

    pretty_print_indent_increase ();
    instruction_list_fprint (f, instr->true_body);
    pretty_print_indent_decrease ();

    if (print_braces)
    {
        pretty_print_indent_fprint (f);
        fprintf (f, "}\n");
    }

    if (instr->has_else)
    {
        pretty_print_indent_fprint (f);

        if (pretty_print_colour_state ())
            fprintf (f, PP_KEYWORD "else" PP_RESET "\n");
        else
            fprintf (f, "else\n");

        print_braces = instruction_list_size (instr->false_body) > 1;
        if (print_braces)
        {
            pretty_print_indent_fprint (f);
            fprintf (f, "{\n");
        }

        pretty_print_indent_increase ();
        instruction_list_fprint (f, instr->false_body);
        pretty_print_indent_decrease ();

        if (print_braces)
        {
            pretty_print_indent_fprint (f);
            fprintf (f, "}\n");
        }
    }
}

void for_loop_fprint (FILE * f, const for_loop * instr)
{
    if (instr == NULL)
        return;

    pretty_print_indent_fprint (f);
    if (pretty_print_colour_state ())
        fprintf (f, PP_KEYWORD "for" PP_RESET " %s " PP_KEYWORD "in" PP_RESET
                " (",
                instr->identifier);
    else
        fprintf (f, "for %s in (", instr->identifier);

    expression_fprint (f, instr->left_boundary);

    if (pretty_print_colour_state ())
        fprintf (f, PP_KEYWORD ".." PP_RESET);
    else
        fprintf (f, "..");

    expression_fprint (f, instr->right_boundary);
    fprintf (f, ")\n");

    if (instruction_list_size (instr->body) > 1)
    {
        pretty_print_indent_fprint (f);
        fprintf (f, "{\n");
    }

    pretty_print_indent_increase ();
    instruction_list_fprint (f, instr->body);
    pretty_print_indent_decrease ();

    if (instruction_list_size (instr->body) > 1)
    {
        pretty_print_indent_fprint (f);
        fprintf (f, "}\n");
    }
}

void instruction_call_fprint (FILE * f, const function_call * instr)
{
    if (instr == NULL)
        return;

    if (pretty_print_colour_state ())
        fprintf (f, PP_CALL "%s (" PP_RESET, instr->identifier);
    else
        fprintf (f, "%s (", instr->identifier);

    if (pretty_print_colour_state ())
        expression_list_fprint (f, instr->arguments,
                PP_CALL ", " PP_RESET);
    else
        expression_list_fprint (f, instr->arguments, ", ");

    if (pretty_print_colour_state ())
        fprintf (f, PP_CALL ")" PP_RESET);
    else
        fprintf (f, ")");

}

////////////////////////////////////////////////////////////////////////////////
// Static functions definitions.
////////////////////////////////////////////////////////////////////////////////

const char * instruction_type_to_string (instruction_type t)
{
    static const char * type_chars[] =
    {
        [INSTR_CALL]            = "",
        [INSTR_FOR]             = "",
        [INSTR_IF]              = "",
        [INSTR_IF_ELSE]         = "",
        [INSTR_ADVANCE]         = "advance",
        [INSTR_FINISH]          = "finish",
        [INSTR_ASYNC]           = "async",
        [INSTR_CLOCKED_FINISH]  = "clocked finish",
        [INSTR_CLOCKED_ASYNC]   = "clocked async",
        [INSTR_UNKNOWN]         = "",
    };

    return type_chars[t];
}
