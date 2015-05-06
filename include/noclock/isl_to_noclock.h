/**
 * \file isl_to_noclock.h
 * \brief ISL AST to noclock AST conversion.
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

#ifndef __ISL_TO_NOCLOCK_H__
#define __ISL_TO_NOCLOCK_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include <isl/ctx.h>
#include <isl/ast.h>
#include <isl/ast_type.h>

#include "noclock/debug.h"
#include "noclock/util.h"
#include "noclock/instruction.h"
#include "noclock/instruction_list.h"

/**
 * \defgroup conversion_group Conversions.
 * \brief AST and expression conversions.
 * \since version `1.0.0`
 */

/**
 * \defgroup isl_to_noclock_group ISL to No Clock conversions.
 * \brief Convert ISL to No Clock.
 * \ingroup conversion_group
 * \since version `1.0.0`
 */

////////////////////////////////////////////////////////////////////////////////
// ISL -> No Clock.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Convert an ISL expression to a No Clock expression.
 * \ingroup isl_to_noclock_group
 * \since version `1.0.0`
 *
 * \param expr Input ISL expression.
 * \return No Clock expression.
 */
expression * isl_expr_to_noclock_expr (isl_ast_expr * expr);

/**
 * \brief Convert an ISL AST to a No Clock AST.
 * \ingroup isl_to_noclock_group
 * \since version `1.0.0`
 *
 * \param ast Input ISL AST
 * \return No Clock AST.
 */
instruction_list * isl_ast_to_noclock_ast (isl_ast_node * ast);

#endif
