/**
 * \file instruction_list.h
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

#ifndef __INSTRUCTION_LIST_H__
#define __INSTRUCTION_LIST_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sysexits.h>

#include "noclock/util.h"
#include "noclock/debug.h"
#include "noclock/pretty_print.h"

#include "noclock/instruction.h"

instruction_list * instruction_list_alloc (void);

void instruction_list_soft_free (instruction_list * list);

void instruction_list_free (instruction_list * list);

instruction_list * instruction_list_append (instruction_list * list,
        instruction * i);

instruction_list * instruction_list_cat (instruction_list * a,
        instruction_list * b);

void instruction_list_decorate (instruction_list * list, const char * level,
        const char * boundaries);

instruction_list * instruction_list_n (instruction_list * list, size_t position);

void instruction_list_compute_dates (instruction_list * list,
        const expression * e, const char * identifier);

void instruction_list_wrap (instruction_list * list, instruction * instr,
        instruction_type t);

bool instruction_list_is_indirect_parent (instruction_list * list,
        instruction * instr);

instruction_list * instruction_list_find_parent (instruction_list * list,
        instruction * instr);

void instruction_list_fill (instruction_list * list, instruction_list * calls);

void instruction_list_strip (instruction_list * list, string_list * s);

size_t instruction_list_size (instruction_list * list);

void instruction_list_fprint (FILE * f, const instruction_list * list);

instruction_list * call_list (instruction_list * ast);

#endif /* __INSTRUCTION_LIST_H__ */
