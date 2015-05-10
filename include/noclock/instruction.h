/**
 * \file instruction.h
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

#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sysexits.h>

#include "noclock/util.h"
#include "noclock/debug.h"
#include "noclock/pretty_print.h"

#include "noclock/expression.h"
#include "noclock/expression_list.h"
#include "noclock/string_list.h"

/**
 * \defgroup instruction_global_group Instructions
 * \brief ::instruction handling.
 * \since version `1.0.0`
 *
 * For further information on:
 *
 * - instructions, see \ref instruction_group
 * - lists of instructions, see \ref instruction_list_group
 */

/**
 * \defgroup instruction_group Instructions
 * \ingroup instruction_global_group
 * \brief Instructions for NoClock ASTs.
 * \since version `1.0.0`
 *
 * This module contains the ::instruction_type `enum` and the ::instruction
 * `struct`.
 *
 * Each ::instruction shall have one of the following types:
 *
 * - ::INSTR_CALL: the instruction is a function call.
 * - ::INSTR_FOR: the instruction is a `for` loop.
 * - ::INSTR_IF or ::INSTR_IF_ELSE: the instruction is a `if then else` branc.
 * - ::INSTR_ADVANCE: the instruction is a `Clock.advanceAll()`.
 * - ::INSTR_FINISH: the instruction is `finish` block.
 * - ::INSTR_FINISH: the instruction is an `async` block.
 * - ::INSTR_CLOCKED_FINISH or ::INSTR_CLOCKED_ASYNC: the instruction is a
 *   `clocked` version of either a ::INSTR_FINISH or ::INSTR_ASYNC block.
 * - ::INSTR_UNKNOWN: the instruction's type is unknown.
 *
 * For further information on:
 *
 * - constructors or destructors, see \ref instruction_management.
 */

////////////////////////////////////////////////////////////////////////////////
// Structs, ennums, typedefs, etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Instruction type.
 * \ingroup instruction_group
 * \since version `1.0.0`
 */
typedef enum instruction_type
{
    INSTR_CALL,             /**< The instruction is a function call. */
    INSTR_FOR,              /**< The instruction is a for loop. */
    INSTR_IF,               /**< The instruction is a `if then else` branch. */
    INSTR_IF_ELSE,          /**< The instruction is a `if then else` branch. */
    INSTR_ADVANCE,          /**< The instruction is an `advance`. */
    INSTR_FINISH,           /**< The instruction is a `finish` block. */
    INSTR_ASYNC,            /**< The instruction is an `async` block. */
    INSTR_CLOCKED_FINISH,   /**< The instruction is a `clocked finish` block. */
    INSTR_CLOCKED_ASYNC,    /**< The instruction is a `clocked async` block. */
    INSTR_UNKNOWN,          /**< The instruction's type is unknown. */
} instruction_type;

/**
 * \brief Lists of ::instruction `struct`s.
 * \ingroup instruction_list_group
 * \since version `1.0.0`
 */
typedef struct instruction_list
{
    struct instruction * element;       /**< The node's element. */
    struct instruction_list * next;     /**< The next node. */
} instruction_list;

//----------------------------------------------------------------------------//

/**
 * \defgroup instruction_content_group Instruction contents.
 * \brief Instruction contents.
 * \since version `1.0.0`
 */

//----------------------------------------------------------------------------//

/**
 * \brief Function call.
 * \ingroup instruction_content_group
 * \since version `1.0.0`
 */
typedef struct function_call
{
    char * identifier;              /**< The identifier of the function call. */
    expression_list * arguments;    /**< The arguments of the function call. */
} function_call;

/**
 * \brief For loop.
 * \ingroup instruction_content_group
 * \since version `1.0.0`
 */
typedef struct for_loop
{
    char * identifier;              /**< The identifier of the iterator. */
    expression * left_boundary;     /**< The left boundary. */
    expression * right_boundary;    /**< The right boundary. */
    instruction_list * body;        /**< The body of the loop. */
} for_loop;

/**
 * \brief `if then else` branchs.
 * \ingroup instruction_content_group
 * \since version `1.0.0`
 */
typedef struct if_then_else
{
    bool has_else;                  /**< Whether the branch has an else body. */
    expression * condition;         /**< Branching condition. */
    instruction_list * true_body;   /**< Body when the condition is true. */
    instruction_list * false_body;  /**< Body when the condition is false. */
} if_then_else;

//----------------------------------------------------------------------------//

/**
 * \brief Annotations.
 * \ingroup instruction_group
 * \since version `1.0.0`
 */
typedef struct instruction_annotation
{
    char * level;                   /**< Level in the AST. */
    char * boundaries;              /**< Boundaries. */
    expression * date;              /**< Dates. */
} instruction_annotation;

//----------------------------------------------------------------------------//

/**
 * \brief Instruction.
 * \ingroup instruction_group
 * \since version `1.0.0`
 */
typedef struct instruction
{
    instruction_type type;              /**< The instruction's type. */
    union
    {
        function_call call;             /**< The function call. */
        for_loop loop;                  /**< The for loop. */
        if_then_else branch;            /**< The branch. */
        instruction_list * block;       /**< The block. */
    } content;                          /**< The instruction's content. */
    instruction_annotation annotation;  /**< The annotation. */
} instruction;

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_management Constructors / Destructors
 * \ingroup instruction_group
 * \brief Create or destroy an ::instruction.
 *
 * An ::instruction can be created using instruction_alloc() and destroyed
 * using instruction_free().
 */

//----------------------------------------------------------------------------//

/**
 * \brief Allocate an instruction.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \return The newly allocated instruction.
 */
instruction * instruction_alloc (void);

/**
 * \brief Free an instruction.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param i Instruction to free.
 */
void instruction_free (instruction  * i);

////////////////////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Build a function call instruction.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param identifier Identifier of the function.
 * \param arguments Arguments of the function.
 * \return The resulting instruction.
 */
instruction * instruction_function_call (char * identifier,
        expression_list * arguments);

/**
 * \brief Build a for loop.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param identifier Identifier of the iterator.
 * \param left Left boundary of the for loop.
 * \param right Right boundary of the for loop.
 * \param body Body of the loop.
 * \return The resulting instruction.
 */
instruction * instruction_for_loop (char * identifier, expression * left,
        expression * right, instruction_list * body);

/**
 * \brief Build a branch.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param has_else Whether the branch has an else body.
 * \param condition The branching condition.
 * \param true_body The body when the condition is true.
 * \param false_body The body when the condition is false.
 * \return The resulting instruction.
 */
instruction * instruction_if_then_else (bool has_else, expression * condition,
        instruction_list * true_body, instruction_list * false_body);

/**
 * \brief Build an advance.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \return The resulting instruction.
 */
instruction * instruction_advance (void);

/**
 * \brief Build a finish block.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param block The block.
 * \return The resulting instruction.
 */
instruction * instruction_finish (instruction_list * block);

/**
 * \brief Build an async block.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param block The block.
 * \return The resulting instruction.
 */
instruction * instruction_async (instruction_list * block);

/**
 * \brief Build a clocked finish block.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param block The block.
 * \return The resulting instruction.
 */
instruction * instruction_clocked_finish (instruction_list * block);

/**
 * \brief Build a clocked async block.
 * \relates instruction
 * \ingroup instruction_management
 * \since version `1.0.0`
 *
 * \param block The block.
 * \return The resulting instruction.
 */
instruction * instruction_clocked_async (instruction_list * block);

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_getter Getters
 * \ingroup instruction_group
 * \brief Get properties of instructions.
 * \since version `1.0.0`
 *
 * The type of an instruction can be inspected with instruction_get_type().
 *
 * \anchor instruction_getter_several_types
 * Getters valid for several types
 * -------------------------------
 *
 * The following getters are valid for several types of instructions. Their
 * behaviour depends on the actual type of the target instruction.
 *
 * - instruction_identifier()
 *
 *     This getter is only valid for ::INSTR_FOR and ::INSTR_CALL.
 *
 *     - ::INSTR_FOR: get the identifier of the foor loop (equivalent to
 *         instruction_for_loop_get_identifier()).
 *
 *     - ::INSTR_CALL: get the identifier of the function loop (equivalent to
 *         instruction_function_call_get_identifier()).
 *
 * - instruction_body()
 *
 *     This getter is not valid for ::INSTR_CALL, ::INSTR_ADVANCE and
 *     ::INSTR_UNKNOWN. It is valid to use this getter for all other types of
 *     instruction.
 *
 *     It returns the inner body of the target instruction. For instructions
 *     which may have two bodies (::INSTR_IF and ::INSTR_IF_ELSE), this getter
 *     returns the body which is known as "true" body.
 *
 *     Depending on an instruction's type, the behaviour of this getter is
 *     equivalent to:
 *
 *     - instruction_for_loop_get_body() for ::INSTR_FOR instructions.
 *
 *     - instruction_if_then_else_get_true_body() for ::INSTR_IF and
 *       ::INSTR_IF_ELSE instructions.
 *
 *     - instruction_other_get_block() for ::INSTR_FINISH, ::INSTR_ASYNC,
 *       ::INSTR_CLOCKED_FINISH and ::INSTR_CLOCKED_ASYNC instructions.
 *
 * \anchor instruction_getter_function_call
 * Getters on function calls
 * -------------------------
 *
 * The getters specific to ::INSTR_CALL instructions are:
 *
 * - instruction_function_call_get_identifier()
 *   returns the identifier of the function.
 *
 * - instruction_function_call_get_arguments()
 *   returns the arguments of the function call.
 *
 * \anchor instruction_getter_for_loop
 * Getters on for loops
 * --------------------
 *
 * The getters for ::INSTR_FOR instructions are:
 *
 * - instruction_for_loop_get_identifier()
 *   returns the identifier of the iterator.
 *
 * - instruction_for_loop_get_left_boundary()
 *   returns the left boundary of the loop.
 *
 * - instruction_for_loop_get_right_boundary()
 *   returns the right boundary of the loop.
 *
 * - instruction_for_loop_get_body()
 *   returns the body of the loop.
 *
 * \anchor instruction_getter_branch
 * Getters on branchs
 * ------------------
 *
 *  The getters for ::INSTR_IF and ::INSTR_IF_ELSE instructions are:
 *
 *  - instruction_if_then_else_get_has_else()
 *    returns a boolean which indicates whether the branch has an else body.
 *
 *  - instruction_if_then_else_get_condition()
 *    returns the branching condition.
 *
 *  - instruction_if_then_else_get_true_body()
 *    returns the "true" body (also known as an "if body").
 *
 *  - instruction_if_then_else_get_false_body()
 *    returns the "false" body (also known as an "else body").
 *
 * \anchor instruction_getter_x10
 * Getters on X10 block wrappers
 * -----------------------------
 *
 * Use instruction_other_get_block() to get the inner block of ::INSTR_FINISH,
 * ::INSTR_CLOCKED_FINISH, ::INSTR_ASYNC and ::INSTR_CLOCKED_ASYNC instructions.
 *
 */

//----------------------------------------------------------------------------//

/**
 * \brief Get an instruction's type.
 * \relates instruction
 * \ingroup instruction_getter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The instruction's type.
 */
instruction_type instruction_get_type (const instruction * instr);

//----------------------------------------------------------------------------//

/**
 * \defgroup instruction_getter_misc Mixed instruction types
 * \ingroup instruction_getter
 * \brief Getters on several types of instructions.
 * \since version `1.0.0`
 * \see \ref instruction_getter_several_types "Getters on several types"
 */

/**
 * \brief Get an instruction's identifier.
 * \relates instruction
 * \ingroup instruction_getter_misc
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The instruction's identifier.
 * \retval NULL if the target instruction's type is incompatible with this
 * getter (see \ref instruction_getter_several_types "Getters valid for several
 * types").
 *
 * \warning This getter shall not be used on an instruction if it's type is
 * neither ::INSTR_FOR nor ::INSTR_CALL.
 * \see \ref instruction_getter_several_types "Getters valid for several types"
 */
char * instruction_identifier (instruction * instr);

/**
 * \brief Get an instruction's body.
 * \relates instruction
 * \ingroup instruction_getter_misc
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The instruction's body.
 * \retval NULL if the target instruction's type is incompatible with this
 * getter (see \ref instruction_getter_several_types
 * "Getters valid for several types").
 *
 * \warning This getter shall not be used on ::INSTR_ADVANCE, ::INSTR_CALL
 * or ::INSTR_UNKNOWN instructions.
 * \see \ref instruction_getter_several_types "Getters valid for several types"
 */
instruction_list * instruction_body (instruction * instr);

//----------------------------------------------------------------------------//

/**
 * \defgroup instruction_getter_function_call_group Function calls
 * \ingroup instruction_getter
 * \brief Getters on function calls.
 * \since version `1.0.0`
 * \see \ref instruction_getter_function_call "Getters on function calls"
 */

/**
 * \brief Get a function call's identifier.
 * \relates instruction
 * \ingroup instruction_getter_function_call_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The function call's identifier.
 *
 * \pre instruction_get_type() returns ::INSTR_CALL if used on \a instr.
 * \see \ref instruction_getter_function_call "Getters on function calls"
 */
char * instruction_function_call_get_identifier (instruction * instr);

/**
 * \brief Get a function call's arguments.
 * \relates instruction
 * \ingroup instruction_getter_function_call_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The function call's arguments.
 *
 * \pre instruction_get_type() returns ::INSTR_CALL if used on \a instr.
 * \see \ref instruction_getter_function_call "Getters on function calls"
 */
expression_list * instruction_function_call_get_arguments (instruction * instr);

//----------------------------------------------------------------------------//

/**
 * \defgroup instruction_getter_for_loop_group For loops
 * \ingroup instruction_getter
 * \brief Getters on `for` loops.
 * \since version `1.0.0`
 * \see \ref instruction_getter_for_loop "Getters on for loops"
 */

/**
 * \brief Get a for loop's iterator identifier.
 * \relates instruction
 * \ingroup instruction_getter_for_loop_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The identifier of the loop's iterator.
 *
 * \pre instruction_get_type() returns ::INSTR_FOR if used on \a instr.
 * \see \ref instruction_getter_for_loop "Getters on for loops"
 */
char * instruction_for_loop_get_identifier (instruction * instr);

/**
 * \brief Get a for loop's left boundary.
 * \relates instruction
 * \ingroup instruction_getter_for_loop_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The left boundary of the loop.
 *
 * \pre instruction_get_type() returns ::INSTR_FOR if used on \a instr.
 * \see \ref instruction_getter_for_loop "Getters on for loops"
 */
expression * instruction_for_loop_get_left_boundary (instruction * instr);

/**
 * \brief Get a for loop's right boundary.
 * \relates instruction
 * \ingroup instruction_getter_for_loop_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The right boundary of the loop.
 *
 * \pre instruction_get_type() returns ::INSTR_FOR if used on \a instr.
 * \see \ref instruction_getter_for_loop "Getters on for loops"
 */
expression * instruction_for_loop_get_right_boundary (instruction * instr);

/**
 * \brief Get a for loop's body.
 * \relates instruction
 * \ingroup instruction_getter_for_loop_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The body of the loop.
 *
 * \pre instruction_get_type() returns ::INSTR_FOR if used on \a instr.
 * \see \ref instruction_getter_for_loop "Getters on for loops"
 */
instruction_list * instruction_for_loop_get_body (instruction * instr);

//----------------------------------------------------------------------------//

/**
 * \defgroup instruction_getter_if_then_else_group If Then Else
 * \ingroup instruction_getter
 * \brief Getters on `if then else` branchs.
 * \since version `1.0.0`
 * \see \ref instruction_getter_branch "Getters on branchs"
 */

/**
 * \brief Get a branch's if_then_else::has_else property.
 * \relates instruction
 * \ingroup instruction_getter_if_then_else_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \retval true if there is a "false" body.
 * \retval false otherwise.
 *
 * \pre instruction_get_type() returns ::INSTR_IF or ::INSTR_IF_ELSE if used
 * on \a instr.
 * \see \ref instruction_getter_branch "Getters on branchs"
 */
bool instruction_if_then_else_get_has_else (instruction * instr);

/**
 * \brief Get a branch's condition.
 * \relates instruction
 * \ingroup instruction_getter_if_then_else_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The branching condition.
 *
 * \pre instruction_get_type() returns ::INSTR_IF or ::INSTR_IF_ELSE if used
 * on \a instr.
 * \see \ref instruction_getter_branch "Getters on branchs"
 */
expression * instruction_if_then_else_get_condition (instruction * instr);

/**
 * \brief Get a branch's true body.
 * \relates instruction
 * \ingroup instruction_getter_if_then_else_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The true body.
 *
 * \pre instruction_get_type() returns ::INSTR_IF or ::INSTR_IF_ELSE if used
 * on \a instr.
 * \see \ref instruction_getter_branch "Getters on branchs"
 */
instruction_list * instruction_if_then_else_get_true_body (instruction * instr);

/**
 * \brief Get a branch's true body.
 * \relates instruction
 * \ingroup instruction_getter_if_then_else_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The true body.
 *
 * \pre instruction_get_type() returns ::INSTR_IF or ::INSTR_IF_ELSE if used
 * on \a instr.
 * \see \ref instruction_getter_branch "Getters on branchs"
 */
instruction_list * instruction_if_then_else_get_false_body (instruction * instr);

//----------------------------------------------------------------------------//

/**
 * \defgroup instruction_getter_x10_group X10 constructs
 * \ingroup instruction_getter
 * \brief Getters on X10 constructs.
 * \since version `1.0.0`
 * \see \ref instruction_getter_x10 "Getters on X10 block wrappers"
 */

/**
 * \brief Get a X10 block wrapper's body.
 * \relates instruction
 * \ingroup instruction_getter_x10_group
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \return The body.
 *
 * \pre instruction_get_type() returns ::INSTR_FINISH, ::INSTR_CLOCKED_FINISH,
 * ::INSTR_ASYNC or ::INSTR_CLOCKED_ASYNC if used on \a instr.
 * \see \ref instruction_getter_x10 "Getters on X10 block wrappers"
 */
instruction_list * instruction_other_get_block (instruction * instr);

////////////////////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_setter Setters
 * \ingroup instruction_group
 * \brief Get properties of instructions.
 * \since version `1.0.0`
 *
 * These functions allow the user to set the properties of an ::instruction.
 * Please set coherent properties. If you are not confident a given set of
 * properties would be coherent, please fallback to the functions from
 * \ref instruction_management to directly build a correct ::instruction.
 */

//----------------------------------------------------------------------------//

/**
 * \brief Set an instruction's type.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param t The target instruction new instruction type.
 */
void instruction_set_type (instruction * instr, instruction_type t);

/**
 * \brief Set a function call's identifier.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param identifier The target function call's new identifier.
 */
void instruction_function_call_set_identifier (instruction * instr,
        char * identifier);

/**
 * \brief Set a function call's arguments.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param arguments The target function call's new arguments.
 */
void instruction_function_call_set_arguments (instruction * instr,
        expression_list * arguments);

/**
 * \brief Set a for loop's identifier.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param identifier The target for loop's new identifier.
 */
void instruction_for_loop_set_identifier (instruction * instr,
        char * identifier);

/**
 * \brief Set a for loop's left boundary.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param left The target for loop's new left boundary.
 */
void instruction_for_loop_set_left_boundary (instruction * instr,
        expression * left);

/**
 * \brief Set a for loop's right boundary.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param right The target for loop's new right boundary.
 */
void instruction_for_loop_set_right_boundary (instruction * instr,
        expression * right);

/**
 * \brief Set a for loop's body.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param body The target for loop's new body.
 */
void instruction_for_loop_set_body (instruction * instr,
        instruction_list * body);

/**
 * \brief Set a if_then_else::has_else property.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param has_else The target branch's new if_then_else::has_else property.
 */
void instruction_if_then_else_set_has_else (instruction * instr, bool has_else);

/**
 * \brief Set a branching condition.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param condition The target branch's new branching condition.
 */
void instruction_if_then_else_set_condition (instruction * instr,
        expression * condition);

/**
 * \brief Set a branch's new true body.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param true_body The target branch's new true body.
 */
void instruction_if_then_else_set_true_body (instruction * instr,
        instruction_list * true_body);

/**
 * \brief Set a branch's new false body.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param false_body The target branch's new false body.
 */
void instruction_if_then_else_set_false_body (instruction * instr,
        instruction_list * false_body);

/**
 * \brief Set a X10 construct's body.
 * \relates instruction
 * \ingroup instruction_setter
 * \since version `1.0.0`
 *
 * \param instr Target instruction.
 * \param block The target X10 construct's new body.
 */
void instruction_other_set_block (instruction * instr, instruction_list * block);

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_io Input / Output
 * \ingroup instruction_group
 * \brief I/O on ::instruction.
 * \since version `1.0.0`
 */

//----------------------------------------------------------------------------//

/**
 * \brief Print an instruction into a stream.
 * \relates instruction
 * \ingroup instruction_io
 * \since \version `1.0.0`
 *
 * \param f Destination stream.
 * \param instr Instruction to print.
 */
void instruction_fprint (FILE * f, const instruction * instr);

/**
 * \brief Print an if then else instruction to a stream.
 * \relates instruction
 * \ingroup instruction_io
 * \since \version `1.0.0`
 *
 * \param f Destination stream.
 * \param instr Instruction to print.
 */
void if_then_else_fprint (FILE * f, const if_then_else * instr);

/**
 * \brief Print a for loop to a stream.
 * \relates instruction
 * \ingroup instruction_io
 * \since \version `1.0.0`
 *
 * \param f Destination stream.
 * \param instr Instruction to print.
 */
void for_loop_fprint (FILE * f, const for_loop * instr);

/**
 * \brief Print a function call to a stream.
 * \relates instruction
 * \ingroup instruction_io
 * \since \version `1.0.0`
 *
 * \param f Destination stream.
 * \param instr Instruction to print.
 */
void instruction_call_fprint (FILE * f, const function_call * instr);

#endif /* __INSTRUCTION_H__ */
