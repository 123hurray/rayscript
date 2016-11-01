%{
//#define RDEBUG
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
#include <object.h>
int is_interactive = 0;
static statement_list_node* root;
static statement_list_node* interactive_root;
int yylex();
int yyerror();
#define YYMALLOC _R_MALLOC
#define YYFREE _R_FREE
%}
%define api.push-pull both
 /* declare tokens */
%token INTERP_START
%token INT_TOKEN FLOAT_TOKEN BOOL_TOKEN NIL_TOKEN
%token OPERATOR OP CP ASSIGN_TOKEN IDENTIFIER 
%token EOL 
%token COMMA ARROW_TOKEN
%token IF_TOKEN ELSE_TOKEN LB RB SC 
%token PRINT_TOKEN
%token TYPE_TERM TYPE_FACTOR
%token AND_TOKEN OR_TOKEN EQUALITY_TOKEN RELATIONAL_TOKEN
%token FOR_TOKEN FROM_TOKEN TO_TOKEN STEP_TOKEN
%token LOOP_TOKEN
%right ASSIGN_TOKEN
%left EQUALITY_TOKEN
%left RELATIONAL_TOKEN
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
    for_from_to_statement_node* ffts;
    arg_node* arn;
    call_arg_node* can;
    call_function_node* cfn;
    function_node* fcn;
    bool_object* b;
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
%type <arn> arg_list;
%type <fcn> function_def;
%type <cfn> call_function;
%type <can> call_arg_list;
%type <an> assign;
%type <sn> statement;
%type <en> exp;
%type <fn> factor;
%type <ffts> for_from_to_statement;
%type <fnum> FLOAT_TOKEN;
%type <inum> INT_TOKEN;
%type <b> BOOL_TOKEN;
%type <str> IDENTIFIER;
%type <op> EQUALITY_TOKEN;
%type <op> RELATIONAL_TOKEN;
%type <op> OPERATOR_1;
%type <op> OPERATOR_2;
%nonassoc EOL
%start program
%%
program: statement_list {
    root = $1;
}
| statement_list eols{
    root = $1;
}

| INTERP_START statement EOL{
    statement_list_node* e = MAKE_AST_NODE(statement_list_node);
    e->next = $2;
    interactive_root = e;
}
statement_list: statement { 
    statement_list_node* e = MAKE_AST_NODE(statement_list_node);
    e->next = e->tail = $1;
    $$ = e;
}
| statement_list eols statement {
    statement_list_node* s = $1;
    s->tail->next = $3;
    s->tail = $3;
    $$ = $1;
}
;
eols: EOL
| eols EOL
;
compound_statement: LB eols RB  {
    compound_statement_node* e = MAKE_AST_NODE(compound_statement_node);
    e->list = NULL;
    $$ = e;
}
| LB eols statement_list eols RB {
    compound_statement_node* e = MAKE_AST_NODE(compound_statement_node);
    e->list = $3;
    $$ = e;
}
;
statement: compound_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->stype = STATEMENT_TYPE_COMPOUND;
    e->next = NULL;
    e->csn = $1;
    $$ = e; 
}
| exp {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->stype = STATEMENT_TYPE_EXP;
    e->next = NULL;
    e->en = $1;
    $$ = e; 
} 
| assign {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->stype = STATEMENT_TYPE_ASSIGN;
    e->next = NULL;
    e->an = $1;
    $$ = e; 
} 
| if_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->stype = STATEMENT_TYPE_IF;
    e->next = NULL;
    e->isn = $1;
    $$ = e; 
}
| print_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->stype = STATEMENT_TYPE_PRINT;
    e->next = NULL;
    e->psn = $1;
    $$ = e; 
}
| for_from_to_statement {
    statement_node* e = MAKE_AST_NODE(statement_node);
    e->stype = STATEMENT_TYPE_FOR_FROM_TO;
    e->next = NULL;
    e->fftn = $1;
    $$ = e; 
}
;
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

call_arg_list: {
    call_arg_node* e = MAKE_AST_NODE(call_arg_node);
    e->size = 0;
    e->arg_list = new_list_object(1);
    $$ = e;
}
| exp {
    call_arg_node* e = MAKE_AST_NODE(call_arg_node);
    e->size = 1;
    e->arg_list = new_list_object(1);
    list_append(AS_OBJ(e->arg_list), AS_OBJ($1));
    $$ = e;
}
| call_arg_list COMMA exp {
    call_arg_node* e = $1; 
    ++(e->size);
    list_append(AS_OBJ(e->arg_list), AS_OBJ($3));
    $$ = e;
}
;

call_function: IDENTIFIER OP call_arg_list CP {
    call_function_node* e = MAKE_AST_NODE(call_function_node);
    e->name = new_string_object($1);
    R_FREE($1);
    e->args = $3;
    $$ = e;
}
;



arg_list: OP CP {
    arg_node* e = MAKE_AST_NODE(arg_node);
    e->arg_list = new_list_object(1);
    $$ = e;
}
| IDENTIFIER {
    arg_node* e = MAKE_AST_NODE(arg_node);
    e->arg_list = new_list_object(1);
    list_append(AS_OBJ(e->arg_list), AS_OBJ(new_string_object($1)));
    R_FREE($1);
    $$ = e;
}
| arg_list IDENTIFIER {
    arg_node* e = $1;
    list_append(AS_OBJ(e->arg_list), AS_OBJ(new_string_object($2)));
    R_FREE($2);
    $$ = $1;
};

function_def: arg_list ARROW_TOKEN compound_statement {
    function_node* e = MAKE_AST_NODE(function_node);
    e->args = $1;
    e->body = $3;
    $$ = e;
}

exp: factor { 
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->etype = EXP_TYPE_FACTOR;
    e->factor = $1;
    $$ = e;
} 

| exp EQUALITY_TOKEN exp {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->etype = EXP_TYPE_OP;
    calc_node *cn = MAKE_AST_NODE(calc_node);
    cn->l = $1;
    cn->r = $3;
    cn->o = $2;
    e->op = cn;
    $$ = e; 
}
| exp RELATIONAL_TOKEN exp {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->etype = EXP_TYPE_OP;
    calc_node *cn = MAKE_AST_NODE(calc_node);
    cn->l = $1;
    cn->r = $3;
    cn->o = $2;
    e->op = cn;
    $$ = e; 
}
 | exp OPERATOR_1 exp {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->etype = EXP_TYPE_OP;
    calc_node *cn = MAKE_AST_NODE(calc_node);
    cn->l = $1;
    cn->r = $3;
    cn->o = $2;
    e->op = cn;
    $$ = e; 
} 
 | exp OPERATOR_2 exp {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->etype = EXP_TYPE_OP;
    calc_node *cn = MAKE_AST_NODE(calc_node);
    cn->l = $1;
    cn->r = $3;
    cn->o = $2;
    e->op = cn;
    $$ = e; 
} 
| function_def {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->etype = EXP_TYPE_FUNCTION_DEF;
    e->function = $1;
    $$ = e; 
}
|call_function {
    exp_node *e = MAKE_AST_NODE(exp_node);
    e->etype = EXP_TYPE_CALL_FUNCTION;
    e->call = $1;
    $$ = e; 
};
factor : INT_TOKEN {
    factor_node *f = MAKE_AST_NODE(factor_node);
    f->ftype = FACTOR_TYPE_INT;
    f->val = (ray_object*)new_number_object_from_long($1);
    $$ = f;
}
| FLOAT_TOKEN {
    factor_node *f = MAKE_AST_NODE(factor_node);
    f->ftype = FACTOR_TYPE_FLOAT;
    f->val = (ray_object*)new_number_object_from_double($1);
    $$ = f;
}
| IDENTIFIER {
    factor_node *f = MAKE_AST_NODE(factor_node);
    f->ftype = FACTOR_TYPE_IDENTIFIER;
    f->val = (ray_object*)new_string_object($1);
    R_FREE($1);
    $$ = f;
}
| OP exp CP   {
    factor_node* e = MAKE_AST_NODE(factor_node);
    e->ftype = FACTOR_TYPE_EXP;
    e->exp = $2;
    $$ = e;
} 
| BOOL_TOKEN {
    factor_node* e = MAKE_AST_NODE(factor_node);
    e->ftype = FACTOR_TYPE_BOOL;
    e->val = (ray_object*)$1;
    $$ = e;
}
| NIL_TOKEN {
    factor_node* e = MAKE_AST_NODE(factor_node);
    e->ftype = FACTOR_TYPE_NIL;
    e->val = (ray_object*)nil;
    $$ = e;
}
;



for_from_to_statement: FOR_TOKEN IDENTIFIER FROM_TOKEN statement TO_TOKEN statement STEP_TOKEN statement compound_statement {
    for_from_to_statement_node *e = MAKE_AST_NODE(for_from_to_statement_node);
    e->name = new_string_object($2);
    R_FREE($2);
    e->from = $4;
    e->to = $6;
    e->step = $8;
    e->body = $9;
    $$ = e;
};

assign: IDENTIFIER ASSIGN_TOKEN statement {
    assign_node* e = MAKE_AST_NODE(assign_node);
    e->lval = new_string_object($1);
    R_FREE($1);
    e->rval = $3;
    $$ = e;
}
;
%%
#if defined(__clang__) && defined(__clang_major__) && defined(__clang_minor__)
#define COMPILER "[clang %d.%d.%d]\n", __clang_major__, __clang_minor__, __clang_patchlevel__
#elif defined(__GNUC__)
#define COMPILER "[GCC " __VERSION__ "]\n"
#else
#define COMPILER "[Unknown compiler]"
#endif

#ifdef HAVE_EDITLINE
int line_status;
#endif
extern int yychar;
extern FILE* yyin;
extern void yyrestart(FILE*);
void interactive_mode() {
    printf("%s.\nBuild on %s with ", PACKAGE_STRING, __DATE__ " " __TIME__);
    printf(COMPILER);
    compiler *c = new_compiler();
    int is_last_eol = 1;
    while(1) {
        int status;
        yypstate *ps = yypstate_new ();
        yychar = INTERP_START;
        status = yypush_parse(ps);
#ifdef HAVE_EDITLINE
        line_status = 0;
#else 
        printf(">>>");
#endif
        do {
            yychar = yylex ();
            if(yychar == 0) {
                exit(0);
            }
            if(yychar == EOL) {
                if(is_last_eol == 1) {
#ifdef HAVE_EDITLINE
                    line_status = 0;
#else
                    printf(">>>");
#endif
                    continue;
                } else {
#ifdef HAVE_EDITLINE
                    line_status = 1;
#else
                    printf("...");
#endif
                    is_last_eol = 1;
                }
            } else {
                is_last_eol = 0;
            }
            status = yypush_parse (ps);
            if(status == 1) {
                // flush input buffer
                yyrestart(yyin);
                interactive_root = NULL;
                break;
            }
        } while (status == YYPUSH_MORE && interactive_root == NULL);
        yypstate_delete (ps);
        printf("\b\b\b");
        if(status != 1 && interactive_root && interactive_root->next) {
            code_gen(c, interactive_root);
#ifdef PARSE_DEBUG
            printf("******op code generated!******\n");
#endif
            eval(c);
        }
        status = 0;
        interactive_root = NULL;
    }
    destruct_compiler(c);
}
void normal_mode() {
    yyparse();
    compiler *c = new_compiler();
    continue_compiler(c);
    if(root) {
        visit_statement_list(c, root);
        root = NULL;
#ifdef PARSE_DEBUG
        R_DEBUG("******op code generated!******\n");
#endif
        eval(c);
    }
    destruct_compiler(c);
}
int main(int argc, char **argv) {
    if(argc == 2) {
        yyin = fopen(argv[1], "r");
    } else {
        is_interactive = 1;
    }
    init_objects();
    if(is_interactive) {
        interactive_mode();
    } else {
        normal_mode();
    }
    return 0;
}
int yyerror(char *s)
{
    printf("\b\b\b");
    R_ERROR("error: %s\n", s);
    return 0;

}
