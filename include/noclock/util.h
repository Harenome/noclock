/**
 * \file util.h
 * \brief Utilities.
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

#ifndef __UTIL_H__
#define __UTIL_H__

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sysexits.h>

/**
 * \brief Macro for forbidding things.
 *
 * \param comp Comparator.
 * \param variable Variable which holds the result to compare.
 * \param result The forbidden result.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 *
 * \details This macro compares a \a variable to a forbidden \a result using
 * the comparator \a comp. If this comparison succeeds, the \a error_message
 * is used with `perror(3)` and the program exits with the \a exit_code.
 */
#define __forbid_comp(comp, variable, result, error_message, exit_code) \
    if ((variable) comp (result)) \
    { \
        perror ((error_message)); \
        exit ((exit_code)); \
    }

/**
 * \brief Forbid a value.
 *
 * \param variable Variable which holds the value.
 * \param result The forbidden value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __forbid_value(variable, result, error_message, exit_code) \
    __forbid_comp (==, variable, result, error_message, exit_code)

/**
 * \brief Forbid a value to be lower than a value.
 *
 * \param variable Variable which holds the value.
 * \param result The forbidden value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __forbid_lower(variable, result, error_message, exit_code) \
    __forbid_comp (<, variable, result, error_message, exit_code)

/**
 * \brief Forbid a value to be greater than a value.
 *
 * \param variable Variable which holds the value.
 * \param result The forbidden value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __forbid_greater(variable, result, error_message, exit_code) \
    __forbid_comp (>, variable, result, error_message, exit_code)

/**
 * \brief Forbid a variable to be lower or equal to a value.
 *
 * \param variable Variable which holds the value.
 * \param result The forbidden value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __forbid_lower_equal(variable, result, error_message, exit_code) \
    __forbid_comp (<=, variable, result, error_message, exit_code)

/**
 * \brief Forbid a variable to be greater or equal to a value.
 *
 * \param variable Variable which holds the value.
 * \param result The forbidden value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __forbid_greater_equal(variable, result, error_message, exit_code) \
    __forbid_comp (>=, variable, result, error_message, exit_code)

/**
 * \brief Expect a variable to be equal to a value.
 *
 * \param variable Variable which holds the value.
 * \param result The expected value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __expect_value(variable, result, error_message, exit_code) \
    __forbid_comp (!=, variable, result, error_message, exit_code)

/**
 * \brief Expect a variable to be lower than a value.
 *
 * \param variable Variable which holds the value.
 * \param result The expected value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __expect_lower(variable, result, error_message, exit_code) \
    __forbid_greater_equal (variable, result, error_message, exit_code)

/**
 * \brief Expect a variable to be lower or equal to a value.
 *
 * \param variable Variable which holds the value.
 * \param result The expected value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __expect_lower_equal(variable, result, error_message, exit_code) \
    __forbid_greater (variable, result, error_message, exit_code)

/**
 * \brief Expect a variable to be greater than a value.
 *
 * \param variable Variable which holds the value.
 * \param result The expected value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __expect_greater(variable, result, error_message, exit_code) \
    __forbid_lower_equal (variable, result, error_message, exit_code)

/**
 * \brief Expect a variable to be greater or equal to a value.
 *
 * \param variable Variable which holds the value.
 * \param result The expected value.
 * \param error_message The message for `perror(3)`.
 * \param exit_code The exit code for `exit(3)`.
 */
#define __expect_greater_equal(variable, result, error_message, exit_code) \
    __forbid_lower (variable, result, error_message, exit_code)

#endif /* __UTIL_H__ */
