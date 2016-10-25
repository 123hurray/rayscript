#ifndef STATEMENT_H
#define STATEMENT_H
typedef struct _ast_node ast_node;
#define YYSTYPE ast_node* 
#include "rayscript.tab.h"
#include "opcode.h"
#define AST_HEADER \
    ast_type_enum ast_type;\
    ast_node* next
typedef enum ast_type_enum {EXP, TERM, FACTOR, NUM, ID, ASSIGNMENT} ast_type_enum;
typedef struct _term_node term_node;
typedef struct _factor_node factor_node;
typedef struct _exp_node exp_node;
struct _exp_node {
    AST_HEADER;
    enum yytokentype type;
    struct _exp_node* op1;
    term_node* op2;
};
struct _term_node {
    AST_HEADER;
    enum yytokentype type;
    term_node *op1;
    factor_node *op2;
};
struct _factor_node {
    AST_HEADER;
    ast_type_enum type;
    union {
        ray_object* val;
        exp_node* exp;
    };
};
typedef struct {
    AST_HEADER;
    char * val;
} identifier_node;
typedef struct {
    AST_HEADER;
    identifier_node * lval;
    exp_node * rval;
} assign_node;
struct _ast_node {
    AST_HEADER;
};
#endif
