#ifndef STATEMENT_H
#define STATEMENT_H
#include <opcode.h>
#include <object.h>
#define AST_HEADER OBJ_HEADER


typedef enum ast_type_enum {EXP, TERM, FACTOR, NUM, ID, ASSIGNMENT, TYPE_PRINT, TYPE_IF, STATEMENT, STATEMENT_LIST, COMPOUND_STATEMENT} ast_type_enum;
typedef struct {
    AST_HEADER;
} ast_node;

typedef enum {
    STATEMENT_TYPE_COMPOUND,
    STATEMENT_TYPE_EXP,
    STATEMENT_TYPE_ASSIGN,
    STATEMENT_TYPE_IF,
    STATEMENT_TYPE_PRINT,
    STATEMENT_TYPE_FOR_FROM_TO
} statement_type;
typedef enum {
    FACTOR_TYPE_INT,
    FACTOR_TYPE_FLOAT,
    FACTOR_TYPE_IDENTIFIER,
    FACTOR_TYPE_EXP,
    FACTOR_TYPE_BOOL,
    FACTOR_TYPE_NIL
} factor_type;

typedef enum {
    CALL_FUNCTION_TYPE_BY_NAME,
    CALL_FUNCTION_TYPE_BY_EXP,
} call_function_type;
typedef enum {
    OP_TYPE_EQ,
    OP_TYPE_NE,
    OP_TYPE_LT,
    OP_TYPE_GT,
    OP_TYPE_LE,
    OP_TYPE_GE,
    OP_TYPE_ADD,
    OP_TYPE_SUB,
    OP_TYPE_MUL,
    OP_TYPE_DIV,
} operator_type;

typedef enum {
    ASSIGN_TYPE_STATEMENT,
    ASSIGN_TYPE_INC,
    ASSIGN_TYPE_DEC,
} assign_type;

typedef enum {
    EXP_TYPE_FACTOR,
    EXP_TYPE_OP,
    EXP_TYPE_FUNCTION_DEF,
    EXP_TYPE_CALL_FUNCTION
} exp_type;

typedef struct _term_node term_node;
typedef struct _factor_node factor_node;
typedef struct _exp_node exp_node;
typedef struct _print_statement_node print_statement_node;
typedef struct _if_statement_node if_statement_node;
typedef struct _statement_node statement_node;
typedef struct _statement_list_node statement_list_node;
typedef struct _compound_statement_node compound_statement_node;
typedef struct _for_from_to_statement_node for_from_to_statement_node;
typedef struct _assign_node assign_node;
typedef struct _calc_node calc_node;
typedef struct _arg_node arg_node;
typedef struct _function_node function_node;
typedef struct _call_function_node call_function_node;
typedef struct _arg_node call_arg_node;
#include <rayscript.tab.h>

struct _compound_statement_node {
    AST_HEADER;
    statement_list_node *list;
};
struct _statement_node {
    AST_HEADER;
    statement_node *next;
    statement_type stype;
    union {
        compound_statement_node * csn;
        exp_node* en;
        assign_node* an;
        print_statement_node* psn;
        if_statement_node* isn;
        for_from_to_statement_node* fftn;
    };
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



struct _arg_node {
    AST_HEADER;
    int size;
    list_object* arg_list;
};

struct _function_node {
    AST_HEADER;
    arg_node* args;
    compound_statement_node* body; 
};

struct _call_function_node {
    AST_HEADER;
    call_function_type ftype;
    union {
        string_object* name;
        exp_node* exp;
    };
    call_arg_node* args; 
};

struct _calc_node {
    AST_HEADER;
    exp_node* l;
    exp_node* r;
    operator_type o;
};
struct _exp_node {
    AST_HEADER;
    exp_type etype;
    union {
        calc_node *op;
        factor_node* factor;
        function_node* function;
        call_function_node* call;
    };
};
struct _factor_node {
    AST_HEADER;
    factor_type ftype;
    union {
        ray_object* val;
        exp_node* exp;
    };
};


struct _assign_node {
    AST_HEADER;
    string_object * lval;
    assign_type atype;
    statement_node * rval;
};

struct _for_from_to_statement_node {
    AST_HEADER;
    string_object* name;
    statement_node* from;
    statement_node* to;
    statement_node* step;
    compound_statement_node* body; 
};



#define MAKE_AST_NODE(type) ((type*)init_ast_node((void *)R_MALLOC(type)));

static inline void * init_ast_node(void *o) {
    INIT_OBJ_HEADER(AS_OBJ(o), ast_type_object);
    return o;
}



#endif
