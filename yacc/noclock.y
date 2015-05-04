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

%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>
    #include <sysexits.h>

    #include <isl/ctx.h>
    #include <isl/set.h>

    extern char * yytext;

    extern int yylex(void);
    extern void yyerror (const char *);
    extern void yacc_init (void);

    #include "noclock/expression.h"
    #include "noclock/expression_list.h"
    #include "noclock/instruction.h"
    #include "noclock/instruction_list.h"
    #include "noclock/instruction_to_set.h"
    #include "noclock/string_list.h"

    extern instruction_list * program;
    extern isl_ctx * ctx;
    extern isl_printer * printer;
    extern string_list * parameters;
%}

%token IMPORT PUBLIC CLASS STATIC DEF
%token CLOCKED FINISH ASYNC ADVANCE
%token IF ELSE FOR IN
%token PLUS MINUS TIMES DIV LT GT EQ NE LE GE NOT AND OR
%token MIN MAX
%token UNARY
%token TRUE FALSE
%token IDENTIFIER
%token NUMBER
%token PROGRAM

%left OR
%left AND
%left LT GT EQ NE LE GE
%left APPLY
%left PLUS MINUS
%left TIMES DIV
%left INCR DECR
%left NOT
%left UNARY
%left MIN MAX

%union
{
    long int _number;
    char * _identifier;
    instruction_list * _instruction_list;
    expression * _expression;
    expression_list * _expression_list;
    bool _boolean;
}

%type <_instruction_list> block
%type <_instruction_list> instruction
%type <_instruction_list> control
%type <_instruction_list> control_block
%type <_expression> arith_expr
%type <_expression_list> arith_expr_list
%type <_expression> bool_expr
%type <_identifier> IDENTIFIER
%type <_number> NUMBER
%type <_boolean> maybe_clocked

%start start

%%

start
    : PROGRAM '[' string_list ']' '{' block '}'
    {
        program = $6;
    }
;

string_list
    : IDENTIFIER
    {
        string_list_append (parameters, $1);
    }
    | IDENTIFIER ',' string_list
    {
        string_list_append (parameters, $1);
    }
;

block
    : instruction ';' block
    {
        $$ = instruction_list_cat ($1, $3);
    }
    | instruction ';'
    {
        $$ = $1;
    }
    | control block
    {
        $$ = instruction_list_cat ($1, $2);
    }
    | control
    {
        $$ = $1;
    }
;

control
    : FOR '(' IDENTIFIER IN arith_expr '.''.' arith_expr ')' control_block
    {
        $$ = instruction_list_alloc ();
        instruction * i = instruction_alloc ();
        $$->element = i;
        i->type = INSTR_FOR;
        i->content.loop.identifier = $3;
        i->content.loop.left_boundary = $5;
        i->content.loop.right_boundary = $8;
        i->content.loop.body = $10;
    }
    | IF '(' bool_expr ')' control_block
    {
        $$ = instruction_list_alloc ();
        instruction * i = instruction_alloc ();
        $$->element = i;
        i->type = INSTR_IF;
        i->content.branch.has_else = false;
        i->content.branch.condition = $3;
        i->content.branch.true_body = $5;
    }
    | IF '(' bool_expr ')' control_block ELSE control_block
    {
        $$ = instruction_list_alloc ();
        instruction * i = instruction_alloc ();
        $$->element = i;
        i->type = INSTR_IF;
        i->content.branch.has_else = true;
        i->content.branch.condition = $3;
        i->content.branch.true_body = $5;
        i->content.branch.false_body = $7;
    }
    | maybe_clocked FINISH control_block
    {
        $$ = instruction_list_alloc ();
        instruction * i = instruction_alloc ();
        $$->element = i;
        i->type = $1 ? INSTR_CLOCKED_FINISH : INSTR_FINISH;
        i->content.block = $3;
    }
    | maybe_clocked ASYNC control_block
    {
        $$ = instruction_list_alloc ();
        instruction * i = instruction_alloc ();
        $$->element = i;
        i->type = $1 ? INSTR_CLOCKED_ASYNC : INSTR_ASYNC;
        i->content.block = $3;
    }
;

maybe_clocked
    : CLOCKED
    {
        $$ = true;
    }
    |
    {
        $$ = false;
    }
;

control_block
    : instruction ';'
    {
        $$ = $1;
    }
    | '{' block '}'
    {
        $$ = $2;
    }
;

instruction
    : ADVANCE
    {
        $$ = instruction_list_alloc ();
        instruction * i = instruction_alloc ();
        i->type = INSTR_ADVANCE;
        $$->element = i;
    }
    | IDENTIFIER '(' arith_expr_list ')'
    {
        $$ = instruction_list_alloc ();
        instruction * i = instruction_alloc ();
        $$->element = i;
        i->type = INSTR_CALL;
        i->content.call.identifier = $1;
        i->content.call.arguments = $3;
    }
;

arith_expr_list
    : arith_expr
    {
        $$ = expression_list_alloc ();
        $$->element = $1;
    }
    | arith_expr ',' arith_expr_list
    {
        $$ = expression_list_alloc ();
        $$->element = $1;
        $$ = expression_list_cat ($$, $3);
    }
    |
    {
        $$ = NULL;
    }
;

bool_expr
    : bool_expr OR bool_expr
    {
        $$ = expression_or ($1, $3);
    }
    | bool_expr AND bool_expr
    {
        $$ = expression_and ($1, $3);
    }
    | NOT bool_expr %prec UNARY
    {
        $$ = expression_not ($2);
    }
    | arith_expr LT arith_expr
    {
        $$ = expression_lt ($1, $3);
    }
    | arith_expr GT arith_expr
    {
        $$ = expression_gt ($1, $3);
    }
    | arith_expr EQ arith_expr
    {
        $$ = expression_eq ($1, $3);
    }
    | arith_expr NE arith_expr
    {
        $$ = expression_ne ($1, $3);
    }
    | arith_expr LE arith_expr
    {
        $$ = expression_le ($1, $3);
    }
    | arith_expr GE arith_expr
    {
        $$ = expression_ge ($1, $3);
    }
    | '(' bool_expr ')'
    {
        $$ = $2;
    }
    | TRUE
    {
        $$ = expression_from_boolean (true);
    }
    | FALSE
    {
        $$ = expression_from_boolean (false);
    }
;

arith_expr
    : arith_expr PLUS arith_expr
    {
        $$ = expression_add ($1, $3);
    }
    | arith_expr MINUS arith_expr
    {
        $$ = expression_sub ($1, $3);
    }
    | arith_expr TIMES arith_expr
    {
        $$ = expression_mult ($1, $3);
    }
    | arith_expr DIV arith_expr
    {
        $$ = expression_div ($1, $3);
    }
    | PLUS arith_expr %prec UNARY
    {
        $$ = $2;
    }
    | MINUS arith_expr %prec UNARY
    {
        $$ = expression_neg ($2);
    }
    | MIN '(' arith_expr ',' arith_expr ')'
    {
        $$ = expression_min ($3, $5);
    }
    | MAX '(' arith_expr ',' arith_expr ')'
    {
        $$ = expression_max ($3, $5);
    }
    | '(' arith_expr ')'
    {
        $$ = $2;
    }
    | NUMBER
    {
        $$ = expression_from_number ($1);
    }
    | IDENTIFIER
    {
        $$ = expression_from_identifier ($1);
        free ($1);
    }
;

%%
