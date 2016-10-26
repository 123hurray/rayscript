#ifndef STATEMENT_H
#define STATEMENT_H
typedef struct _ast_node ast_node;
#define YYSTYPE ast_node* 
#include "rayscript.tab.h"
#include "opcode.h"
#define AST_HEADER \
    ast_type_enum ast_type;
typedef enum ast_type_enum {EXP, TERM, FACTOR, NUM, ID, ASSIGNMENT, TYPE_PRINT, TYPE_IF, STATEMENT, STATEMENT_LIST, COMPOUND_STATEMENT} ast_type_enum;
typedef struct _term_node term_node;
typedef struct _factor_node factor_node;
typedef struct _exp_node exp_node;
typedef struct _print_statement_node print_statement_node;
typedef struct _if_statement_node if_statement_node;
typedef struct _statement_node statement_node;
typedef struct _statement_list_node statement_list_node;
typedef struct _compound_statement_node compound_statement_node;

struct _compound_statement_node {
    AST_HEADER;
    statement_list_node *list;
};
struct _statement_node {
    AST_HEADER;
    statement_node *next;
    ast_node *val;
    ast_type_enum type;
};

struct _statement_list_node {
    AST_HEADER;
    statement_node *next;
    statement_node *tail;
};

struct _print_statement_node {
    AST_HEADER;
    exp_node* exp;
};
struct _if_statement_node {
    AST_HEADER;
    exp_node* test_exp;
    compound_statement_node *then;
    compound_statement_node *els;
};


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





#define MAKE_AST_NODE(type) ((type *)malloc(sizeof(type)))



#endif
