/**
 * \file pretty_print.c
 * \brief Pretty printing.
 * \author Harenome RAZANAJATO RANAIVOARIVONY
 * \date 2015
 * \copyright MIT License
 * \since version `1.0.0`
 *
 * This file defines the contents of the \ref pretty_printing_group module
 * and its child modules \ref pretty_printing_colours and
 * \ref pretty_printing_indent.
 *
 * Internal states
 * ---------------
 *
 * The internal states of the colour mode and the indentation level are
 * respectively stored in the static variables #colour_state,
 * #indentation_level, and #indentation_style.
 * These variables are static and should be manipulated
 * using only the functions declared in the pretty_print.h header.
 *
 * Conversion ::pp_indent_style -> const char *
 * --------------------------------------------
 *
 * The array #indentation_strings should be edited with care whenever
 * the ::pp_indent_style enumeration is modified: all values lower than
 * ::INDENT_UNKNOWN should have a corresponding string in this array. (This
 * is also the reason why ::INDENT_UNKNOWN should always be the last value
 * of the enumeration, as this makes various checks much easier...)
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

#include "noclock/pretty_print.h"

////////////////////////////////////////////////////////////////////////////////
// Static variables.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief The current colour state.
 * \since version `1.0.0`
 */
static bool colour_state = false;

/**
 * \brief The current indentation level.
 * \since version `1.0.0`
 */
static size_t indentation_level = 0;

/**
 * \brief The current indentation style.
 * \since version `1.0.0`
 */
static pp_indent_style indentation_style = INDENT_SPACES;

/**
 * \brief The indentation style string representations.
 * \since version `1.0.0`
 *
 * \warning Do not forget to edit this array accordingly when adding new
 * values (or modifying them) to the ::pp_indent_style enum!
 */
static const char * indentation_strings[] =
{
    [INDENT_SPACES] = "    ",
    [INDENT_TABS] = "\t",

    /* Assuming INDENT_UNKNOWN is the last value, the size of this array is
     * INDENT_UNKNOWN.
     * Please make sure all other indexes have a string: this way, one only has
     * to ensure that the index is lower or equal to INDENT_UNKNOWN when using
     * this array.
     */
    [INDENT_UNKNOWN] = "    ",
};

////////////////////////////////////////////////////////////////////////////////
// Static functions.
////////////////////////////////////////////////////////////////////////////////

/**
 * \brief Fix pp_indent_style values, if necessary.
 * \since version `1.0.0`
 *
 * \param style The value to fix.
 * \return The fixed value.
 */
static inline pp_indent_style pp_indent_style_fix (pp_indent_style style);

////////////////////////////////////////////////////////////////////////////////
// Colour management.
////////////////////////////////////////////////////////////////////////////////

bool pretty_print_colour_state (void)
{
    return colour_state;
}

void pretty_print_colour_set_state (bool state)
{
    colour_state = state;
}

void pretty_print_colour_enable (void)
{
    pretty_print_colour_set_state (true);
}

void pretty_print_colour_disable (void)
{
    pretty_print_colour_set_state (false);
}

////////////////////////////////////////////////////////////////////////////////
// Indentation management.
////////////////////////////////////////////////////////////////////////////////

pp_indent_style pretty_print_indent_style (void)
{
    return indentation_style;
}

void pretty_print_indent_set_style (pp_indent_style style)
{
    style = pp_indent_style_fix (style);
    indentation_style = style;
}

const char * pretty_print_indent_style_string (pp_indent_style style)
{
    style = pp_indent_style_fix (style);
    return indentation_strings[style];
}

size_t pretty_print_indent_level (void)
{
    return indentation_level;
}

void pretty_print_indent_increase (void)
{
    indentation_level++;
}

void pretty_print_indent_decrease (void)
{
    indentation_level--;
}

void pretty_print_indent_fprint (FILE * f)
{
    /* Alternative implementation:
     * Use pretty_print_indent_to_string(), print the resulting string and free
     * the string...
     */

    /* Of course we could directly access the variables. But this should be
     * more robust to possible future implementation changes!
     */
    size_t level = pretty_print_indent_level ();
    pp_indent_style style = pretty_print_indent_style ();

    for (size_t i = 0; i < level; ++i)
        fprintf (f, "%s", indentation_strings[style]);
}

char * pretty_print_indent_to_string (void)
{
    size_t level = pretty_print_indent_level ();
    pp_indent_style style = pretty_print_indent_style ();
    const char * style_string = pretty_print_indent_style_string (style);

    size_t indentation_string_size = strlen (style_string);
    size_t string_size = indentation_string_size * level;

    char * indent_string = malloc (string_size + 1);
    __forbid_value (indent_string, NULL, "malloc", EX_OSERR);
    memset (indent_string, '\0', string_size + 1);

    for (size_t i = 0; i < level; ++i)
        sprintf (indent_string + i * indentation_string_size, "%s", style_string);

    return indent_string;
}

////////////////////////////////////////////////////////////////////////////////
// Static functions definitions.
////////////////////////////////////////////////////////////////////////////////

pp_indent_style pp_indent_style_fix (pp_indent_style style)
{
    /* It has been arbitrarily decided that the default style shall
     * be INDENT_SPACES.
     */
    if (style >= INDENT_UNKNOWN || style < 0)
        style = INDENT_SPACES;

    return style;
}
