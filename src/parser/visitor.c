#include <stdio.h>
#include <stdlib.h>
#include <statement.h>
#include <opcode.h>
#include <vm.h>
#include <visitor.h>
#include <rayscript.tab.h>

void visit_exp(compiler *c, exp_node *node) {
    switch(node->type) {
    case TYPE_TERM:
        visit_term(c, node->op2);
        break;
    case ADD:
        visit_exp(c, node->op1);
        visit_term(c, node->op2);
        add_op(c, D_ADD);
        printf("ADD\n");
        break;
    case SUB:
        visit_exp(c, node->op1);
        visit_term(c, node->op2);
        add_op(c, D_SUB);
        printf("SUB\n");
        break;
    default:
        fprintf(stderr, "exp_node type %d error!\n", node->type);
        exit(-1);
    }
}
void visit_term(compiler *c, term_node* node) {
    switch(node->type) {
    case TYPE_FACTOR:
        visit_factor(c, node->op2);
        break;
    case MUL:
        visit_term(c, node->op1);
        visit_factor(c, node->op2);
        add_op(c, D_MUL);
        printf("MUL\n");
        break;
    case DIV:
        visit_term(c, node->op1);
        visit_factor(c, node->op2);
        add_op(c, D_DIV);
        printf("DIV\n");
        break;
    default:
        fprintf(stderr, "term_node type error!");
        exit(-1);
    }
}
void visit_rname(compiler *c, ray_object *name) {
    if(STRING_EXACT(name)) {
        printf("LOAD_NAME %s\n", STRING_OBJ_AS_STRING(name));
    }
    add_op_arg(c, LOAD_NAME, name);

}
void visit_factor(compiler *c, factor_node * node) {
    switch(node->type) {
    case NUM:
        if(NUMBER_EXACT(node->val)) {
            printf("PUSH %lf\n", NUMBER_OBJ_AS_NUMBER(node->val));
        }
        add_op_arg(c, PUSH, node->val);
        break;
    case EXP:
        visit_exp(c, node->exp);
        break;
    case ID:
        if(!STRING_OBJ_AS_STRING(node->val)) {
            printf("ID error!\n");
            exit(-1);
        }
        visit_rname(c, node->val);
        break;
    default:
        fprintf(stderr, "factor_node type %d error!\n", node->type);
        exit(-1);
    }
}

void visit_lname(compiler *c, identifier_node * node) {
    string_object *str_obj = new_string_object(node->val);
    printf("PUSH %s\n", STRING_OBJ_AS_STRING(str_obj));
    add_op_arg(c, PUSH, (ray_object *)str_obj);
}

void visit_assign(compiler *c, assign_node * node) {
    visit_exp(c, node->rval);
    visit_lname(c, node->lval);
    printf("STORE_NAME\n");
    add_op(c, STORE_NAME);
}
void visit_if(compiler *c, if_statement_node *node) {
    code_block * next;
    code_block * end = new_code_block();
    if(node->els == NULL) {
        next = end;
    } else {
        next = new_code_block();
    }
    visit_exp(c, node->test_exp);
    printf("JUMP_FALSE\n");
    add_op_jmp(c, JUMP_FALSE, next);
    visit_compound_statement(c, node->then);
    printf("JUMP\n");
    add_op_jmp(c, JUMP, end);
    if(node->els != NULL) {
        use_code_block_next(c, next);
        visit_compound_statement(c, node->els);
    }
    use_code_block_next(c, end);
}

void visit_print(compiler *c, print_statement_node *node) {
    visit_exp(c, node->exp);
    printf("PRINT\n");
    add_op(c, PRINT);    
}
void visit_compound_statement(compiler *c, compound_statement_node *node) {
    statement_list_node* list_node = node->list;
    if(list_node) {
        visit_statement_list(c, list_node);
    }
}

void visit_statement(compiler *c, statement_node * node) {
    ast_node* val = node->val;
    switch(val->ast_type) {
    case EXP:
        visit_exp(c, (exp_node *)val);
        break;
    case TERM:
        visit_term(c, (term_node *)val);
        break;
    case FACTOR:
        visit_factor(c, (factor_node *)val);
        break;
    case ASSIGNMENT:
        visit_assign(c, (assign_node *)val);
        break;
    case TYPE_PRINT:
        visit_print(c, (print_statement_node *)val);
        break;
    case TYPE_IF:
        visit_if(c, (if_statement_node *)val);
        break;
    case STATEMENT:
        visit_statement(c, (statement_node *)val);
        break;
    case STATEMENT_LIST:
        visit_statement_list(c, (statement_list_node *)val);
        break;
    case COMPOUND_STATEMENT:
        visit_compound_statement(c, (compound_statement_node *)val);
        break;
    default:
        fprintf(stderr, "ast_node type %d error!", node->ast_type);
        exit(-1);
    }
}
void visit_statement_list(compiler *c, statement_list_node * node) {
    statement_node * n = node->next;
    while(n) {
        visit_statement(c, n);
        n = n->next;
    }
}

