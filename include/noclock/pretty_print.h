/**
 * \file pretty_print.h
 * \brief Pretty printing.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \version 1.0.0
 *
 * This header file declares pretty printing utilities.
 *
 * For further information, see the \ref pretty_printing_group module and its
 * child modules \ref pretty_printing_colours and \ref pretty_printing_indent.
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

#ifndef __PRETTY_PRINT_H__
#define __PRETTY_PRINT_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "noclock/util.h"

/**
 * \defgroup pretty_printing_group Pretty Printing
 * \since version 1.0.0
 *
 * \brief Pretty print text.
 *
 * The available utilities are utilities for:
 *
 * - indentation: see the \ref pretty_printing_indent module.
 * - colours: see the \ref pretty_printing_colours module.
 */

/**
 * \defgroup pretty_printing_colours Pretty Printing Colours
 * \ingroup pretty_printing_group
 * \since version 1.0.0
 *
 * \brief Use pretty colours.
 *
 * Pretty printing colour mode
 * ---------------------------
 *
 * The pretty printing colour mode can be inspected using the
 * pretty_print_colour_state() function.
 *
 * It can be enabled with pretty_print_colour_enable() and disabled with
 * pretty_print_colour_disable().
 *
 * Usage
 * -----
 *
 * Several colours or modificators are defined using macros. Since `C`
 * automatically concatenates strings, one can simply put these macros besides
 * `C` strings. When attempting to use these macros, the user should first check
 * whether the pretty printing colour mode is enabled.
 *
 * It is also strongly advised to always add ::PP_RESET at the end of the
 * strings, in order to make sure that any following text will be normal,
 * unless this is not the desired behaviour.
 *
 * ### Available macros
 *
 * #### Modifications
 *
 * Modification     |   Macro       | Text representation
 * -----------------|---------------|--------------------
 * Reset            | ::PP_RESET    | "\x1B[0m"
 * Bold             | ::PP_BOLD     | "\x1B[1m"
 *
 * #### Colours
 *
 * Modification     |   Macro       | Text representation
 * -----------------|---------------|--------------------
 * Black            | ::PP_BLACK    | "\x1B[30m"
 * Red              | ::PP_RED      | "\x1B[31m"
 * Green            | ::PP_GREEN    | "\x1B[32m"
 * Yellow           | ::PP_YELLOW   | "\x1B[33m"
 * Blue             | ::PP_BLUE     | "\x1B[34m"
 * Purple           | ::PP_PURPLE   | "\x1B[35m"
 * Cyan             | ::PP_CYAN     | "\x1B[36m"
 * Grey             | ::PP_GREY     | "\x1B[37m"
 * Gray             | ::PP_GRAY     | "\x1B[37m"
 *
 * Example
 * -------
 *
 * ~~~{.C}
 * bool enabled = pretty_printing_colour_state ();
 * if (enabled)
 *     printf (PP_BOLD PP_BLUE "Some text\n" PP_RESET);
 * else
 *     printf ("Some text\n");
 * ~~~
 *
 */

/**
 * \defgroup pretty_printing_indent Pretty Printing Indentation
 * \ingroup pretty_printing_group
 *
 * \brief Display pretty indentation.
 *
 * Indentation level
 * -----------------
 *
 * The current indentation level can be inspected using
 * pretty_print_indent_level(). It can be increased or decreased with
 * pretty_print_indent_increase() and pretty_print_indent_decrease().
 *
 * Indentation style
 * -----------------
 *
 * The current implementation provides two indentation styles:
 *
 * - ::INDENT_SPACES: indentation with spaces (which as of version 1.0.0 is the
 *   default).
 * - ::INDENT_TABS: indentation with tabs.
 *
 * The currently used indentation style can be inspected using
 * pretty_print_indent_style(). It can be changed using
 * pretty_print_indent_set_style().
 *
 * The string representation of a given indentation style can be obtained using
 * pretty_print_indent_style_string(). However, the current implementation
 * does not allow the user to change the string representation of an
 * indentation style...
 *
 * Usage
 * -----
 *
 * One can either directly print the indentation into a stream with
 * pretty_print_indent_fprint() or convert it into a string using
 * pretty_print_indent_to_string().
 *
 * Please note that the string returned by pretty_print_indent_to_string() is
 * \c malloc'd and should be \c free'd once it is useless.
 *
 * Example
 * -------
 *
 * ~~~{.C}
 * pretty_print_indent_fprint (stdout);
 * fprintf (stdout, "while (i < 100) {\n");
 *
 * pretty_print_indent_increase ();
 * pretty_print_indent_fprint (stdout);
 * fprintf (stdout, "++i;\n");
 *
 * pretty_print_indent_decrease ();
 * pretty_print_indent_fprint (stdout);
 * fprintf (stdout, "}\n");
 * ~~~
 *
 */

////////////////////////////////////////////////////////////////////////////////
// Macros.
////////////////////////////////////////////////////////////////////////////////

// System modificators.
// =============================================================================

/**
 * \brief Reset terminal text modifications.
 * \ingroup pretty_printing_colours
 */
#define PP_RESET "\x1B[0m"

/**
 * \brief Bold terminal text.
 * \ingroup pretty_printing_colours
 */
#define PP_BOLD "\x1B[1m"

// System colours.
// =============================================================================

/**
 * \brief Use the system black colour.
 * \ingroup pretty_printing_colours
 */
#define PP_BLACK "\x1B[30m"

/**
 * \brief Use the system red colour.
 * \ingroup pretty_printing_colours
 */
#define PP_RED "\x1B[31m"

/**
 * \brief Use the system green colour.
 * \ingroup pretty_printing_colours
 */
#define PP_GREEN "\x1B[32m"

/**
 * \brief Use the system yellow colour.
 * \ingroup pretty_printing_colours
 */
#define PP_YELLOW "\x1B[33m"

/**
 * \brief Use the system blue colour.
 * \ingroup pretty_printing_colours
 */
#define PP_BLUE "\x1B[34m"

/**
 * \brief Use the system purple colour.
 * \ingroup pretty_printing_colours
 */
#define PP_PURPLE "\x1B[35m"

/**
 * \brief Use the system cyan colour.
 * \ingroup pretty_printing_colours
 */
#define PP_CYAN "\x1B[36m"

/**
 * \brief Use the system grey colour.
 * \ingroup pretty_printing_colours
 */
#define PP_GREY "\x1B[37m"

/**
 * \brief Alias for #PP_GREY
 * \ingroup pretty_printing_colours
 */
#define PP_GRAY PP_GREY

// Pretty printer macros.
// =============================================================================

/**
 * \brief Colour for normal keywords (for, if, etc.)
 * \ingroup pretty_printing_colours
 */
#define PP_KEYWORD PP_BOLD PP_BLUE

/**
 * \brief Colour for special keywords (finish, async)
 * \ingroup pretty_printing_colours
 */
#define PP_KEYWORD_SPECIAL PP_BOLD PP_CYAN

/**
 * \brief Colour for the keywords this program aims to remove (clocked, advance)
 * \ingroup pretty_printing_colours
 */
#define PP_KEYWORD_NOT_WANTED PP_BOLD PP_RED

/**
 * \brief Colour for identifiers.
 * \ingroup pretty_printing_colours
 */
#define PP_IDENTIFIER PP_YELLOW

/**
 * \brief Colour for function calls.
 * \ingroup pretty_printing_colours
 */
#define PP_CALL PP_BOLD PP_IDENTIFIER


/**
 * \brief Colour for constants.
 * \ingroup pretty_printing_colours
 */
#define PP_CONSTANT PP_GREEN

////////////////////////////////////////////////////////////////////////////////
// Structs, ennums, typedefs, etc.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Indentation styles.
 * \ingroup pretty_printing_indent
 */
typedef enum pp_indent_style
{
    /* Please check the pretty_print.c file when modifying this enum. */
    INDENT_SPACES,      /**< Indent with spaces. */
    INDENT_TABS,        /**< Indent with tabs. */

    /* Always leave the INDENT_UNKNOWN at the end! */
    INDENT_UNKNOWN,     /**< Unknown indent style. */
} pp_indent_style;

////////////////////////////////////////////////////////////////////////////////
// Colour management.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the current colour state.
 * \retval true if colours are enabled.
 * \retval false otherwise.
 * \ingroup pretty_printing_colours
 */
bool pretty_print_colour_state (void);

/**
 * \brief Get the current colour state.
 * \retval true if colours are enabled.
 * \retval false otherwise.
 * \ingroup pretty_printing_colours
 * \see pretty_print_colour_state()
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_state ();
 * ~~~
 *
 */
static inline bool pretty_print_color_state (void)
{
    return pretty_print_colour_state ();
}

/**
 * \brief Get the current colour state.
 * \retval true if colours are enabled.
 * \retval false otherwise.
 * \ingroup pretty_printing_colours
 * \see pretty_print_colour_state()
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_state ();
 * ~~~
 *
 */
static inline bool pretty_print_colour_get_state (void)
{
    return pretty_print_colour_state ();
}

/**
 * \brief Get the current colour state.
 * \retval true if colours are enabled.
 * \retval false otherwise.
 * \ingroup pretty_printing_colours
 * \see pretty_print_colour_state()
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_state ();
 * ~~~
 *
 */
static inline bool pretty_print_color_get_state (void)
{
    return pretty_print_colour_state ();
}

/**
 * \brief Set the colour state.
 * \param state The desired colour state.
 * \ingroup pretty_printing_colours
 */
void pretty_print_colour_set_state (bool state);

/**
 * \brief Set the colour state.
 * \param state The desired colour state.
 * \ingroup pretty_printing_colours
 * \see pretty_print_colour_set_state()
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_set_state (state);
 * ~~~
 *
 */
static inline void pretty_print_color_set_state (bool state)
{
    pretty_print_colour_set_state (state);
}

/**
 * \brief Enable colours.
 * \ingroup pretty_printing_colours
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_set_state (true)
 * ~~~
 *
 */
void pretty_print_colour_enable (void);

/**
 * \brief Enable colours.
 * \see pretty_print_colour_enable()
 * \ingroup pretty_printing_colours
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_set_state (true)
 * ~~~
 *
 */
static inline void pretty_print_color_enable (void)
{
    pretty_print_colour_enable ();
}

/**
 * \brief Disable colours.
 * \ingroup pretty_printing_colours
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_set_state (false)
 * ~~~
 *
 */
void pretty_print_colour_disable (void);

/**
 * \brief Disable colours.
 * \see pretty_print_colour_disable()
 * \ingroup pretty_printing_colours
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_colour_set_state (false)
 * ~~~
 *
 */
static inline void pretty_print_color_disable (void)
{
    pretty_print_colour_disable ();
}

////////////////////////////////////////////////////////////////////////////////
// Indentation management.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Get the current indentation style.
 * \retval ::INDENT_SPACES if the current indentation style is set to spaces.
 * \retval ::INDENT_TABS if the current indentation style is set to tabs.
 * \retval ::INDENT_UNKNOWN if the current indentation style is unknown.
 * \ingroup pretty_printing_indent
 */
pp_indent_style pretty_print_indent_style (void);

/**
 * \brief Get the current indentation style.
 * \retval ::INDENT_SPACES if the current indentation style is set to spaces.
 * \retval ::INDENT_TABS if the current indentation style is set to tabs.
 * \retval ::INDENT_UNKNOWN if the current indentation style is unknown.
 * \ingroup pretty_printing_indent
 * \see pretty_print_indent_style()
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_indent_style ();
 * ~~~
 *
 */
static inline pp_indent_style pretty_print_indent_get_style (void)
{
    return pretty_print_indent_style ();
}

/**
 * \brief Set the indentation style.
 * \param style The desired indentation style.
 * \ingroup pretty_printing_indent
 */
void pretty_print_indent_set_style (pp_indent_style style);

/**
 * \brief Get the string corresponding to an indent style.
 * \return Corresponding string.
 * \ingroup pretty_printing_indent
 */
const char * pretty_print_indent_style_string (pp_indent_style style);

/**
 * \brief Get the string corresponding to an indent style.
 * \return Corresponding string.
 * \see pretty_print_indent_style_string()
 * \ingroup pretty_printing_indent
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_indent_style_string (style);
 * ~~~
 *
 */
static inline const char * pretty_print_indent_get_style_string
        (pp_indent_style style)
{
    return pretty_print_indent_style_string (style);
}

/**
 * \brief Get the current indentation level.
 * \return The current indentation level.
 * \ingroup pretty_printing_indent
 */
size_t pretty_print_indent_level (void);

/**
 * \brief Get the current indentation level.
 * \return The current indentation level.
 * \ingroup pretty_printing_indent
 * \see pretty_print_indent_level()
 *
 * \note This is equivalent to:
 * ~~~{.C}
 * pretty_print_indent_level ();
 * ~~~
 *
 */
static inline size_t pretty_print_indent_get_level (void)
{
    return pretty_print_indent_level ();
}

/**
 * \brief Increase the indentation level.
 * \ingroup pretty_printing_indent
 */
void pretty_print_indent_increase (void);

/**
 * \brief Decrease the indentation level.
 * \ingroup pretty_printing_indent
 */
void pretty_print_indent_decrease (void);

/**
 * \brief Indent the current line of a stream.
 * \param f Stream.
 * \pre The line of the stream should be empty for proper indentation.
 * \note If the indentation style is set to INDENT_UNKNOWN, the current
 * implementation defaults to INDENT_SPACES. This behaviour may be subject to
 * change.
 * \ingroup pretty_printing_indent
 */
void pretty_print_indent_fprint (FILE * f);

/**
 * \brief Get a string that represents the current level of indentation.
 * \return A string of indentation.
 * \note If the indentation style is set to INDENT_UNKNOWN, the current
 * implementation defaults to INDENT_SPACES. This behaviour may be subject to
 * change.
 * \warning The resulting string is malloc'd. It is up to the user to free it.
 * \ingroup pretty_printing_indent
 */
char * pretty_print_indent_to_string (void);

#endif /* __PRETTY_PRINT_H__ */
