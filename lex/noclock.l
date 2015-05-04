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

int main (int argc, char ** argv)
{
    --argc; ++argv;

    /* Expect at least one argument. */
    if (argc < 1)
    {
        fprintf (stderr, "Error: wrong number of arguments.\n");
    }

    /* Open input. */
    yyin = fopen (argv[0], "r");
    if (yyin == NULL)
    {
        perror ("fopen");
        exit (EX_OSERR);
    }

    fdebug (stderr, "noclock version %s\n", noclock_version ());
    fdebug (stderr, "%s", noclock_compilation_informations ());
    fdebug (stderr, "%s", noclock_libraries_informations ());

    verbose_mode_enable ();

    /* ISL. */
    ctx = isl_ctx_alloc ();
    printer = isl_printer_to_file (ctx, stderr);

    string_list_init (parameters);
    string_list_append (parameters, "f");
    string_list_append (parameters, "a");

    /* Parse. */
    yyparse ();

    instruction_list_compute_dates (program, NULL, NULL);
    instruction_list_decorate (program, NULL, NULL);

    string_list s_list;
    string_list_init (& s_list);

    fverbosef (stderr, PP_BOLD "\nOriginal program\n================\n"
            PP_RESET);
    if (verbose_mode_state ())
    {
        pretty_print_colour_enable ();
        instruction_list_fprint (stderr, program);
        pretty_print_colour_disable ();
    }

    fverbosef (stderr, PP_BOLD "\nInstructions\n============\n" PP_RESET);

    isl_set_list * sets = program_to_set_list (ctx, parameters, program,
        & s_list);


    isl_union_set * unions = union_set_list (sets);

    fverbosef (stderr, PP_BOLD "\nISL Union\n=========\n" PP_RESET);
    if (verbose_mode_state ())
        isl_printer_print_union_set (printer, unions);
    fverbosef (stderr, "\n");

    isl_union_map * schedule = isl_union_set_identity (unions);
    isl_space * space = isl_union_map_get_space (schedule);
    isl_set * context = isl_set_universe (isl_space_params (space));
    isl_ast_build * build = isl_ast_build_from_context (context);
    isl_ast_node * ast = isl_ast_build_ast_from_schedule (build, schedule);

    fverbosef (stderr, PP_BOLD "\nISL code\n========\n" PP_RESET);
    if (verbose_mode_state ())
    {
        int format = isl_printer_get_output_format (printer);
        printer = isl_printer_set_output_format (printer, ISL_FORMAT_C);
        printer = isl_printer_print_ast_node (printer, ast);
        printer = isl_printer_set_output_format (printer, format);
    }

    instruction_list * final_ast = isl_ast_to_noclock_ast (ast);

    instruction_list * calls = call_list (final_ast);

    fverbosef (stdout,
            PP_BOLD "\nISL AST => NoClock AST\n======================\n" PP_RESET);
    pretty_print_colour_enable ();
    instruction_list_fprint (stdout, final_ast);
    pretty_print_colour_disable ();

    instruction_list_fill (final_ast, calls);
    instruction_list_strip (calls, & s_list);

    fverbosef (stdout, PP_BOLD "\nFinal program\n=============\n" PP_RESET);
    pretty_print_colour_enable ();
    instruction_list_fprint (stdout, final_ast);
    pretty_print_colour_disable ();

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
