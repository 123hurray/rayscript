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
int is_interactive = 0;
static statement_list_node* root;
static statement_list_node* interactive_root;
int yylex();
int yyerror();
%}
%define api.push-pull both
 /* declare tokens */
%token INTERP_START
%token INT_TOKEN FLOAT_TOKEN 
%token OPERATOR OP CP ASSIGN IDENTIFIER 
%token EOL 
%token IF_TOKEN ELSE_TOKEN LB RB SC 
%token PRINT_TOKEN
%token TYPE_TERM TYPE_FACTOR
%token AND_TOKEN OR_TOKEN EQUALS_TOKEN
%left OPERATOR_1
%left OPERATOR_2
%union{
    statement_list_node* sln;
    if_statement_node* isn;
    compound_statement_node* csn;
    print_statement_node* psn;
    assign_node* an;
    statement_node* sn;
    exp_node* en;
    factor_node* fn;
    char* str;
    double fnum;
    long inum;
    operator_type op;
}
%type <sln> program;
%type <sln> statement_list;
%type <csn> compound_statement;
%type <psn> print_statement;
%type <isn> if_statement;
%type <an> assign;
%type <sn> statement;
%type <en> exp;
%type <fn> factor;
%type <fnum> FLOAT_TOKEN;
%type <inum> INT_TOKEN;
%type <str> IDENTIFIER;
%type <op> OPERATOR_1;
%type <op> OPERATOR_2;
%nonassoc EOL
%start program
%%
program: statement_list {
    root = $1;
} 
| INTERP_START statement EOL{
    statement_list_node* e = MAKE_AST_NODE(statement_list_node);
    e->next = $2;
    interactive_root = e;
}
statement_list: statement EOL { 
    statement_list_node* e = MAKE_AST_NODE(statement_list_node);
    e->next = e->tail = $1;
    $$ = e;
}
| statement_list statement EOL {
    statement_list_node* s = $1;
    s->tail->next = $2;
    s->tail = $2;
    $$ = $1;
}
;
compound_statement: LB RB  {
    compound_statement_node* e = MAKE_AST_NODE(compound_statement_node);
    e->list = NULL;
    $$ = e;
}
| LB EOL statement_list RB {
    compound_statement_node* e = MAKE_AST_NODE(compound_statement_node);
    e->list = $3;
    $$ = e;
}
;
statement: compound_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->type = STATEMENT_TYPE_COMPOUND;
    e->next = NULL;
    e->csn = $1;
    $$ = e; 
}
| exp {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->type = STATEMENT_TYPE_EXP;
    e->next = NULL;
    e->en = $1;
    $$ = e; 
} 
| assign {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->type = STATEMENT_TYPE_ASSIGN;
    e->next = NULL;
    e->an = $1;
    $$ = e; 
} 
| if_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->type = STATEMENT_TYPE_IF;
    e->next = NULL;
    e->isn = $1;
    $$ = e; 
}
| print_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->type = STATEMENT_TYPE_PRINT;
    e->next = NULL;
    e->psn = $1;
    $$ = e; 
}
|{
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->type = STATEMENT_TYPE_EMPTY;
    e->next = NULL;
    $$ = e; 
};
print_statement: PRINT_TOKEN exp {
    print_statement_node *e = MAKE_AST_NODE(print_statement_node);
    e->exp = $2;
    $$ = e;
};
if_statement: IF_TOKEN exp compound_statement {
    if_statement_node *e = MAKE_AST_NODE(if_statement_node);
    e->test_exp = $2;
    e->then = $3;
    e->els = NULL; 
    $$ = e;
}
| IF_TOKEN exp compound_statement ELSE_TOKEN compound_statement {
    if_statement_node *e = MAKE_AST_NODE(if_statement_node);
    e->test_exp = $2;
    e->then = $3;
    e->els = $5;
    $$ = e;
}; 


exp: factor { 
    exp_node *e = (exp_node*)malloc(sizeof(exp_node));
    e->type = EXP_TYPE_FACTOR;
    e->op1 = NULL;
    e->op2 = NULL; 
    e->op3 = 0; 
    e->op4= $1;
    $$ = e;
} 
 | exp OPERATOR_1 exp {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->type = EXP_TYPE_OP;
    e->op1 = $1;
    e->op2 = $3; 
    e->op3 = $2;
    $$ = e; 
}; 
 | exp OPERATOR_2 exp {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->type = EXP_TYPE_OP;
    e->op1 = $1;
    e->op2 = $3; 
    e->op3 = $2;
    $$ = e; 
}; 
factor : INT_TOKEN {
    factor_node *f = MAKE_AST_NODE(factor_node);
    f->type = FACTOR_TYPE_INT;
    f->val = (ray_object*)new_number_object($1);
    $$ = f;
}
| FLOAT_TOKEN {
    factor_node *f = MAKE_AST_NODE(factor_node);
    f->type = FACTOR_TYPE_FLOAT;
    f->val = (ray_object*)new_number_object($1);
    $$ = f;
}
| IDENTIFIER {
    factor_node *f = MAKE_AST_NODE(factor_node);
    f->type = FACTOR_TYPE_IDENTIFIER;
    f->val = (ray_object*)new_string_object($1);
    $$ = f;
}
| OP exp CP   {
    factor_node* e = MAKE_AST_NODE(factor_node);
    e->type = FACTOR_TYPE_EXP;
    e->exp = $2;
    $$ = e;
}

;

assign: IDENTIFIER ASSIGN exp {
    assign_node* e = MAKE_AST_NODE(assign_node);
    e->lval = $1;
    e->rval = $3;
    $$ = e;
}
;
%%
extern int yychar;
extern FILE* yyin;
void interactive_mode() {
    compiler *c = new_compiler();
    int is_last_eol = 1;
    while(1) {
        printf(">>>");
        int status;
        yypstate *ps = yypstate_new ();
        yychar = INTERP_START;
        status = yypush_parse(ps);
        do {
            yychar = yylex ();
            if(yychar == 0) {
                exit(0);
            }
            if(yychar == EOL) {
                if(is_last_eol == 1) {
                    printf(">>>");
                    continue;
                } else {
                    printf("...");
                    is_last_eol = 1;
                }
            } else {
                is_last_eol = 0;
            }
            status = yypush_parse (ps);
        } while (status == YYPUSH_MORE && interactive_root == NULL);
        yypstate_delete (ps);
        if(interactive_root && interactive_root->next) {
            code_gen(c, interactive_root);
            printf("******op code generated!******\n");
            eval(c);
        }
        status = 0;
        interactive_root = NULL;
    }
}
void normal_mode() {
    yyparse();
    compiler *c = new_compiler();
    continue_compiler(c);
    visit_statement_list(c, root);
#ifdef PARSE_DEBUG
    R_DEBUG("******op code generated!******\n");
#endif
    eval(c);
}
int main(int argc, char **argv) {
    if(argc == 2) {
        yyin = fopen(argv[1], "r");
    } else {
        is_interactive = 1;
    }
    if(is_interactive) {
        interactive_mode();
    } else {
        normal_mode();
    }
    return 0;
}
int yyerror(char *s)
{
    static int i;
    if(++i==3) exit(-1);
    return fprintf(stderr, "error: %s\n", s);
}
