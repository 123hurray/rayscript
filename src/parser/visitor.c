#include<stdio.h>
#include<stdlib.h>
#include"statement.h"
#include"opcode.h"
#include"visitor.h"
#include"rayscript.tab.h"

void visit_exp(code_block * b, exp_node *node) {
    switch(node->type) {
    case 0:
        visit_term(b, node->op2);
        break;
    case ADD:
        visit_exp(b, node->op1);
        visit_term(b, node->op2);
        add_op(b, D_ADD);
        printf("ADD\n");
        break;
    case SUB:
        visit_exp(b, node->op1);
        visit_term(b, node->op2);
        add_op(b, D_SUB);
        printf("SUB\n");
        break;
    default:
        fprintf(stderr, "exp_node type %d error!\n", node->type);
        exit(-1);
    }
}
void visit_term(code_block * b, term_node* node) {
    switch(node->type) {
    case 0:
        visit_factor(b, node->op2);
        break;
    case MUL:
        visit_term(b, node->op1);
        visit_factor(b, node->op2);
        add_op(b, D_MUL);
        printf("MUL\n");
        break;
    case DIV:
        visit_term(b, node->op1);
        visit_factor(b, node->op2);
        add_op(b, D_DIV);
        printf("DIV\n");
        break;
    default:
        fprintf(stderr, "Syn_node type error!");
        exit(-1);
    }
}
void visit_factor(code_block *b, factor_node * node) {
    switch(node->type) {
    case NUM:
        printf("PUSH %lf\n", *node->val);
        add_op_arg(b, PUSH, node->val);
        break;
    case EXP:
        visit_exp(b, node->exp);
        break;
    default:
        fprintf(stderr, "factor_node type error!");
        exit(-1);
    }
}
void visit(code_block * b, syn_node * node) {
    switch(node->type) {
    case EXP:
        visit_exp(b, node->exp);
        break;
    case TERM:
        visit_term(b, node->term);
        break;
    case FACTOR:
        visit_factor(b, node->factor);
        break;
    default:
        fprintf(stderr, "Syn_node type error!");
        exit(-1);
    }
    add_op(b, PRINT);
}

