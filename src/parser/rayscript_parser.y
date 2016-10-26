%{
#ifdef RDEBUG
#define YYDEBUG 1
int yydebug = 1;
#endif
#include <stdio.h>
#include <stdlib.h>
#include <statement.h>
#include <visitor.h>
#include <opcode.h>
#include <vm.h>
#include <eval.h>
static statement_list_node* root;
int yylex();
int yyerror();
%}
 /* declare tokens */
%token NUMBER 
%token ADD SUB MUL DIV OP CP ASSIGN IDENTIFIER 
%token EOL 
%token IF_TOKEN ELSE_TOKEN LB RB SC 
%token PRINT_TOKEN
%token TYPE_TERM TYPE_FACTOR
%start program
%%
program: statement_list {
    root = (statement_list_node*)$1;
};
statement_list: statement { 
    statement_list_node* e = MAKE_AST_NODE(statement_list_node);
    e->ast_type = STATEMENT_LIST;
    e->next = e->tail = (statement_node *)$1;
    $$ = (ast_node*)e;
}
| statement_list statement {
    statement_list_node* s = (statement_list_node *)$1;
    s->tail->next = (statement_node *)$2;
    s->tail = (statement_node *)$2;
    $$ = $1;
}
;
compound_statement: LB RB  {
    compound_statement_node* e = MAKE_AST_NODE(compound_statement_node);
    e->ast_type = COMPOUND_STATEMENT;
    e->list = NULL;
    $$ = (ast_node *)e;
}
| LB statement_list RB {
    compound_statement_node* e = MAKE_AST_NODE(compound_statement_node);
    e->ast_type = COMPOUND_STATEMENT;
    e->list = $2;
    $$ = (ast_node *)e;
}
;
statement: 
| compound_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->ast_type = STATEMENT;
    e->type = COMPOUND_STATEMENT;
    e->next = NULL;
    e->val = (ast_node *)$1;
    $$ = (ast_node *)e; 
}
| exp {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->ast_type = STATEMENT;
    e->type = EXP;
    e->next = NULL;
    e->val = (ast_node *)$1;
    $$ = (ast_node *)e; 
} 
| assign {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->ast_type = STATEMENT;
    e->type = ASSIGNMENT;
    e->next = NULL;
    e->val = (ast_node *)$1;
    $$ = (ast_node *)e; 
} 
| if_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->ast_type = STATEMENT;
    e->type = TYPE_IF;
    e->next = NULL;
    e->val = (ast_node *)$1;
    $$ = (ast_node *)e; 
}
| print_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->ast_type = STATEMENT;
    e->type = TYPE_IF;
    e->next = NULL;
    e->val = (ast_node *)$1;
    $$ = (ast_node *)e; 
}
;
print_statement: PRINT_TOKEN exp {
    print_statement_node *e = MAKE_AST_NODE(print_statement_node);
    e->ast_type = TYPE_PRINT;
    e->exp = (exp_node *)$2;
    $$ = (ast_node *)e;
}
if_statement: IF_TOKEN exp compound_statement{
    if_statement_node *e = MAKE_AST_NODE(if_statement_node);
    e->ast_type = TYPE_IF;
    e->test_exp = (exp_node *)$2;
    e->then = (compound_statement_node *)$3;
    e->els = NULL; 
    $$ = (ast_node *)e;
}
| IF_TOKEN exp compound_statement ELSE_TOKEN compound_statement {
    if_statement_node *e = MAKE_AST_NODE(if_statement_node);
    e->ast_type = TYPE_IF;
    e->test_exp = (exp_node *)$2;
    e->then = (compound_statement_node *)$3;
    e->els = (compound_statement_node *)$5;
    $$ = (ast_node *)e;
}
; 



exp: term { 
    exp_node *e = (exp_node*)malloc(sizeof(exp_node));
    e->ast_type = EXP;
    e->type = TYPE_TERM;
    e->op1 = NULL;
    e->op2 = (term_node*)$1; 
    $$ = (ast_node *)e;
} 
 | exp ADD term {
    exp_node *e = (exp_node*)malloc(sizeof(exp_node));
    e->ast_type = EXP;
    e->type = ADD;
    e->op1 = (exp_node*)$1;
    e->op2 = (term_node*)$3; 
    $$ = (ast_node *)e; 
} 
 | exp SUB term {
    exp_node *e = (exp_node*)malloc(sizeof(exp_node));
    e->ast_type = EXP;
    e->type = SUB;
    e->op1 = (exp_node *)$1;
    e->op2 = (term_node *)$3; 
    $$ = (ast_node *)e;
} 
;
term: factor {
    term_node *e = (term_node*)malloc(sizeof(term_node));
    e->ast_type = TERM;
    e->type = TYPE_FACTOR;
    e->op1 = NULL;
    e->op2 = (factor_node *)$1; 
    $$ = (ast_node *)e;
} 
 | term MUL factor {
    term_node *e = (term_node*)malloc(sizeof(term_node));
    e->ast_type = TERM;
    e->type = MUL;
    e->op1 = (term_node *)$1;
    e->op2 = (factor_node *)$3; 
    $$ = (ast_node *)e;
} 
 | term DIV factor {
    term_node *e = (term_node*)malloc(sizeof(term_node));
    e->ast_type = TERM;
    e->type = DIV;
    e->op1 = (term_node *)$1;
    e->op2 = (factor_node *)$3; 
    $$ = (ast_node *)e;
} 
;
factor : NUMBER {
    $$ = $1;
}
| IDENTIFIER {
    factor_node *f = (factor_node*)malloc(sizeof(factor_node));
    f->ast_type = FACTOR;
    f->type = ID;
    f->val = (ray_object*)new_string_object(((identifier_node *)$1)->val);
    $$ = (ast_node *)f;
}
| OP exp CP   {
    factor_node* e = (factor_node*)malloc(sizeof(factor_node));
    e->ast_type = FACTOR;
    e->type = EXP;
    e->exp = (exp_node *)$2;
    $$ = (ast_node *)e;
}
;

assign: IDENTIFIER ASSIGN exp {
    assign_node* e = (assign_node *)malloc(sizeof(assign_node));
    e->ast_type = ASSIGNMENT;
    e->lval = (identifier_node *)$1;
    e->rval = (exp_node *)$3;
    $$ = (ast_node *)e;
}
;
%%
int main(int argc, char **argv)
{
    yyparse();
    compiler *c = new_compiler();
    visit_statement_list(c, root);
    eval(c);
    return 0;
}
int yyerror(char *s)
{
    return fprintf(stderr, "error: %s\n", s);
}
