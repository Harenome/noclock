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

////////////////////////////////////////////////////////////////////////////////
// Structs, ennums, typedefs, etc.
////////////////////////////////////////////////////////////////////////////////

typedef enum instruction_type
{
    INSTR_CALL,
    INSTR_FOR,
    INSTR_IF,
    INSTR_IF_ELSE,
    INSTR_ADVANCE,
    INSTR_FINISH,
    INSTR_ASYNC,
    INSTR_CLOCKED_FINISH,
    INSTR_CLOCKED_ASYNC,
    INSTR_UNKNOWN,
} instruction_type;

typedef struct instruction_list
{
    struct instruction * element;
    struct instruction_list * next;
} instruction_list;

typedef struct function_call
{
    char * identifier;
    expression_list * arguments;
} function_call;

typedef struct for_loop
{
    char * identifier;
    expression * left_boundary;
    expression * right_boundary;
    instruction_list * body;
} for_loop;

typedef struct if_then_else
{
    bool has_else;
    expression * condition;
    instruction_list * true_body;
    instruction_list * false_body;
} if_then_else;

typedef struct instruction_annotation
{
    char * level;
    char * boundaries;
    expression * date;
} instruction_annotation;

typedef struct instruction
{
    instruction_type type;
    union
    {
        function_call call;
        for_loop loop;
        if_then_else branch;
        instruction_list * block;
    } content;
    instruction_annotation annotation;
} instruction;

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

instruction * instruction_alloc (void);

void instruction_free (instruction  * i);

////////////////////////////////////////////////////////////////////////////////
// Constructors.
////////////////////////////////////////////////////////////////////////////////

instruction * instruction_function_call (char * identifier,
        expression_list * arguments);

instruction * instruction_for_loop (char * identifier, expression * left,
        expression * right, instruction_list * body);

instruction * instruction_if_then_else (bool has_else, expression * condition,
        instruction_list * true_body, instruction_list * false_body);

instruction * instruction_advance (void);

instruction * instruction_finish (instruction_list * block);

instruction * instruction_async (instruction_list * block);

instruction * instruction_clocked_finish (instruction_list * block);

instruction * instruction_clocked_async (instruction_list * block);

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

instruction_type instruction_get_type (const instruction * instr);

char * instruction_identifier (instruction * instr);
instruction_list * instruction_body (instruction * instr);

char * instruction_function_call_get_identifier (instruction * instr);
expression_list * instruction_function_call_get_arguments (instruction * instr);

char * instruction_for_loop_get_identifier (instruction * instr);
expression * instruction_for_loop_get_left_boundary (instruction * instr);
expression * instruction_for_loop_get_right_boundary (instruction * instr);
instruction_list * instruction_for_loop_get_body (instruction * instr);

bool instruction_if_then_else_get_has_else (instruction * instr);
expression * instruction_if_then_else_get_condition (instruction * instr);
instruction_list * instruction_if_then_else_get_true_body (instruction * instr);
instruction_list * instruction_if_then_else_get_false_body (instruction * instr);

instruction_list * instruction_other_get_block (instruction * instr);

////////////////////////////////////////////////////////////////////////////////
// Setters.
////////////////////////////////////////////////////////////////////////////////

void instruction_set_type (instruction * instr, instruction_type t);

void instruction_function_call_set_identifier (instruction * instr,
        char * identifier);

void instruction_function_call_set_arguments (instruction * instr,
        expression_list * arguments);

void instruction_for_loop_set_identifier (instruction * instr,
        char * identifier);

void instruction_for_loop_set_left_boundary (instruction * instr,
        expression * left);

void instruction_for_loop_set_right_boundary (instruction * instr,
        expression * right);

void instruction_for_loop_set_body (instruction * instr,
        instruction_list * body);

void instruction_if_then_else_set_has_else (instruction * instr, bool has_else);

void instruction_if_then_else_set_condition (instruction * instr,
        expression * condition);

void instruction_if_then_else_set_true_body (instruction * instr,
        instruction_list * true_body);

void instruction_if_then_else_set_false_body (instruction * instr,
        instruction_list * false_body);

void instruction_other_set_block (instruction * instr, instruction_list * block);

////////////////////////////////////////////////////////////////////////////////
// Input/Output.
////////////////////////////////////////////////////////////////////////////////

void instruction_fprint (FILE * f, const instruction * instr);

void instruction_fprint_shift (FILE * f, const instruction * instr, size_t shift);

void if_then_else_fprint (FILE * f, const if_then_else * instr);

void for_loop_fprint (FILE * f, const for_loop * instr);

void instruction_call_fprint (FILE * f, const function_call * instr);

#endif /* __INSTRUCTION_H__ */
