%{
#include<stdio.h>
#include<stdlib.h>
#include"statement.h"
#include"visitor.h"
#include"opcode.h"
#include"eval.h"
static ast_node* head, *cur;
int yylex();
int yyerror();
%}
 /* declare tokens */
%token NUMBER 
%token ADD SUB MUL DIV OP CP ASSIGN IDENTIFIER 
%token EOL 
%token TYPE_TERM TYPE_FACTOR
%%

calclist: 
 | calclist exp EOL {
    cur->next = $2;
    cur = cur->next;
    $$ = $2; 
} 
 | calclist assign EOL {
    cur->next = $2;
    cur = cur->next;
    $$ = $2; 
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
    f->ast_type = ID;
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
    head = (ast_node *)malloc(sizeof(ast_node));
    cur = head;
    yyparse();
    code_block *b = init_code_block();
    visit(b, head->next);
    eval(b);
    return 0;
}
int yyerror(char *s)
{
    return fprintf(stderr, "error: %s\n", s);
}
