/**
 * \file expression.c
 * \brief Expressions.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * Conversion ::expression -> const char *
 * --------------------------------------------
 *
 * The conversion of ::expression into strings relies on an array of string
 * representations for ::expression_type.
 *
 * The array #expression_type_strings should be edited with care whenever
 * the ::expression_type enumeration is modified: all values lower than
 * ::EXPR_UNKNOWN should have a corresponding string in this array. (This
 * is also the reason why ::EXPR_UNKNOWN should always be the last value
 * of the enumeration, as this makes various checks much easier...)
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

#include "noclock/expression.h"

////////////////////////////////////////////////////////////////////////////////
// Static variables.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief The expression type string representations.
 * \since version `1.0.0`
 *
 * \warning Do not forget to edit this array accordingly when adding new
 * values (or modifying them) to the ::expression_type enum!
 */
static const char * expression_type_strings[] =
{
    /* Binary boolean expressions. */
    [EXPR_OR]       = "||",
    [EXPR_AND]      = "&&",
    [EXPR_LT]       = "<",
    [EXPR_GT]       = ">",
    [EXPR_EQ]       = "==",
    [EXPR_NE]       = "!=",
    [EXPR_LE]       = "<=",
    [EXPR_GE]       = ">=",

    /* Binary arithmetic expressions. */
    [EXPR_ADD]      = "+",
    [EXPR_SUB]      = "-",
    [EXPR_MULT]     = "*",
    [EXPR_DIV]      = "/",
    [EXPR_MIN]      = "min",
    [EXPR_MAX]      = "max",

    /* Unary boolean expressions. */
    [EXPR_NOT]      = "!",

    /* Unary arithmetic expressions. */
    [EXPR_NEG]      = "-",

    /* Misc. */
    [EXPR_ID]       = "id",
    [EXPR_NUMBER]   = "number",

    /* Boolean constants. */
    [EXPR_TRUE]     = "true",
    [EXPR_FALSE]    = "false",

    /* Assuming EXPR_UNKNOWN is the last value, the size of this array is
     * EXPR_UNKNOWN.
     * Please make sure all other indexes have a string: this way, one only has
     * to ensure that the index is lower or equal to EXPR_UNKNOWN when using
     * this array.
     */
    [EXPR_UNKNOWN]  = "????",
};

////////////////////////////////////////////////////////////////////////////////
// Static functions declarations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Keep the first expression, free the second one.
 * \since version `1.0.0`
 *
 * \param keep The expression to keep.
 * \param ditch The expression to ditch.
 * \return The kept expression.
 */
static inline expression * keep_first (expression * keep, expression * ditch);

/**
 * \brief Keep the second expression, free the first one.
 * \since version `1.0.0`
 *
 * \param ditch The expression to ditch.
 * \param keep The expression to keep.
 * \return The kept expression.
 */
static inline expression * ditch_first (expression * ditch, expression * keep);

/**
 * \brief Function to execute when both expressions are number.
 * \since version `1.0.0`
 *
 * \param a First operand.
 * \param b Second operand.
 * \param t Operation.
 * \return The resulting expression.
 */
static inline expression * both_numbers (expression * a, expression * b,
        expression_type t);

/**
 * \brief Set up a binary expression.
 * \since version `1.0.0`
 *
 * \param e Target expression.
 * \param a First operand.
 * \param b Second operand.
 * \param t Binary operation.
 * \return The resulting expression.
 */
static inline void expression_set_binary (expression * e, expression * left,
        expression * right, expression_type t);

/**
 * \brief Attempt to fold expressions.
 * \since version `1.0.0`
 *
 * \param number Number to fold.
 * \param target Target expression.
 * \param t Operation.
 * \retval true if the fold succeeded.
 * \retval false otherwise.
 */
static bool attempt_to_fold (long int number, expression * target,
        expression_type t);

/**
 * \brief Attempt to fold, set up a binary expression in case of failure.
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \param t Operation.
 * \return The resulting expression.
 */
static expression * fold_or_operation (expression * a, expression * b,
        expression_type t);

/**
 * \brief Convert an expression type to a string.
 * \since version `1.0.0`
 *
 * \param t Type
 * \return String representation of the type.
 */
static inline const char * expression_type_to_string (expression_type t);

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

void expression_init (expression * e)
{
    if (e == NULL)
        return;

    e->type = EXPR_UNKNOWN;
    e->content.operands.left = NULL;
    e->content.operands.right = NULL;
}

void expression_clean (expression * e)
{
    if (e == NULL)
        return;

    switch (e->type)
    {
        /* Binary boolean expressions. */
        case EXPR_OR:
        case EXPR_AND:
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_EQ:
        case EXPR_NE:
        case EXPR_LE:
        case EXPR_GE:

        /* Binary arithmetic expressions. */
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MULT:
        case EXPR_DIV:
        case EXPR_MIN:
        case EXPR_MAX:
            expression_free (e->content.operands.right);
            /* No break here! We need to free the left operand...which is
             * also needs to be done for unary operations!
             */

        /* Unary boolean expressions. */
        case EXPR_NOT:

        /* Unary arithmetic expressions. */
        case EXPR_NEG:
            expression_free (e->content.operands.left);
            break;

        /* Misc. */
        case EXPR_ID:
            free (e->content.identifier);
            break;

        case EXPR_NUMBER:

        /* Boolean constants. */
        case EXPR_TRUE:
        case EXPR_FALSE:

        /* Default. */
        case EXPR_UNKNOWN:
        default:
            break;
    }

    /* Reset the expression. */
    expression_init (e);
}

expression * expression_alloc (void)
{
    expression * e = malloc (sizeof * e);
    __forbid_value (e, NULL, "malloc", EX_OSERR);
    expression_init (e);

    return e;
}

void expression_free (expression * e)
{
    expression_clean (e);
    free (e);
}

expression * expression_copy (const expression * const e)
{
    if (e == NULL)
        return NULL;

    expression * copy = expression_alloc ();
    copy->type = e->type;

    switch (copy->type)
    {
        /* Binary boolean expressions. */
        case EXPR_OR:
        case EXPR_AND:
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_EQ:
        case EXPR_NE:
        case EXPR_LE:
        case EXPR_GE:

        /* Binary arithmetic expressions. */
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MULT:
        case EXPR_DIV:
        case EXPR_MIN:
        case EXPR_MAX:
            copy->content.operands.right =
                expression_copy (e->content.operands.right);
            /* No break here! We need to copy the left operand...which is
             * also needs to be done for unary operations!
             */

        /* Unary boolean expressions. */
        case EXPR_NOT:

        /* Unary arithmetic expressions. */
        case EXPR_NEG:
            copy->content.operands.left =
                expression_copy (e->content.operands.left);
            break;

        /* Misc. */
        case EXPR_ID:
            copy->content.identifier = strdup (e->content.identifier);
            break;

        case EXPR_NUMBER:
            copy->content.number = e->content.number;

        /* Boolean constants. */
        case EXPR_TRUE:
        case EXPR_FALSE:

        /* Default. */
        case EXPR_UNKNOWN:
        default:
            break;
    }

    return copy;
}

////////////////////////////////////////////////////////////////////////////////
// Special creation.
////////////////////////////////////////////////////////////////////////////////

expression * expression_from_number (long int number)
{
    expression * e = expression_alloc ();
    expression_set_type (e, EXPR_NUMBER);
    expression_set_number (e, number);

    return e;
}

expression * expression_from_identifier (const char * const identifier)
{
    expression * e = expression_alloc ();
    expression_set_type (e, EXPR_ID);
    expression_set_identifier (e, identifier);

    return e;
}

expression * expression_from_boolean (bool boolean)
{
    expression * e = expression_alloc ();
    expression_set_boolean (e, boolean);

    return e;
}

////////////////////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////////////////////

void expression_set_type (expression * const e, expression_type t)
{
    if (e == NULL)
        return;

    e->type = t;
}

void expression_set_number (expression * const e, long int number)
{
    if (e == NULL)
        return;

    e->type = EXPR_NUMBER;
    e->content.number = number;
}

void expression_set_identifier (expression * const e, const char * identifier)
{
    if (e == NULL)
        return;

    e->type = EXPR_ID;
    e->content.identifier = strdup (identifier);
}

void expression_set_boolean (expression * const e, bool boolean)
{
    if (e == NULL)
        return;

    e->type = boolean ? EXPR_TRUE : EXPR_FALSE;
}

void expression_set_left_operand (expression * const e, expression * const left)
{
    if (e == NULL)
        return;

    e->content.operands.left = left;
}

void expression_set_right_operand (expression * const e, expression * const right)
{
    if (e == NULL)
        return;

    e->content.operands.right = right;
}

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

expression_type expression_get_type (const expression * e)
{
    if (e == NULL)
        return EXPR_UNKNOWN;

    return e->type;
}

bool expression_is_number (const expression * const e)
{
    if (e == NULL)
        return false;

    return e->type == EXPR_NUMBER;
}

bool expression_is_boolean (const expression * const e)
{
    if (e == NULL)
        return false;

    return e->type == EXPR_TRUE || e->type == EXPR_FALSE;
}

bool expression_is_identifier (const expression * const e)
{
    if (e == NULL)
        return false;

    return e->type == EXPR_ID;
}

long int expression_get_number (const expression * const e)
{
    return e->content.number;
}

const char * expression_get_identifier (const expression * const e)
{
    return e->content.identifier;
}

bool expression_get_boolean (const expression * const e)
{
    return e->type == EXPR_TRUE;
}

expression * expression_get_left (const expression * const e)
{
    if (e == NULL)
        return NULL;

    return e->content.operands.left;
}

expression * expression_get_right (const expression * const e)
{
    if (e == NULL)
        return NULL;

    return e->content.operands.right;
}

bool expression_is_zero (const expression * const e)
{
    if (e == NULL)
        return false;
    if (e->type != EXPR_NUMBER)
        return false;

    return e->content.number == 0;
}

bool expression_is_one (const expression * const e)
{
    if (e == NULL)
        return false;
    if (e->type != EXPR_NUMBER)
        return false;

    return e->content.number == 1;
}

bool expression_is_true (const expression * const e)
{
    if (e == NULL)
        return false;

    return e->type == EXPR_TRUE;
}

bool expression_is_false (const expression * const e)
{
    if (e == NULL)
        return false;

    return e->type == EXPR_FALSE;
}

////////////////////////////////////////////////////////////////////////////////
// Operations.
////////////////////////////////////////////////////////////////////////////////

expression * expression_neg (expression * e)
{
    if (e == NULL)
        return NULL;

    expression * result = expression_alloc ();

    expression_set_type (result, EXPR_NEG);
    expression_set_left_operand (result, e);

    return result;
}

expression * expression_not (expression * e)
{
    if (e == NULL)
        return NULL;

    expression * result = expression_alloc ();

    expression_set_type (result, EXPR_NOT);
    expression_set_left_operand (result, e);

    return result;
}

expression * expression_add (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = NULL;

    if (expression_is_zero (a))
        result = ditch_first (a, b);
    else if (expression_is_zero (b))
        result = keep_first (a, b);
    else if (expression_is_number (a) && expression_is_number (b))
        result = both_numbers (a, b, EXPR_ADD);
    else
        result = fold_or_operation (a, b, EXPR_ADD);

    return result;
}

expression * expression_sub (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = NULL;

    if (expression_is_zero (b))
        result = keep_first (a, b);
    else if (expression_is_number (a) && expression_is_number (b))
        result = both_numbers (a, b, EXPR_SUB);
    else
    {
        result = expression_alloc ();
        expression_set_binary (result, a, b, EXPR_SUB);
    }

    return result;
}

expression * expression_mult (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = NULL;

    if (expression_is_zero (a))
        result = keep_first (a, b);
    else if (expression_is_zero (b))
        result = ditch_first (a, b);
    else if (expression_is_one (a))
        result = ditch_first (a, b);
    else if (expression_is_one (b))
        result = keep_first (a, b);
    else if (expression_is_number (a) && expression_is_number (b))
        result = both_numbers (a, b, EXPR_MULT);
    else
        result = fold_or_operation (a, b, EXPR_MULT);

    return result;
}

expression * expression_div (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = NULL;

    if (expression_is_zero (a))
        result = keep_first (a, b);
    else if (expression_is_one (b))
        result = keep_first (a, b);
    else if (expression_is_number (a) && expression_is_number (b))
        result = both_numbers (a, b, EXPR_DIV);
    else
    {
        result = expression_alloc ();
        expression_set_binary (result, a, b, EXPR_DIV);
    }

    return result;
}

expression * expression_min (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = NULL;

    if (expression_is_number (a) && expression_is_number (b))
        result = both_numbers (a, b, EXPR_MIN);
    else
    {
        result = expression_alloc ();
        expression_set_binary (result, a, b, EXPR_MIN);
    }

    return result;
}

expression * expression_max (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = NULL;

    fprintf (stderr, "\n\n\x1B[1m\x1B[31mMAX MAX MAX\x1B[0m\n\n");
    if (expression_is_number (a) && expression_is_number (b))
        result = both_numbers (a, b, EXPR_MAX);
    else
    {
        result = expression_alloc ();
        expression_set_binary (result, a, b, EXPR_MAX);
    }

    return result;
}

expression * expression_and (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_AND);

    return result;
}

expression * expression_or (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_OR);

    return result;
}

expression * expression_lt (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_LT);

    return result;
}

expression * expression_le (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_LE);

    return result;
}

expression * expression_gt (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_GT);

    return result;
}

expression * expression_ge (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_GE);

    return result;
}

expression * expression_eq (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_EQ);

    return result;
}

expression * expression_ne (expression * a, expression * b)
{
    if (a == NULL)
        return b == NULL ? NULL : b;
    else if (b == NULL)
        return a;

    expression * result = expression_alloc ();
    expression_set_binary (result, a, b, EXPR_NE);

    return result;
}

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

void expression_fprint (FILE * f, const expression * e)
{
    if (e == NULL)
        return;

    char * expression_string = expression_to_string (e);
    fprintf (f, "%s", expression_string);
    free (expression_string);
}

void expression_print (const expression * e)
{
    expression_fprint (stdout, e);
}

char * expression_to_string (const expression * e)
{
    /* This should be the default value for string allocations and margins. */
    #define MARGIN 64

    char * string = NULL;
    if (e == NULL)
    {
        string = malloc (MARGIN);
        memset (string, 0, MARGIN);
        return string;
    }

    char * left, * right;
    left = right = NULL;

    expression_type t = e->type;
    const char * type_string = expression_type_to_string (t);

    switch (t)
    {
        /* Binary boolean expressions. */
        case EXPR_OR:
        case EXPR_AND:
        case EXPR_LT:
        case EXPR_GT:
        case EXPR_EQ:
        case EXPR_NE:
        case EXPR_LE:
        case EXPR_GE:

        /* Binary arithmetic expressions. */
        case EXPR_ADD:
        case EXPR_SUB:
        case EXPR_MULT:
        case EXPR_DIV:
            left = expression_to_string (expression_get_left (e));
            right = expression_to_string (expression_get_right (e));

            string = malloc (strlen (left) + strlen (right) + MARGIN);
            __forbid_value (string, NULL, "malloc", EX_OSERR);

            sprintf (string, "(%s %s %s)", left, type_string, right);

            free (left);
            free (right);

            break;

        case EXPR_MIN:
        case EXPR_MAX:
            left = expression_to_string (expression_get_left (e));
            right = expression_to_string (expression_get_right (e));

            string = malloc (strlen (left) + strlen (right) + MARGIN);
            __forbid_value (string, NULL, "malloc", EX_OSERR);

            sprintf (string, "%s (%s, %s)", type_string, left, right);

            free (left);
            free (right);

            break;

        /* Unary boolean expressions. */
        case EXPR_NOT:

        /* Unary arithmetic expressions. */
        case EXPR_NEG:
            left = expression_to_string (expression_get_left (e));

            string = malloc (strlen (left) + MARGIN);
            __forbid_value (string, NULL, "malloc", EX_OSERR);

            sprintf (string, "%s%s", type_string, left);

            free (left);

            break;

        /* Misc. */
        case EXPR_ID:
            string = malloc (strlen (expression_get_identifier (e)) + MARGIN);
            __forbid_value (string, NULL, "malloc", EX_OSERR);

            if (pretty_print_colour_state ())
                sprintf (string, PP_YELLOW "%s" PP_RESET,
                        expression_get_identifier (e));
            else
                sprintf (string, "%s", expression_get_identifier (e));

            break;

        case EXPR_NUMBER:
            string = malloc (MARGIN);
            __forbid_value (string, NULL, "malloc", EX_OSERR);

            if (pretty_print_colour_state ())
                sprintf (string, PP_CONSTANT "%ld" PP_RESET,
                        expression_get_number (e));
            else
                sprintf (string, "%ld", expression_get_number (e));

            break;

        /* Boolean constants. */
        case EXPR_TRUE:
        case EXPR_FALSE:
            string = malloc (strlen (type_string) + MARGIN);
            __forbid_value (string, NULL, "malloc", EX_OSERR);

            if (pretty_print_colour_state ())
                sprintf (string, PP_CONSTANT "%s" PP_RESET, type_string);
            else
                sprintf (string, "%s", type_string);

            break;

        /* Default. */
        case EXPR_UNKNOWN:
        default:
            break;
    }

    return string;

    #undef MARGIN
}

////////////////////////////////////////////////////////////////////////////////
// Static functions definitions.
////////////////////////////////////////////////////////////////////////////////

expression * keep_first (expression * keep, expression * ditch)
{
    free (ditch);
    return keep;
}

expression * ditch_first (expression * ditch, expression * keep)
{
    free (ditch);
    return keep;
}

static inline expression * both_numbers (expression * a, expression * b,
        expression_type t)
{
    switch (t)
    {
        case EXPR_ADD:
            a->content.number += b->content.number;
            break;
        case EXPR_SUB:
            a->content.number -= b->content.number;
            break;
        case EXPR_MULT:
            a->content.number *= b->content.number;
            break;
        case EXPR_DIV:
            a->content.number /= b->content.number;
            break;
        case EXPR_MIN:
            a->content.number = a->content.number < b->content.number ?
                a->content.number : b->content.number;
            break;
        case EXPR_MAX:
            a->content.number = a->content.number > b->content.number ?
                a->content.number : b->content.number;
            break;
        default:
            break;
    }

    expression_free (b);

    return a;
}

void expression_set_binary (expression * e, expression * left,
        expression * right, expression_type t)
{
    expression_set_type (e, t);
    expression_set_left_operand (e, left);
    expression_set_right_operand (e, right);
}

bool attempt_to_fold (long int number, expression * target,
        expression_type t)
{
    bool folded = false;

    expression * left = expression_get_left (target);
    expression * right = expression_get_right (target);

    if (t == target->type)
    {
        if (expression_is_number (left))
        {
            if (t == EXPR_ADD)
                left->content.number += number;
            else if (t == EXPR_MULT)
                left->content.number *= number;
            folded = true;
        }
        else
            folded = attempt_to_fold (number, left, t);

        if (folded)
            return folded;

        if (expression_is_number (right))
        {
            if (t == EXPR_ADD)
                right->content.number += number;
            else if (t == EXPR_MULT)
                right->content.number *= number;
            folded = true;
        }
        else
            folded = attempt_to_fold (number, right, t);
    }

    return folded;
}

expression * fold_or_operation (expression * a, expression * b,
        expression_type t)
{
    expression * result = NULL;
    bool folded = false;

    if (expression_is_number (a))
        folded = attempt_to_fold (expression_get_number (a), b, t);
    else if (expression_is_number (b))
        folded = attempt_to_fold (expression_get_number (b), a, t);

    if (folded)
    {
        result = expression_is_number (a) ? b : a;
        free (result == a ? b : a);
    }
    else
    {
        result = expression_alloc ();
        expression_set_binary (result, a, b, t);
    }

    return result;
}

const char * expression_type_to_string (expression_type t)
{
    if (t < 0 || t > EXPR_UNKNOWN)
        return expression_type_strings[EXPR_UNKNOWN];

    return expression_type_strings[t];
}
