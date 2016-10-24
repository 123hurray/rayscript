%{
#include<stdio.h>
#include<stdlib.h>
#include"statement.h"
#include"visitor.h"
#include"opcode.h"
#include"eval.h"
syn_node root;
syn_node * new_syn_node(enum syntax_type type, void *val) {
    syn_node *s = (syn_node *)malloc(sizeof(syn_node));
    s->type = type;
    if(s->type == EXP) {
        s->exp = (exp_node*)val;
    } else if(s->type == TERM) {
        s->term = (term_node*)val;
    } else if(s->type == FACTOR) {
        s->factor = (factor_node*)val;
    }
    return s;
}
int yylex();
int yyerror();
%}
 /* declare tokens */
%token NUMBER 
%token ADD SUB MUL DIV OP CP 
%token EOL 
%%
calclist: 
 | calclist exp EOL {root = *$2; $$ = $2; } 
 ;
exp: term { 
    exp_node *e = (exp_node*)malloc(sizeof(exp_node));
    e->type = 0;
    e->op1 = NULL;
    e->op2 = $1->term; 
    $$ = new_syn_node(EXP, e);
} 
 | exp ADD term {
    exp_node *e = (exp_node*)malloc(sizeof(exp_node));
    e->type = ADD;
    e->op1 = $1->exp;
    e->op2 = $3->term; 
    $$ = new_syn_node(EXP, e);
} 
 | exp SUB term {
    exp_node *e = (exp_node*)malloc(sizeof(exp_node));
    e->type = SUB;
    e->op1 = $1->exp;
    e->op2 = $3->term; 
    $$ = new_syn_node(EXP, e);
} 
;
term: factor {
    term_node *e = (term_node*)malloc(sizeof(term_node));
    e->type = 0;
    e->op1 = NULL;
    e->op2 = $1->factor; 
    $$ = new_syn_node(TERM, e);
} 
 | term MUL factor {
    term_node *e = (term_node*)malloc(sizeof(term_node));
    e->type = MUL;
    e->op1 = $1->term;
    e->op2 = $3->factor; 
    $$ = new_syn_node(TERM, e);
} 
 | term DIV factor {
    term_node *e = (term_node*)malloc(sizeof(term_node));
    e->type = DIV;
    e->op1 = $1->term;
    e->op2 = $3->factor; 
    $$ = new_syn_node(TERM, e);
} 
;
factor : NUMBER {
    $$ = $1;
} 
 | OP exp CP   {
    factor_node* e = (factor_node*)malloc(sizeof(factor_node));
    e->type = EXP;
    e->exp = $2->exp;
    $$ = new_syn_node(FACTOR, e);
} 
;
%%
int main(int argc, char **argv)
{
    yyparse();
    code_block *b = init_code_block();
    visit(b, &root);
    eval(b);
    return 0;
}
int yyerror(char *s)
{
    return fprintf(stderr, "error: %s\n", s);
}
