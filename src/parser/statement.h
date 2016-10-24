#ifndef STATEMENT_H
#define STATEMENT_H
typedef struct _syn_node syn_node;
typedef double VAL_TYPE;
#define YYSTYPE syn_node* 
#include "rayscript.tab.h"
#include "opcode.h"
typedef enum syntax_type {EXP, TERM, FACTOR, NUM} syntax_type;
typedef struct _term_node term_node;
typedef struct _factor_node factor_node;
typedef struct _exp_node {
    enum yytokentype type;
    struct _exp_node* op1;
    term_node* op2;
} exp_node;
typedef struct _term_node {
    enum yytokentype type;
    term_node *op1;
    factor_node *op2;
} term_node;
typedef struct _factor_node {
    enum syntax_type type;
    union {
        ray_object* val;
        exp_node* exp;
    };
} factor_node;
typedef struct _syn_node {
    syntax_type type;
    union {
        exp_node* exp;
        term_node* term;
        factor_node* factor;
    };
} syn_node;
#endif
