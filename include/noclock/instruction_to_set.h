/**
 * \file instruction_to_set.h
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

#ifndef __INSTRUCTION_TO_SET_H__
#define __INSTRUCTION_TO_SET_H__

#include <isl/ctx.h>
#include <isl/set.h>
#include <isl/map.h>
#include <isl/union_set.h>

#include "noclock/util.h"
#include "noclock/verbose.h"
#include "noclock/instruction.h"
#include "noclock/string_list.h"

/**
 * \defgroup noclock_to_isl_group No Clock to ISL conversions.
 * \ingroup conversion_group
 * \brief Convert No Clock to ISL.
 */

////////////////////////////////////////////////////////////////////////////////
// No Clock -> ISL.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Convert a No Clock AST to an ISL list of sets.
 * \param ctx ISL ctx.
 * \param parameters Parameters.
 * \param instructions No Clock AST.
 * \param s Instruction names.
 * \return ISL list of sets.
 * \ingroup noclock_to_isl_group
 */
isl_set_list * program_to_set_list (isl_ctx* ctx,
        const string_list * parameters, const instruction_list * instructions,
        string_list * s);

/**
 * \brief Merge ISL sets into an union.
 * \param list ISL sets.
 * \return Union of the sets.
 * \ingroup noclock_to_isl_group
 */
isl_union_set * union_set_list (isl_set_list * list);

#endif /* __INSTRUCTION_TO_SET_H__ */
