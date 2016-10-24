#ifndef VISITOR_H
#define VISITOR_H
#include"opcode.h"
void visit(code_block *, syn_node *);
void visit_exp(code_block *, exp_node*);
void visit_term(code_block *, term_node*);
void visit_factor(code_block *, factor_node*);
#endif
