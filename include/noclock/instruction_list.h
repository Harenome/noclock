/**
 * \file instruction_list.h
 * \brief Instruction lists.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * This file declares the contents of the \ref instruction_list module (except
 * the ::instruction_list struct which is declared in the \ref instruction
 * module).
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

/**
 * \defgroup instruction_list_group Lists of instructions
 * \ingroup instruction_global_group
 * \brief Lists of ::instruction
 * \since version `1.0.0`
 */

////////////////////////////////////////////////////////////////////////////////
// Allocation, initialization, copy, cleaning, free.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_list_management Constructors / Destructors
 * \ingroup instruction_list_group
 * \brief Create or destroy ::instruction_list lists.
 *
 * An ::instruction_list node can be created using instruction_list_alloc().
 *
 * An ::instruction_list list must be destroyed using instruction_list_free() or
 * softly destroyed using instruction_list_soft_free().
 *
 * Softly freeing an ::instruction_list list using
 * instruction_list_soft_free() destroys the container list without destroying
 * the inner ::instruction elements whereas the "standard" freeing with
 * instruction_list_free() destroys the list and its elements.
 */

//----------------------------------------------------------------------------//

/**
 * \brief Allocate an instruction list.
 * \relates instruction_list
 * \ingroup instruction_list_management
 * \since version `1.0.0`
 *
 * \return A pointer to the newly allocated list.
 */
instruction_list * instruction_list_alloc (void);

/**
 * \brief Softly free an instruction list.
 * \relates instruction_list
 * \ingroup instruction_list_management
 * \since version `1.0.0`
 *
 * \param list The list to softly free.
 */
void instruction_list_soft_free (instruction_list * list);

/**
 * \brief Free an instruction list.
 * \relates instruction_list
 * \ingroup instruction_list_management
 * \since version `1.0.0`
 *
 * \param list The list to free.
 */
void instruction_list_free (instruction_list * list);

////////////////////////////////////////////////////////////////////////////////
// Operations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_list_operation Operations
 * \ingroup instruction_list_group
 * \brief Operations on ::instruction_list lists.
 * \since version `1.0.0`
 */

//----------------------------------------------------------------------------//

/**
 * \brief Append an instruction to an instruction list.
 * \relates instruction_list
 * \ingroup instruction_list_operation
 * \since version `1.0.0`
 *
 * \param list Input instruction list.
 * \param i Instruction to append.
 * \return The resulting instruction list.
 */
instruction_list * instruction_list_append (instruction_list * list,
        instruction * i);

/**
 * \brief Concatenate instruction lists.
 * \relates instruction_list
 * \ingroup instruction_list_operation
 * \since version `1.0.0`
 *
 * \param a First list.
 * \param b Second list.
 * \return The resulting list.
 */
instruction_list * instruction_list_cat (instruction_list * a,
        instruction_list * b);

/**
 * \brief Wrap an instruction list with a wrapping block construct.
 * \relates instruction_list
 * \ingroup instruction_list_operation
 * \since version `1.0.0`
 *
 * \param list Input list.
 * \param instr Input instruction.
 * \param t Wrapper type.
 */
void instruction_list_wrap (instruction_list * list, instruction * instr,
        instruction_type t);

/**
 * \brief Fill an AST with the missing finish and async constructs.
 * \relates instruction_list
 * \ingroup instruction_list_operation
 * \since version `1.0.0`
 *
 * \param list Input AST.
 * \param calls List of function calls inside the AST.
 */
void instruction_list_fill (instruction_list * list, instruction_list * calls);

/**
 * \brief Strip an AST of unnecessary information in function calls.
 * \relates instruction_list
 * \ingroup instruction_list_operation
 * \since version `1.0.0`
 *
 * \param list Input AST.
 * \param s List of function identifiers.
 */
void instruction_list_strip (instruction_list * list, string_list * s);

////////////////////////////////////////////////////////////////////////////////
// Annotations.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_list_annotation Annotation
 * \ingroup instruction_list_group
 * \brief Add annotations to ASTs.
 * \since version `1.0.0`
 */

//----------------------------------------------------------------------------//

/**
 * \brief Decorate an AST with levels and boundaries.
 * \relates instruction_list
 * \ingroup instruction_list_annotation
 * \since version `1.0.0`
 *
 * \param list List to decorate.
 * \param level Current level.
 * \param boundaries Current boundaries.
 */
void instruction_list_decorate (instruction_list * list, const char * level,
        const char * boundaries);

/**
 * \brief Compute the dates of an AST.
 * \relates instruction_list
 * \ingroup instruction_list_annotation
 * \since version `1.0.0`
 *
 * \param list Input list.
 * \param e Current date.
 * \param identifier Current identifier.
 */
void instruction_list_compute_dates (instruction_list * list,
        const expression * e, const char * identifier);

////////////////////////////////////////////////////////////////////////////////
// Getters.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_list_getter Getters
 * \ingroup instruction_list_group
 * \brief Getters on ::instruction_list.
 * \since version `1.0.0`
 */

//----------------------------------------------------------------------------//

/**
 * \brief Get the nth instruction in an instruction list.
 * \relates instruction_list
 * \ingroup instruction_list_getter
 * \since version `1.0.0`
 *
 * \param list Input list.
 * \param position Position of the instruction.
 * \return The nth instruction.
 */
instruction_list * instruction_list_n (instruction_list * list, size_t position);

/**
 * \brief Get the size of an instruction list.
 * \relates instruction_list
 * \ingroup instruction_list_getter
 * \since version `1.0.0`
 *
 * \param list Input instruction list
 * \return The size of the list.
 */
size_t instruction_list_size (instruction_list * list);

/**
 * \brief Determine whether a list is an indirect parent of an instruction.
 * \relates instruction_list
 * \ingroup instruction_list_getter
 * \since version `1.0.0`
 *
 * \param list List.
 * \param instr Instruction.
 * \retval true if \a list is an indirect parent of \a instr.
 */
bool instruction_list_is_indirect_parent (instruction_list * list,
        instruction * instr);

/**
 * \brief Find, if any, the parent of an instruction.
 * \relates instruction_list
 * \ingroup instruction_list_getter
 * \since version `1.0.0`
 *
 * \param list List.
 * \param instr Instruction.
 * \retval The parent of the instruction \a instr.
 */
instruction_list * instruction_list_find_parent (instruction_list * list,
        instruction * instr);

/**
 * \brief Get the list of function calls in an AST.
 * \relates instruction_list
 * \ingroup instruction_list_getter
 * \since version `1.0.0`
 *
 * \param ast Input AST.
 * \return The list of function calls.
 */
instruction_list * call_list (instruction_list * ast);

////////////////////////////////////////////////////////////////////////////////
// Input / Output.
////////////////////////////////////////////////////////////////////////////////

/**
 * \defgroup instruction_list_io Input / Output.
 * \ingroup instruction_list_group
 * \brief I/O on ::instruction_list.
 * \since version `1.0.0`
 */

//----------------------------------------------------------------------------//


/**
 * \brief Print an instruction list to a stream.
 * \relates instruction_list
 * \ingroup instruction_list_io
 * \since version `1.0.0`
 *
 * \param f Destination stream.
 * \param list List to print.
 */
void instruction_list_fprint (FILE * f, const instruction_list * list);

#endif /* __INSTRUCTION_LIST_H__ */
