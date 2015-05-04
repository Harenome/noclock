/**
 * \file isl_to_noclock.c
 * \brief ISL AST to noclock AST conversion.
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

#include "noclock/isl_to_noclock.h"

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

/** * \brief Convert a for loop.
 * \param for_node Input loop.
 * \return Output loop.
 */
static instruction_list * isl_for_to_noclock (isl_ast_node * for_node);

/**
 * \brief Convert a if then else branch.
 * \param for_node Input branch.
 * \return Output branch.
 */
static instruction_list * isl_if_to_noclock (isl_ast_node * if_node);

/**
 * \brief Convert a block.
 * \param for_node Input block.
 * \return Output block.
 */
static instruction_list * isl_block_to_noclock (isl_ast_node * block_node);

/**
 * \brief Convert a call.
 * \param for_node Input call.
 * \return Output call.
 */
static instruction_list * isl_user_to_noclock (isl_ast_node * user_node);

/**
 * \brief Convert an initialization.
 * \param for_node Input initialization.
 * \return Output initialization.
 */
static expression * isl_init_to_expr (isl_ast_expr * expr);

/**
 * \brief Convert a condition.
 * \param for_node Input condition.
 * \return Output condition.
 */
static expression * isl_cond_to_expr (isl_ast_expr * expr);

////////////////////////////////////////////////////////////////////////////////
// ISL -> No Clock.
////////////////////////////////////////////////////////////////////////////////

expression * isl_expr_to_noclock_expr (isl_ast_expr * expr)
{
    expression * e = NULL;

    enum isl_ast_expr_type expr_t = isl_ast_expr_get_type (expr);

    if (expr_t == isl_ast_expr_id)
    {
        e = expression_from_identifier (isl_id_get_name (
                    isl_ast_expr_get_id (expr)));

        return e;
    }
    else if (expr_t == isl_ast_expr_int)
    {
        e = expression_from_number (
                isl_val_get_num_si (isl_ast_expr_get_val (expr)));

        return e;
    }

    bool binary = false;
    enum isl_ast_op_type t = isl_ast_expr_get_op_type (expr);

    switch (t)
    {
        case isl_ast_op_max:
            e = expression_alloc ();
            expression_set_type (e, EXPR_MAX);
            binary = true;
            break;
        case isl_ast_op_min:
            e = expression_alloc ();
            expression_set_type (e, EXPR_MIN);
            binary = true;
            break;
        case isl_ast_op_minus:
            e = expression_alloc ();
            expression_set_type (e, EXPR_NEG);
            break;
        case isl_ast_op_add:
            e = expression_alloc ();
            expression_set_type (e, EXPR_ADD);
            binary = true;
            break;
        case isl_ast_op_sub:
            e = expression_alloc ();
            expression_set_type (e, EXPR_SUB);
            binary = true;
            break;
        case isl_ast_op_mul:
            e = expression_alloc ();
            expression_set_type (e, EXPR_MULT);
            binary = true;
            break;
        case isl_ast_op_div:
        case isl_ast_op_fdiv_q:
        case isl_ast_op_pdiv_q:
        case isl_ast_op_pdiv_r:
            e = expression_alloc ();
            expression_set_type (e, EXPR_DIV);
            binary = true;
            break;
        case isl_ast_op_member:
        case isl_ast_op_cond:
        case isl_ast_op_select:
            return e;
            break;
        case isl_ast_op_eq:
            e = expression_alloc ();
            expression_set_type (e, EXPR_EQ);
            binary = true;
            break;
        case isl_ast_op_le:
            e = expression_alloc ();
            expression_set_type (e, EXPR_LE);
            binary = true;
            break;
        case isl_ast_op_lt:
            e = expression_alloc ();
            expression_set_type (e, EXPR_LT);
            binary = true;
            break;
        case isl_ast_op_ge:
            e = expression_alloc ();
            expression_set_type (e, EXPR_GE);
            binary = true;
            break;
        case isl_ast_op_gt:
            e = expression_alloc ();
            expression_set_type (e, EXPR_GT);
            binary = true;
            break;
        case isl_ast_op_and:
        case isl_ast_op_and_then:
            e = expression_alloc ();
            expression_set_type (e, EXPR_AND);
            binary = true;
            break;
        case isl_ast_op_or:
        case isl_ast_op_or_else:
            e = expression_alloc ();
            expression_set_type (e, EXPR_OR);
            binary = true;
            break;
        case isl_ast_op_call:
        case isl_ast_op_access:
        case isl_ast_op_address_of:
        default:
            return e;
            break;
    }

    if (binary)
    {
        expression_set_left_operand (e,
                isl_expr_to_noclock_expr (isl_ast_expr_get_op_arg (expr, 0)));
        expression_set_right_operand (e,
                isl_expr_to_noclock_expr (isl_ast_expr_get_op_arg (expr, 1)));
    }
    else
    {
        expression_set_left_operand (e,
                isl_expr_to_noclock_expr (isl_ast_expr_get_op_arg (expr, 0)));
    }

    return e;
}

instruction_list * isl_ast_to_noclock_ast (isl_ast_node * ast)
{
    enum isl_ast_node_type t = isl_ast_node_get_type (ast);

    instruction_list * list = NULL;
    switch (t)
    {
        case isl_ast_node_for:
            list = isl_for_to_noclock (ast);
            break;
        case isl_ast_node_if:
            list = isl_if_to_noclock (ast);
            break;
        case isl_ast_node_block:
            list = isl_block_to_noclock (ast);
            break;
        case isl_ast_node_user:
            list = isl_user_to_noclock (ast);
            break;
        default:
            fdebug (stderr, "isl_ast_to_noclock_ast(): "
                    "Unexpected node_type: %u\n", t);
            break;
    }

    return list;
}

////////////////////////////////////////////////////////////////////////////////
// Static function definitions.
////////////////////////////////////////////////////////////////////////////////

instruction_list * isl_for_to_noclock (isl_ast_node * for_node)
{
    /* Extract the for loop information. */
    isl_ast_expr * iterator = isl_ast_node_for_get_iterator (for_node);
    isl_id * id = isl_ast_expr_get_id (iterator);
    isl_ast_expr * init = isl_ast_node_for_get_init (for_node);
    isl_ast_expr * cond = isl_ast_node_for_get_cond (for_node);
    isl_ast_node * body = isl_ast_node_for_get_body (for_node);


    /* Construct the for loop. */
    instruction * loop = instruction_for_loop (
            strdup (isl_id_get_name (id)),
            isl_init_to_expr (init),
            isl_cond_to_expr (cond),
            isl_ast_to_noclock_ast (body));

    /* Wrap the loop in an instruction list node. */
    instruction_list * list = instruction_list_alloc ();
    list->element = loop;
    list->next = NULL;

    return list;
}

instruction_list * isl_if_to_noclock (isl_ast_node * if_node)
{
    /* Extract the if then else information. */
    isl_ast_expr * cond = isl_ast_node_if_get_cond (if_node);
    isl_ast_node * if_body = isl_ast_node_if_get_then (if_node);
    bool has_else = isl_ast_node_if_has_else (if_node);

    instruction * if_i = instruction_alloc ();
    if_i->type = INSTR_IF;
    if_i->content.branch.condition = isl_expr_to_noclock_expr (cond);
    if_i->content.branch.true_body = isl_ast_to_noclock_ast (if_body);
    if_i->content.branch.false_body = NULL;

    if (has_else)
    {
        isl_ast_node * else_body = isl_ast_node_if_get_else (if_node);
        if_i->content.branch.false_body =
            isl_ast_to_noclock_ast (else_body);
        if_i->content.branch.has_else = true;
    }

    /* Wrap the if then else in an instruction list node. */
    instruction_list * list = instruction_list_alloc ();
    list->element = if_i;
    list->next = NULL;

    return list;
}

instruction_list * isl_block_to_noclock (isl_ast_node * block_node)
{
    isl_ast_node_list * children = isl_ast_node_block_get_children (block_node);
    int n = isl_ast_node_list_n_ast_node (children);

    instruction_list * list = NULL;

    for (int i = 0; i < n; ++i)
    {
        isl_ast_node * current = isl_ast_node_list_get_ast_node (children, i);
        instruction_list * instr = isl_ast_to_noclock_ast (current);
        list = instruction_list_cat (list, instr);
    }

    return list;
}

instruction_list * isl_user_to_noclock (isl_ast_node * user_node)
{
    isl_ast_expr * expr = isl_ast_node_user_get_expr (user_node);

    instruction * user = instruction_alloc ();
    user->type = INSTR_CALL;
    user->content.call.identifier = strdup (isl_id_get_name (
                isl_ast_expr_get_id (isl_ast_expr_get_op_arg (expr, 0))));

    for (int i = 1; i < isl_ast_expr_get_op_n_arg (expr); ++i)
    {
        expression_list * e = expression_list_alloc ();
        e->element = isl_expr_to_noclock_expr (isl_ast_expr_get_op_arg (expr, i));
        e->next = NULL;
        user->content.call.arguments = expression_list_cat (
                user->content.call.arguments, e);
    }

    instruction_list * list = instruction_list_alloc ();
    list->element = user;
    list->next = NULL;

    return list;
}

expression * isl_init_to_expr (isl_ast_expr * expr)
{
    return isl_expr_to_noclock_expr (expr);
}

expression * isl_cond_to_expr (isl_ast_expr * expr)
{
    expression * result;
    enum isl_ast_op_type t = isl_ast_expr_get_op_type (expr);

    result = isl_expr_to_noclock_expr (isl_ast_expr_get_op_arg (expr, 1));

    if (t == isl_ast_op_lt)
    {
        expression * minus_one = expression_alloc ();
        minus_one->type = EXPR_NUMBER;
        minus_one->content.number = 1;

        expression * new_result = expression_sub (result, minus_one);

        result = new_result;
    }

    return result;
}
