#ifndef VISITOR_H
#define VISITOR_H
#include <opcode.h>
#include <vm.h>
void visit_compound_statement(compiler *, compound_statement_node *);
void visit_statement_list(compiler *, statement_list_node *);
void visit_statement(compiler *, statement_node *);
void visit_exp(compiler *, exp_node*);
void visit_term(compiler *, term_node*);
void visit_factor(compiler *, factor_node*);
#endif
