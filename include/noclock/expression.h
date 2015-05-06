/**
 * \file expression.h
 * \brief Expressions.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * This file declares expressions utilities.
 *
 * For further information, see the \ref expression_group module.
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

#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sysexits.h>
#include <string.h>

#include "noclock/util.h"
#include "noclock/pretty_print.h"

/**
 * \defgroup expression_global_group Expressions
 * \brief ::expression handling.
 * \since version `1.0.0`
 *
 * For further information on:
 *
 * - expressions, see \ref expression_group
 * - lists of expressions, see \ref expression_list_group
 */

/**
 * \defgroup expression_group Expressions
 * \ingroup expression_global_group
 * \brief Arithmetic or boolean ::expression `struct`s.
 * \since version `1.0.0`
 *
 * This module declares the ::expression_type `enum` and the ::expression
 * `struct`.
 *
 * An expression can be:
 *
 * - an identifier (its type would be ::EXPR_ID).
 * - a number (its type would be ::EXPR_NUMBER).
 * - a boolean value (its type would be either ::EXPR_TRUE or ::EXPR_FALSE).
 * - an unary operation on another expression (its type would be either
 *   ::EXPR_NEG or ::EXPR_NOT).
 * - a binary operation on two expressions (its type would be any
 *   ::expression_type apart from ::EXPR_UNKNWON and the aforementioned types).
 *
 * For further information on:
 *
 * - ::expression constructors or destructors, see \ref expression_management.
 * - ::expression getters, see \ref expression_getter.
 * - ::expression setters, see \ref expression_setter.
 * - ::expression operations, see \ref expression_operation.
 * - ::expression input/output, see \ref expression_io.
 *
 * To use lists of ::expression, see \ref expression_list_group.
 */

////////////////////////////////////////////////////////////////////////////////
// Structs, ennums, typedefs, etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Expression type.
 * \ingroup expression_group
 * \since version `1.0.0`
 */
typedef enum expression_type
{
    /* Binary boolean expressions. */
    EXPR_OR,        /**< The expression is a disjunction. */
    EXPR_AND,       /**< The expression is a conjonction. */
    EXPR_LT,        /**< The expression is a lower than comparison. */
    EXPR_GT,        /**< The expression is a greater than comparison. */
    EXPR_EQ,        /**< The expression is an equality test. */
    EXPR_NE,        /**< The expression is a not equal test. */
    EXPR_LE,        /**< The expression is a lower or equal test. */
    EXPR_GE,        /**< The expression is a greater or equal test. */

    /* Binary arithmetic expressions. */
    EXPR_ADD,       /**< The expression is an addition. */
    EXPR_SUB,       /**< The expression is a substraction. */
    EXPR_MULT,      /**< The expression is a multiplication. */
    EXPR_DIV,       /**< The expression is a division. */
    EXPR_MIN,       /**< The expression is a minimum. */
    EXPR_MAX,       /**< The expression is a maximum. */

    /* Unary boolean expressions. */
    EXPR_NOT,       /**< The expression is a negation. */

    /* Unary arithmetic expressions. */
    EXPR_NEG,       /**< The expression is a negation. */

    /* Misc. */
    EXPR_ID,        /**< The expression refers to an identifier. */
    EXPR_NUMBER,    /**< The expression is a number. */

    /* Boolean constants. */
    EXPR_TRUE,      /**< The expression is true. */
    EXPR_FALSE,     /**< The expression is false. */

    /* Always leave the EXPR_UNKNOWN at the end! */
    EXPR_UNKNOWN,   /**< The expression's type is unknown. */
} expression_type;

/**
 * \brief Expression.
 * \ingroup expression_group
 * \since version `1.0.0`
 */
typedef struct expression
{
    expression_type type;               /**< The expression's type. */
    union
    {
        struct
        {
            struct expression * left;   /**< The left operand. */
            struct expression * right;  /**< The right operand. */
        } operands;                     /**< The expression's operands. */
        char * identifier;              /**< The expression's identifier. */
        long int number;                /**< The expression's number. */
    } content;                          /**< The expression's content. */
} expression;

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup expression_management Constructors / Destructors
 * \ingroup expression_group
 * \brief Create or destroy expressions.
 * \since version `1.0.0`
 *
 * Blank expressions can be created with expression_alloc(). Any expression
 * created via expression_alloc() can be destroyed with expression_free().
 * An expression can be copied with expression_copy().
 *
 * It is also possible to directly create constant expressions from:
 *
 * - a boolean value: expression_from_boolean()
 * - an identifier: expression_from_identifier()
 * - a number: expression_from_number()
 *
 * \warning It is strongly advised not to use expression_init() and
 * expression_clean() unless you know what you are doing!
 */

//----------------------------------------------------------------------------//

/**
 * \brief Initialize an expression.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \param e A pointer to the expression to initialize.
 *
 * \note This function is already called by expression_alloc().
 */
void expression_init (expression * e);

/**
 * \brief Clean an expression.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \param e A pointer to the expression to clean.
 *
 * \note This function is already called by expression_free().
 */
void expression_clean (expression * e);

/**
 * \brief Allocate and initialize an expression.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \return A pointer to the newly allocated expression.
 *
 * \note It is not needed to initialize the resulting expression
 * using expression_init().
 */
expression * expression_alloc (void);

/**
 * \brief Free an expression.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \param e A pointer to the expression to free.
 */
void expression_free (expression * e);

/**
 * \brief Copy an expression.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \param e A pointer to the expression to copy.
 * \return A pointer to the copy.
 */
expression * expression_copy (const expression * e);

////////////////////////////////////////////////////////////////////////////////
// Special creation.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Create an expression from a number.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \param number Input number.
 * \return The resulting expression.
 */
expression * expression_from_number (long int number);

/**
 * \brief Create an expression from an identifier.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \param identifier Input identifier.
 * \return The resulting expression.
 */
expression * expression_from_identifier (const char * identifier);

/**
 * \brief Create an expression from a boolean.
 * \relates expression
 * \ingroup expression_management
 * \since version `1.0.0`
 *
 * \param boolean Input boolean.
 * \return The resulting expression.
 *
 */
expression * expression_from_boolean (bool boolean);

////////////////////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup expression_setter Setters
 * \ingroup expression_group
 * \brief Set properties of expressions.
 * \since version `1.0.0`
 *
 * These functions allows the user to set the properties of an expression.
 * Please set coherent properties. If you are not confident a given set of
 * properties would be coherent, please fallback to the functions from
 * \ref expression_management and \ref expression_operation. The functions
 * from these modules automatically set the appropriate properties.
 */

//----------------------------------------------------------------------------//

/**
 * \brief Set the type of an expression.
 * \relates expression
 * \ingroup expression_setter
 * \since version `1.0.0`
 *
 * \param e Target expression.
 * \param t Expression type.
 */
void expression_set_type (expression * e, expression_type t);

/**
 * \brief Set the value of a number expression.
 * \relates expression
 * \ingroup expression_setter
 * \since version `1.0.0`
 *
 * \param e Target expression.
 * \param number value.
 */
void expression_set_number (expression * e, long int number);

/**
 * \brief Set the identifier of an identifier expression.
 * \relates expression
 * \ingroup expression_setter
 * \since version `1.0.0`
 *
 * \param e Target expression.
 * \param identifier Identifier.
 */
void expression_set_identifier (expression * e, const char * identifier);

/**
 * \brief Set the boolean value of a boolean expression.
 * \relates expression
 * \ingroup expression_setter
 * \since version `1.0.0`
 *
 * \param e Target expression.
 * \param boolean Boolean value.
 */
void expression_set_boolean (expression * e, bool boolean);

/**
 * \brief Set the left operand of a binary expression.
 * \relates expression
 * \ingroup expression_setter
 * \since version `1.0.0`
 *
 * \param e Target expression.
 * \param left Left operand.
 */
void expression_set_left_operand (expression * e, expression * left);

/**
 * \brief Set the right operand of a binary expression.
 * \relates expression
 * \ingroup expression_setter
 * \since version `1.0.0`
 *
 * \param e Target expression.
 * \param right Right operand.
 */
void expression_set_right_operand (expression * e, expression * right);

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup expression_getter Getters
 * \ingroup expression_group
 * \brief Get properties of an expression.
 * \since version `1.0.0`
 *
 *
 * Expression type inspection
 * --------------------------
 *
 * The type of an expression can and should be inspected using
 * expression_get_type().
 *
 * Constant expressions have dedicated inspection functions:
 *
 * - expression_is_boolean()
 * - expression_is_number()
 * - expression_is_identifier()
 *
 * Number expressions
 * ------------------
 *
 * If an expression is a number, it is possible to:
 *
 * - get its value: expression_get_number()
 * - determine whether it is evaluates to 0: expression_is_zero()
 * - determine whether it is evaluates to 1: expression_is_one()
 *
 * Boolean expressions
 * -------------------
 *
 * If an expression is a boolean value, it is possible to:
 *
 * - get its value: expression_get_boolean()
 * - determine whether it is evaluates to \c true: expression_is_true()
 * - determine whether it is evaluates to \c false: expression_is_false()
 *
 * Other expressions
 * -----------------
 *
 * For identifier expressions, the identifier can be obtained with
 * expression_get_identifier().
 *
 * The left and right operands of a binary operation can be obtained with
 * expression_get_left() and expression_get_right().
 *
 * \warning Please note that it is only safe to use the appropriate getter(s)
 * on an expression:
 *
 * \warning
 * Expression type               | Appropriate getter(s)
 * ------------------------------|---------------------------------------------
 * ::EXPR_NUMBER                 | expression_get_number()
 * ::EXPR_TRUE                   | expression_get_boolean()
 * ::EXPR_FALSE                  | expression_get_boolean()
 * ::EXPR_ID                     | expression_get_identifier()
 * ::EXPR_NOT                    | expression_get_left()
 * ::EXPR_NEG                    | expression_get_left()
 * other types != ::EXPR_UNKNOWN | expression_get_left(), expression_get_right()
 */

//----------------------------------------------------------------------------//

/**
 * \brief Get an expression's type.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \return The expression's type.
 *
 */
expression_type expression_get_type (const expression * e);

/**
 * \brief Determine whether an expression is a number expression.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \retval true if the expression is a number.
 * \retval false otherwise.
 *
 */
bool expression_is_number (const expression * e);

/**
 * \brief Determine whether an expression is a boolean expression.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \retval true if the expression is a boolean.
 * \retval false otherwise.
 */
bool expression_is_boolean (const expression * e);

/**
 * \brief Determine whether an expression is an identifier.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \retval true if the expression is an identifier.
 * \retval false otherwise.
 */
bool expression_is_identifier (const expression * e);

/**
 * \brief Get an expression's number.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \return Number.
 */
long int expression_get_number (const expression * e);

/**
 * \brief Get an expression's identifier.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \return Identifier.
 */
const char * expression_get_identifier (const expression * e);

/**
 * \brief Get an expression's boolean.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \return Boolean.
 */
bool expression_get_boolean (const expression * e);

/**
 * \brief Get an expression's left operand.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \return Left operand.
 */
expression * expression_get_left (const expression * e);

/**
 * \brief Get an expression's right operand.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \return Right operand.
 */
expression * expression_get_right (const expression * e);

/**
 * \brief Determine whether an expression is 0.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \retval true if the expression is 0.
 * \retval otherwise.
 */
bool expression_is_zero (const expression * e);

/**
 * \brief Determine whether an expression is 1.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \retval true if the expression is 1.
 * \retval otherwise.
 */
bool expression_is_one (const expression * e);

/**
 * \brief Determine whether an expression is true.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \retval true if the expression is true.
 * \retval otherwise.
 */
bool expression_is_true (const expression * e);

/**
 * \brief Determine whether an expression is false.
 * \relates expression
 * \ingroup expression_getter
 * \since version `1.0.0`
 *
 * \param e Expression of interest.
 * \retval true if the expression is true.
 * \retval otherwise.
 */
bool expression_is_false (const expression * e);

////////////////////////////////////////////////////////////////////////////////
// Operations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup expression_operation Operations
 * \ingroup expression_group
 * \brief Operations on expressions.
 * \since version `1.0.0`
 *
 * These functions apply operations on expressions.
 *
 * \warning It is not safe to use the input expressions once operations have
 * be applied!
 */

//----------------------------------------------------------------------------//

/**
 * \brief Negate an expression.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param e Expression.
 * \return The resulting expression.
 *
 * \pre e != NULL
 * \warning It is not safe to use e afterwards.
 */
expression * expression_neg (expression * e);

/**
 * \brief Not an expression.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param e Expression.
 * \return The resulting expression.
 *
 * \pre e != NULL
 * \warning It is not safe to use e afterwards.
 */
expression * expression_not (expression * e);

/**
 * \brief Add two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_add (expression * a, expression * b);

/**
 * \brief Substract two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_sub (expression * a, expression * b);

/**
 * \brief Multiply two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_mult (expression * a, expression * b);

/**
 * \brief Divide two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_div (expression * a, expression * b);

/**
 * \brief Minimum of two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_min (expression * a, expression * b);

/**
 * \brief Maximum of two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_max (expression * a, expression * b);

/**
 * \brief Conjunction of two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_and (expression * a, expression * b);

/**
 * \brief Disjunction of two expressions.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_or (expression * a, expression * b);

/**
 * \brief Lower than.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_lt (expression * a, expression * b);

/**
 * \brief Lower or equals.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_le (expression * a, expression * b);

/**
 * \brief Greater than.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_gt (expression * a, expression * b);

/**
 * \brief Greater or equals.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \warning It is not safe to use either a or b afterwards.
 * \pre a != NULL and b != NULL
 */
expression * expression_ge (expression * a, expression * b);

/**
 * \brief Equals.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_eq (expression * a, expression * b);

/**
 * \brief Differs.
 * \relates expression
 * \ingroup expression_operation
 * \since version `1.0.0`
 *
 * \param a Left operand.
 * \param b Right operand.
 * \return The result of the operation.
 *
 * \pre a != NULL and b != NULL
 * \warning It is not safe to use either a or b afterwards.
 */
expression * expression_ne (expression * a, expression * b);

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup expression_io Input / Output
 * \ingroup expression_group
 * \brief Input or output expressions.
 * \since version `1.0.0`
 *
 * These functions provide ways to print expressions into streams or strings.
 */

//----------------------------------------------------------------------------//

/**
 * \brief Write an expression to an output stream.
 * \relates expression
 * \ingroup expression_io
 * \since version `1.0.0`
 *
 * \param f Output stream.
 * \param e Expression to write.
 */
void expression_fprint (FILE * f, const expression * e);

/**
 * \brief Write an expression to stdout.
 * \relates expression
 * \ingroup expression_io
 * \since version `1.0.0`
 *
 * \param e Expression to write.
 */
void expression_print (const expression * e);

/**
 * \brief Convert an expression to a string.
 * \relates expression
 * \ingroup expression_io
 * \since version `1.0.0`
 *
 * \param e The expression to convert into a string.
 * \return The resulting string.
 *
 * \warning It is up to the user to free the resulting string.
 */
char * expression_to_string (const expression * e);

#endif /* __EXPRESSION_H__ */
