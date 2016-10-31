#ifndef VISITOR_H
#define VISITOR_H
#include <globals.h>
#include <config.h>
#include <opcode.h>
#include <vm.h>


// Defines

#ifdef PARSE_DEBUG
#define ADD_OP(c, o) do{\
    R_DEBUG("%p:%-3d   "#o"\n", (c)->lb->cb, (c)->lb->cb->code_len);\
    add_op((c), (o));\
}while(0)



#define ADD_OP_ARG(c, o, a) do{\
    R_DEBUG("%p:%-3d   %-16s%-3d (%s)\n", (c)->lb->cb, (c)->lb->cb->code_len, #o, (a), STRING_OBJ_AS_STRING(OBJ_STR(list_get(AS_OBJ((c)->lb->consts), (a)))));\
    add_op_arg((c), (o), (a));\
}while(0)


#define ADD_OP_JMP(c, o, t) do{\
    R_DEBUG("%p:%-3d   %-16s%p\n", (c)->lb->cb, (c)->lb->cb->code_len, #o, (t));\
    add_op_jmp((c), (o), (t));\
}while(0)


#define ADD_OP_INVOKE(c, o, a) do{\
    R_DEBUG("%p:%-3d   %-16s%-3d\n", (c)->lb->cb, (c)->lb->cb->code_len, #o, (a));\
    add_op_arg((c), (o), (a));\
}while(0)


#else
#define ADD_OP(c, o) do{add_op((c), (o));}while(0)
#define ADD_OP_ARG(c, o, a) do{add_op_arg((c), (o), (a));}while(0)
#define ADD_OP_JMP(c, o, t) do{add_op_jmp((c), (o), (t));}while(0)
#define ADD_OP_INVOKE(c, o, a) do{add_op_arg((c), (o), (a));}while(0)
#endif


void code_gen(compiler *, statement_list_node *);
void visit_compound_statement(compiler *, compound_statement_node *);
void visit_statement_list(compiler *, statement_list_node *);
void visit_statement(compiler *, statement_node *);
void visit_exp(compiler *, exp_node*);
void visit_term(compiler *, term_node*);
void visit_factor(compiler *, factor_node*);
#endif
