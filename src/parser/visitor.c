#include <stdio.h>
#include <stdlib.h>
#include <statement.h>
#include <opcode.h>
#include <visitor.h>
#include <rayscript.tab.h>

void visit_exp(code_block * b, exp_node *node) {
    switch(node->type) {
    case TYPE_TERM:
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
    case TYPE_FACTOR:
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
        fprintf(stderr, "term_node type error!");
        exit(-1);
    }
}
void visit_rname(code_block *b, ray_object *name) {
    if(STRING_EXACT(name)) {
        printf("LOAD_NAME %s\n", STRING_OBJ_AS_STRING(name));
    }
    add_op_arg(b, LOAD_NAME, name);

}
void visit_factor(code_block *b, factor_node * node) {
    switch(node->type) {
    case NUM:
        if(NUMBER_EXACT(node->val)) {
            printf("PUSH %lf\n", NUMBER_OBJ_AS_NUMBER(node->val));
        }
        add_op_arg(b, PUSH, node->val);
        break;
    case EXP:
        visit_exp(b, node->exp);
        break;
    case ID:
        if(!STRING_OBJ_AS_STRING(node->val)) {
            printf("ID error!\n");
            exit(-1);
        }
        visit_rname(b, node->val);
        break;
    default:
        fprintf(stderr, "factor_node type %d error!\n", node->type);
        exit(-1);
    }
}

void visit_lname(code_block * b, identifier_node * node) {
    string_object *str_obj = new_string_object(node->val);
    printf("PUSH %s\n", STRING_OBJ_AS_STRING(str_obj));
    add_op_arg(b, PUSH, (ray_object *)str_obj);
}

void visit_assign(code_block * b, assign_node * node) {
    visit_exp(b, node->rval);
    visit_lname(b, node->lval);
    printf("STORE_NAME\n");
    add_op(b, STORE_NAME);
}
void visit(code_block * b, ast_node * node) {
    while(node) {
        switch(node->ast_type) {
        case EXP:
            visit_exp(b, (exp_node *)node);
            break;
        case TERM:
            visit_term(b, (term_node *)node);
            break;
        case FACTOR:
            visit_factor(b, (factor_node *)node);
            break;
        case ASSIGNMENT:
            visit_assign(b, (assign_node *)node);
            break;
        default:
            fprintf(stderr, "ast_node type %d error!", node->ast_type);
            exit(-1);
        }
        printf("PRINT\n");
        add_op(b, PRINT);
        node = node->next;
    }
}

