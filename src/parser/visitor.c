#include <stdio.h>
#include <stdlib.h>
#include <statement.h>
#include <opcode.h>
#include <vm.h>
#include <visitor.h>
#include <rayscript.tab.h>
#include <function_object.h>
#define HANDLE_BIN_OP(op) \
    case OP_TYPE_##op:\
        visit_exp(c, n->l);\
        visit_exp(c, n->r);\
        ADD_OP(c, OP_##op);\
        DEC_REF(n);\
        break

void destruct_ast_node(ray_object* o) {
    R_FREE(o);
}

int store_obj(compiler *c, ray_object* obj) {
    long i = list_find((ray_object*)c->lb->consts, obj);
    if(i < 0) {
        i = list_append((ray_object *)c->lb->consts, obj);
    }
    return (int)i;
}
void visit_function_def(compiler* c, function_node* node) {
#ifdef PARSE_DEBUG
    R_DEBUG("-------------FUNC START------------\n");
#endif
    map_object* locals = new_map_object(1);
    logic_block* lb = new_logic_block(c->lb->globals, locals);
    DEC_REF(locals);
    lb->head_block = lb->eval_block = lb->cb = new_code_block();
    compiler_push_logic_block(c, lb);
    visit_compound_statement(c, node->body);
    logic_block *old = compiler_pop_logic_block(c);
    DEC_REF(old);
    int size = LIST_SIZE(node->args->arg_list); 
    for(int i = 0; i < size; ++i) {
        ray_object* item = list_get(AS_OBJ(node->args->arg_list), i);
        store_obj(c, item);
    }
#ifdef PARSE_DEBUG
    R_DEBUG("--------------FUNC END-------------\n");
#endif
    function_object* f = new_function_object(node->args->arg_list, lb, size); 
    DEC_REF(lb);
    DEC_REF(node->args->arg_list);
    DEC_REF(node->args);
    int i = store_obj(c, AS_OBJ(f));
    ADD_OP_ARG(c, PUSH, i);
    DEC_REF(f);
    DEC_REF(node);
}

void visit_call_function(compiler* c, call_function_node* node) {
    list_object* l = node->args->arg_list;
    int size = l->size;
    for(int i = 0; i < size; ++i) {
        exp_node *e = (exp_node*)list_get(AS_OBJ(l), i);
        visit_exp(c, e);
    }
    DEC_REF(l);
    int i = store_obj(c, AS_OBJ(node->name));
    ADD_OP_ARG(c, LOAD_NAME, i);
    ADD_OP_INVOKE(c, INVOKE, size);
    DEC_REF(node->args);
    DEC_REF(node->name);
    DEC_REF(node);
}


void visit_exp(compiler *c, exp_node *node) {
    switch(node->etype) {
    case EXP_TYPE_OP: {
        calc_node* n = node->op;
        switch(node->op->o) {
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
            R_FATAL("Unknown operator type: %d", node->op->o);
        }
    }
    break;
    case EXP_TYPE_FACTOR:
        visit_factor(c, node->factor);
        break;
    case EXP_TYPE_FUNCTION_DEF:
        visit_function_def(c, node->function);
        break;
    case EXP_TYPE_CALL_FUNCTION:
        visit_call_function(c, node->call);
        break;
    default:
        R_FATAL("exp_node type %d error!\n", node->etype);
    }
    DEC_REF(node);
}
void visit_factor(compiler *c, factor_node * node) {
    switch(node->ftype) {
    case FACTOR_TYPE_INT:
    case FACTOR_TYPE_FLOAT:
    case FACTOR_TYPE_BOOL:
    case FACTOR_TYPE_NIL: {
            int i = store_obj(c, node->val);
            DEC_REF(node->val);
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
        int i = store_obj(c, node->val);
        ADD_OP_ARG(c, LOAD_NAME, i);
        DEC_REF(node->val);
        break;
    default:
        R_FATAL("factor_node type %d error!\n", node->ftype);
    }
    DEC_REF(node);
}


void visit_assign(compiler *c, assign_node * node) {
    visit_statement(c, node->rval);
    int i = store_obj(c, AS_OBJ(node->lval));
    DEC_REF(node->lval);
    ADD_OP_ARG(c, STORE_NAME, i);
    DEC_REF(node);
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
        int i = store_obj(c, AS_OBJ(nil));
        ADD_OP_ARG(c, PUSH, i);
    }
    
    use_code_block_next(c, end);
    DEC_REF(node);
}

void visit_print(compiler *c, print_statement_node *node) {
    visit_exp(c, node->exp);
    ADD_OP(c, DUP);
    ADD_OP(c, PRINT);    
    DEC_REF(node);
}
void visit_compound_statement(compiler *c, compound_statement_node *node) {
    statement_list_node* list_node = node->list;
    if(list_node) {
        visit_statement_list(c, list_node);
    }
    DEC_REF(node);
}
void visit_for_from_to (compiler* c, for_from_to_statement_node *node) {

    // from, init i 
    visit_statement(c, node->from);
    int i = store_obj(c, AS_OBJ(node->name));
    DEC_REF(node->name);
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
    DEC_REF(node);

}
void visit_statement(compiler *c, statement_node * node) {
    switch(node->stype) {
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
        R_FATAL("ast_node type %d error!", node->stype);
    }
    DEC_REF(node);
}

void code_gen(compiler *c, statement_list_node *node) {

    continue_compiler(c);
    visit_statement_list(c, node);
    DEC_REF(node);

}
void visit_statement_list(compiler *c, statement_list_node * node) {
    statement_node * n = node->next;
    while(1) {
        INC_REF(n);
        visit_statement(c, n);
        if(n->next) {
            ADD_OP(c, POP);
        }
        statement_node* tmp = n;
        n = n->next;
        DEC_REF(tmp);
        if(n == NULL) {
            break;
        }
    }
    DEC_REF(node);
}

