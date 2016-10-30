#include <stdio.h>
#include <stdlib.h>
#include <statement.h>
#include <opcode.h>
#include <vm.h>
#include <visitor.h>
#include <rayscript.tab.h>
#define HANDLE_BIN_OP(op) \
    case OP_TYPE_##op:\
        visit_exp(c, op1);\
        visit_exp(c, op2);\
        ADD_OP(c, OP_##op);\
        break
void visit_exp(compiler *c, exp_node *node) {
    switch(node->type) {
    case EXP_TYPE_OP: {
        exp_node *op1 = node->op1;
        exp_node *op2 = node->op2;
        switch(node->op3) {
        HANDLE_BIN_OP(ADD);
        HANDLE_BIN_OP(SUB);
        HANDLE_BIN_OP(MUL);
        HANDLE_BIN_OP(DIV);
        HANDLE_BIN_OP(LT);
        HANDLE_BIN_OP(GT);
        HANDLE_BIN_OP(LE);
        HANDLE_BIN_OP(GE);
        HANDLE_BIN_OP(NE);
        HANDLE_BIN_OP(EQ);
        default:
            R_FATAL("Unknown operator type: %d", node->op3);
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
int store_obj(compiler *c, ray_object* obj) {
    long i = list_find((ray_object*)c->lb->consts, obj);
    if(i < 0) {
        i = list_append((ray_object *)c->lb->consts, obj);
    }
    return (int)i;
}
void visit_rname(compiler *c, ray_object *name) {
    int i = store_obj(c, name);
    ADD_OP_ARG(c, LOAD_NAME, i);
}
void visit_factor(compiler *c, factor_node * node) {
    switch(node->type) {
    case FACTOR_TYPE_INT:
    case FACTOR_TYPE_FLOAT:
    case FACTOR_TYPE_BOOL:
    case FACTOR_TYPE_NIL: {
            int i = store_obj(c, node->val);
            ADD_OP_ARG(c, PUSH, i);
        }
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

int visit_lname(compiler *c, char* node) {
    string_object *str_obj = new_string_object(node);
    long i = list_find((ray_object*)c->lb->consts, (ray_object*)str_obj);
    if(i < 0) {
        i = list_append((ray_object *)c->lb->consts, (ray_object *)str_obj);
    }
    return (int)i;
}

void visit_assign(compiler *c, assign_node * node) {
    visit_statement(c, node->rval);
    int index = visit_lname(c, node->lval);
    ADD_OP_ARG(c, STORE_NAME, index);
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
        int i = (int)list_append((ray_object*)c->lb->consts, AS_OBJ(nil));
        ADD_OP_ARG(c, PUSH, i);
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
void visit_for_from_to (compiler* c, for_from_to_statement_node *node) {

    // from, init i 
    visit_statement(c, node->from);
    ray_object* name = (ray_object*)new_string_object(node->i);
    long i = list_find((ray_object*)c->lb->consts, name);
    if(i < 0) {
        i = list_append((ray_object *)c->lb->consts, name);
    }
    ADD_OP_ARG(c, STORE_NAME, (int)i);
    ADD_OP(c, POP);

    // to, test i
    code_block* to_block = new_code_block();
    use_code_block_next(c, to_block);
    ADD_OP_ARG(c, LOAD_NAME, (int)i);
    // dup name so value of for will be set
    ADD_OP(c, DUP);
    visit_statement(c, node->to);
    ADD_OP(c, OP_EQ);
    code_block* end_block = new_code_block();
    ADD_OP_JMP(c, JUMP_TRUE, end_block);

    // body

    // pop i that dup before
    ADD_OP(c, POP);
    visit_compound_statement(c, node->body);
    ADD_OP(c, POP);
    visit_statement(c, node->step);
    ADD_OP(c, POP);
    ADD_OP_JMP(c, JUMP, to_block);

    // end
    use_code_block_next(c, end_block);

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
    case STATEMENT_TYPE_FOR_FROM_TO:
        visit_for_from_to(c, node->fftn);
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
        visit_statement(c, n);
        if(n->next) {
            ADD_OP(c, POP);
        }
        n = n->next;
    }
}

