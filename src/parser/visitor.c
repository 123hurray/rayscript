#include <stdio.h>
#include <stdlib.h>
#include <statement.h>
#include <opcode.h>
#include <vm.h>
#include <visitor.h>
#include <rayscript.tab.h>

void visit_exp(compiler *c, exp_node *node) {
    switch(node->type) {
    case EXP_TYPE_OP: {
        exp_node *op1 = node->op1;
        exp_node *op2 = node->op2;
        switch(node->op3) {
        case OP_ADD:
            visit_exp(c, op1);
            visit_exp(c, op2);
            ADD_OP(c, D_ADD);
            break;
        case OP_SUB:
            visit_exp(c, op1);
            visit_exp(c, op2);
            ADD_OP(c, D_SUB);
            break;
        case OP_MUL:
            visit_exp(c, op1);
            visit_exp(c, op2);
            ADD_OP(c, D_MUL);
            break;
        case OP_DIV:
            visit_exp(c, op1);
            visit_exp(c, op2);
            ADD_OP(c, D_DIV);
            break;
        }
    }
    break;
    case EXP_TYPE_FACTOR:
        visit_factor(c, node->op4);
        break;
    default:
        R_FATAL("exp_node type %d error!\n", node->type);
    }
}
void visit_rname(compiler *c, ray_object *name) {
    ADD_OP_ARG(c, LOAD_NAME, name);

}
void visit_factor(compiler *c, factor_node * node) {
    switch(node->type) {
    case FACTOR_TYPE_INT:
    case FACTOR_TYPE_FLOAT:
        ADD_OP_ARG(c, PUSH, node->val);
        break;
    case FACTOR_TYPE_EXP:
        visit_exp(c, node->exp);
        break;
    case FACTOR_TYPE_IDENTIFIER:
        if(!STRING_OBJ_AS_STRING(node->val)) {
            R_FATAL("ID error!\n");
        }
        visit_rname(c, node->val);
        break;
    default:
        R_FATAL("factor_node type %d error!\n", node->type);
    }
}

void visit_lname(compiler *c, char* node) {
    string_object *str_obj = new_string_object(node);
    ADD_OP_ARG(c, PUSH, (ray_object *)str_obj);
}

void visit_assign(compiler *c, assign_node * node) {
    visit_exp(c, node->rval);
    visit_lname(c, node->lval);
    ADD_OP(c, STORE_NAME);
}
void visit_if(compiler *c, if_statement_node *node) {
    code_block * next = new_code_block();

    code_block * end = new_code_block();
    visit_exp(c, node->test_exp);
    ADD_OP_JMP(c, JUMP_FALSE, next);
    visit_compound_statement(c, node->then);
    ADD_OP_JMP(c, JUMP, end);
    use_code_block_next(c, next);
    if(node->els != NULL) {
        visit_compound_statement(c, node->els);
    } else {
        // Make fake else return nil
        ADD_OP_ARG(c, PUSH, (ray_object *)nil);
    }
    
    use_code_block_next(c, end);
}

void visit_print(compiler *c, print_statement_node *node) {
    visit_exp(c, node->exp);
    ADD_OP(c, DUP);
    ADD_OP(c, PRINT);    
}
void visit_compound_statement(compiler *c, compound_statement_node *node) {
    statement_list_node* list_node = node->list;
    if(list_node) {
        visit_statement_list(c, list_node);
    }
}

void visit_statement(compiler *c, statement_node * node) {
    switch(node->type) {
    case STATEMENT_TYPE_EXP:
        visit_exp(c, node->en);
        break;
    case STATEMENT_TYPE_ASSIGN:
        visit_assign(c, node->an);
        break;
    case STATEMENT_TYPE_PRINT:
        visit_print(c, node->psn);
        break;
    case STATEMENT_TYPE_IF:
        visit_if(c, node->isn);
        break;
    case STATEMENT_TYPE_COMPOUND:
        visit_compound_statement(c, node->csn);
        break;
    default:
        R_FATAL("ast_node type %d error!", node->type);
    }
}

void code_gen(compiler *c, statement_list_node *node) {

    continue_compiler(c);
    visit_statement_list(c, node);

}
void visit_statement_list(compiler *c, statement_list_node * node) {
    statement_node * n = node->next;
    while(n) {
        if(n->type != STATEMENT_TYPE_EMPTY) {
            visit_statement(c, n);
            if(n->next) {
                ADD_OP(c, POP);
            }
        }
        n = n->next;
    }
}

