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

%top{
    #include <stdlib.h>
    #include <stdio.h>
    #include <stdbool.h>
    #include <string.h>
    #include <sysexits.h>
    #include <unistd.h>
    #include <getopt.h>

    #include <isl/ctx.h>
    #include <isl/set.h>
    #include <isl/ast.h>
    #include <isl/ast_build.h>

    #include "noclock/version.h"

    #include "noclock/util.h"
    #include "noclock/debug.h"
    #include "noclock/verbose.h"
    #include "noclock/pretty_print.h"

    #include "noclock/expression.h"
    #include "noclock/expression_list.h"
    #include "noclock/instruction.h"
    #include "noclock/instruction_list.h"
    #include "noclock/instruction_to_set.h"
    #include "noclock/string_list.h"
    #include "noclock/isl_to_noclock.h"

    #include "y.tab.h"
}
%{
    extern int yyparse (void);
    extern int yylex (void);

    void line_action (void);
    void yyerror (const char * s);

    #define YY_USER_ACTION line_action ();
    size_t line_count = 1;
    size_t total_characters = 0;
    size_t line_characters = 0;

    instruction_list * program;
    isl_ctx * ctx;
    isl_printer * printer;
    string_list _parameters;
    string_list * parameters = & _parameters;

    int enable_colours = 1;
    int enable_verbose = 0;

    FILE * input_file = NULL;
    FILE * output_file = NULL;
%}

number [1-9][0-9]*|0
identifier [a-zA-Z_]+[0-9a-zA-Z_]*

%%
"if"            { return IF; }
"for"           { return FOR; }
"else"          { return ELSE; }
"in"            { return IN; }

"Program"       { return PROGRAM; }
"async"         { return ASYNC; }
"finish"        { return FINISH; }
"clocked"       { return CLOCKED; }
"advance"       { return ADVANCE; }

"true"          { return TRUE; }
"false"         { return FALSE; }

"+"             { return PLUS; }
"-"             { return MINUS; }
"*"             { return TIMES; }
"/"             { return DIV; }
"<"             { return LT; }
">"             { return GT; }
"<="            { return LE; }
">="            { return GE; }
"=="            { return EQ;}
"!="            { return NE; }
"!"             { return NOT; }
"&&"            { return AND; }
"||"            { return OR; }
"min"           { return MIN; }
"max"           { return MAX; }

{number}        { yylval._number = atoi (yytext); return NUMBER; }
{identifier}    { yylval._identifier = strdup (yytext); return IDENTIFIER; }

[.,;=(){}\[\]]  { return * yytext; }

"\n"            { ++yylineno; ++line_count; line_characters = 0; }

.               {}
%%

void line_action (void)
{
    line_characters += yyleng;
    total_characters += yyleng;
}

void yyerror (const char * s)
{
    /* Rewind input file. */
    rewind (yyin);

    /* Eat lines up to line_count. */
    for (size_t i = 1; i < line_count; ++i)
    {
        /* Separate calls to fscanf: in case the line is empty, the first
         * pattern would fail!
         */
        fscanf (yyin, "%*[^\n]");
        fscanf (yyin, "%*1[\n]");
    }

    /* Print the error's coordinates. */
    fprintf (stderr, "\x1B[1m\x1B[38;5;196mError:\x1B[0m"
        " %s near \x1B[1mline %zu\x1B[0m"
        ", \x1B[1mcharacter %zu\x1B[0m"
        ": %s\x1B[0m\n",
        s, line_count, line_characters, yytext);

    /* Print the bogous line. */
    bool keep_reading = true;
    char buffer[256];
    do
    {
        memset (buffer, 0, 256);
        keep_reading = fscanf (yyin, "%255[^\n]", buffer);
        if (keep_reading)
        {
            fprintf (stderr, "%s", buffer);
            /* If the buffer is full the line may be longer. */
            keep_reading = buffer[255] == 0;
        }
    }
    while (keep_reading);
    fprintf (stderr, "\n");

    /* Indicate the error. */
    for (size_t i = 0; i < line_characters - (strlen (yytext)); ++i)
        fprintf (stderr, " ");
    fprintf (stderr, "\x1B[1m\x1B[38;5;106m^");
    for (size_t i = 0; i < strlen (yytext) - 1; ++i)
        fprintf (stderr, "~");
    fprintf (stderr, "\x1B[0m\n");
}

void print_infos (void)
{
    fprintf (stderr, "noclock version %s\n", noclock_version ());
    fprintf (stderr, "%s", noclock_compilation_informations ());
    fprintf (stderr, "%s", noclock_libraries_informations ());
}

void debug_infos (void)
{
    fdebug (stderr, "noclock version %s\n", noclock_version ());
    fdebug (stderr, "%s", noclock_compilation_informations ());
    fdebug (stderr, "%s", noclock_libraries_informations ());
}

void print_help (void)
{
    if (enable_colours)
    {
        fprintf (stderr,
                PP_BOLD "noclock" PP_RESET " [OPTIONS] <input_file> <output_file>\n\n"
                PP_BOLD "OPTIONS\n\n" PP_RESET

                "\t" PP_BOLD "-h" PP_RESET ", " PP_BOLD "--help\n" PP_RESET
                "\t\tPrint this help.\n"

                "\t" PP_BOLD "-v" PP_RESET ", " PP_BOLD "--version\n" PP_RESET
                "\t\tPrint the version.\n"

                "\t" PP_BOLD "-i" PP_RESET ", " PP_BOLD "--input" PP_RESET " <file>\n"
                "\t\tUse <file> as the input file.\n"

                "\t" PP_BOLD "-o" PP_RESET ", " PP_BOLD "--output" PP_RESET " <file>\n"
                "\t\tUse <file> as the output file.\n"

                "\t" PP_BOLD "--verbose\n" PP_RESET
                "\t\tEnable verbose mode.\n"

                "\t" PP_BOLD "--no-colours\n" PP_RESET
                "\t\tDisable colours.\n"

                "\n" PP_BOLD "noclock" PP_RESET " version %s\n",
                noclock_version ()
                );
    }
    else
    {
        fprintf (stderr,
                "noclock" " [OPTIONS] <input_file> <output_file>\n\n"
                "OPTIONS\n\n"

                "\t" "-h" ", " "--help\n" 
                "\t\tPrint this help.\n"

                "\t" "-v" ", " "--version\n"
                "\t\tPrint the version.\n"

                "\t" "-i" ", " "--input" " <file>\n"
                "\t\tUse <file> as the input file.\n"

                "\t" "-o" ", " "--output" " <file>\n"
                "\t\tUse <file> as the output file.\n"

                "\t" "--verbose\n"
                "\t\tEnable verbose mode.\n"

                "\t" "--no-colours\n"
                "\t\tDisable colours.\n"

                "\n" "noclock" " version %s\n",
                noclock_version ()
                );
    }

}

void parse_args (int argc, char ** argv)
{
    static const struct option noclock_options[] =
    {
        { "no-colours",    no_argument, & enable_colours, 0, },
        { "verbose",   no_argument, & enable_verbose, 1, },
        { "input",    required_argument, NULL, 'i', },
        { "output",    required_argument, NULL, 'o', },
        { "version",   no_argument, NULL, 'v', },
        { "help",      no_argument, NULL, 'h', },
        { 0, 0, 0, 0, },
    };

    bool unknown_option = false;
    int val = 1;
    opterr = 0;

    do
    {
        int longindex;
        val = getopt_long (argc, argv, "iovh", noclock_options,
                & longindex);

        switch (val)
        {
            case 'i':
                input_file = fopen (argv[optind], "r");
                __forbid_value (input_file, NULL, "fopen", EX_OSERR);
                break;
            case 'o':
                output_file = fopen (argv[optind], "w");
                __forbid_value (output_file, NULL, "fopen", EX_OSERR);
                break;
            case 'v':
                print_infos ();
                exit (EXIT_SUCCESS);
                break;
            case 'h':
                print_help ();
                exit (EXIT_SUCCESS);
                break;
            case '?':
                if (enable_colours)
                    fprintf (stderr, "\x1B[1m\x1B[31mError: Unknown option "
                        "\"%s\".\x1B[0m\n",
                        argv[optind-1]);
                else
                    fprintf (stderr, "Error: Unknown option \"%s\".\n",
                        argv[optind-1]);
                unknown_option = true;
                break;
            default:
                break;
        }
    }
    while (val != - 1);

    if (enable_verbose)
        verbose_mode_enable();

    if (unknown_option)
        exit (EX_USAGE);
}

void verbose_header (FILE * file, const char * header)
{
    char * copy = strdup (header);
    for (size_t i = 0; copy[i]; ++i)
        copy[i] = '=';

    fverbosef (file, PP_BOLD "\n%s\n%s\n" PP_RESET, header, copy);
}

int main (int argc, char ** argv)
{
    /* Parse command line arguments and expect at least one non-option
     * argument if no file has been opened.
     */
    parse_args (argc, argv);
    if (input_file == NULL && optind >= argc)
    {
        fprintf (stderr, "Error: wrong number of arguments.\n");
        exit (EX_USAGE);
    }

    /* Set yyin to the input file or open the input file. */
    yyin = input_file;
    if (yyin == NULL)
    {
        yyin = fopen (argv[optind++], "r");
        __forbid_value (yyin, NULL, "fopen", EX_OSERR);
    }

    /* Open the output file if there is one and it has not bee opened yet. */
    if (output_file == NULL && optind < argc)
    {
        output_file = fopen (argv[optind++], "w");
        __forbid_value (output_file, NULL, "fopen", EX_OSERR);
    }

    /* If the destination output is not the console, colours must be disabled. */
    if (output_file != NULL)
        yyout = output_file;

    /* Always print no clock information in DEBUG mode. */
    debug_infos();

    /* Initialize ISL. */
    ctx = isl_ctx_alloc ();
    printer = isl_printer_to_file (ctx, stderr);

    /* Initialize the parameter list. */
    string_list_init (parameters);
    string_list_append (parameters, "f");
    string_list_append (parameters, "a");

    /* Parse the input. */
    yyparse ();

    /* Compute dates and add annotations to the AST. */
    instruction_list_compute_dates (program, NULL, NULL);
    instruction_list_decorate (program, NULL, NULL);

    string_list s_list;
    string_list_init (& s_list);

    verbose_header (stderr, "Original program");
    if (verbose_mode_state ())
    {
        if (enable_colours)
            pretty_print_colour_enable ();
        instruction_list_fprint (stderr, program);
        pretty_print_colour_disable ();
    }

    /* Extract the *S* instructions and unite them.
     * (In verbose mode, the *S* instructions will be printed.)
     */
    verbose_header (stderr, "Instructions");
    isl_set_list * sets = program_to_set_list (ctx, parameters, program,
        & s_list);
    isl_union_set * unions = union_set_list (sets);

    /* Print the union (only in verbose mode). */
    verbose_header (stderr, "ISL Union");
    if (verbose_mode_state ())
        isl_printer_print_union_set (printer, unions);
    fverbosef (stderr, "\n");

    /* Create the ISL AST. */
    isl_union_map * schedule = isl_union_set_identity (unions);
    isl_space * space = isl_union_map_get_space (schedule);
    isl_set * context = isl_set_universe (isl_space_params (space));
    isl_ast_build * build = isl_ast_build_from_context (context);
    isl_ast_node * ast = isl_ast_build_ast_from_schedule (build, schedule);

    /* Print the ISL AST (only in verbose mode). */
    verbose_header (stderr, "ISL Code");
    if (verbose_mode_state ())
    {
        int format = isl_printer_get_output_format (printer);
        printer = isl_printer_set_output_format (printer, ISL_FORMAT_C);
        printer = isl_printer_print_ast_node (printer, ast);
        printer = isl_printer_set_output_format (printer, format);
    }

    /* Convert the ISL AST to a NoClock AST and get the list of *S*
     * instructions. */
    instruction_list * final_ast = isl_ast_to_noclock_ast (ast);
    instruction_list * calls = call_list (final_ast);

    /* Print the initial NoClock AST (only in verbose mode). */
    verbose_header (stderr, "ISL AST => NoClock AST");
    if (enable_colours)
        pretty_print_colour_enable ();
    if (verbose_mode_state ())
        instruction_list_fprint (stderr, final_ast);
    pretty_print_colour_disable ();

    /* Adjust the NoClock AST. */
    instruction_list_fill (final_ast, calls);
    instruction_list_strip (calls, & s_list);

    /* Print the final result. */
    if (verbose_mode_state ())
    {
        verbose_header (stderr, "Final program");
        if (enable_colours)
            pretty_print_colour_enable ();
        instruction_list_fprint (stderr, final_ast);
        pretty_print_colour_disable ();
    }

    if (output_file != NULL)
    {
        pretty_print_colour_disable ();
        instruction_list_fprint (yyout, final_ast);
    }
    else if (! verbose_mode_state ())
    {
        if (enable_colours)
            pretty_print_colour_enable ();
        instruction_list_fprint (yyout, final_ast);
        pretty_print_colour_disable ();
    }

    /* AST clean up. */
    instruction_list_free (program);
    instruction_list_soft_free (calls);
    instruction_list_free (final_ast);
    string_list_clean (parameters);
    string_list_clean (& s_list);

    /* ISL clean up. */
    /* isl_ast_node_free (ast); */
    /* isl_ast_build_free (build); */
    /* isl_set_free (context); */
    /* isl_space_free (space); */
    /* isl_union_map_free (schedule); */
    /* isl_union_set_free (unions); */
    /* isl_set_list_free (sets); */
    /* isl_printer_free (printer); */
    /* isl_ctx_free (ctx); */

    /* lex/yacc clean up. */
    fclose (yyin);
    fclose (yyout);
    yylex_destroy ();

    exit (EXIT_SUCCESS);
}
