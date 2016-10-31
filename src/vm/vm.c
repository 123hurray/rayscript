#include <config.h>
#include <assert.h>
#include <vm.h>
#include <object.h>
#include <map_object.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef VM_DEBUG 
#define QUIT_VM(v, ...) do{ \
    R_DEBUG("\n");\
    R_ERROR(v, ##__VA_ARGS__);\
    goto clear;\
}while(0)
#else
#define QUIT_VM(v, ...) do{ \
    R_ERROR(v, ##__VA_ARGS__);\
    goto clear;\
}while(0)
#endif

#define VM_HANDLE_OP_BIN(op) \
    HANDLE(OP_##op) {\
                ray_object *op2 = STACK_POP();\
                ray_object* op1 = STACK_POP();\
                ray_object* val = (ray_object*)OBJ_BOOL_FROM_INT(OBJ_##op(op1, op2));\
                STACK_PUSH(val);\
                ++i;\
            }\
            break



void eval(compiler *c) {
    ray_object* ret;
    logic_block* lb_save = c->lb;
    code_block * b = c->lb->eval_block;
    ray_object **stack = c->lb->stack;
    int stack_pos = c->lb->stack_pos;
    int i = c->lb->pc;
    int len = b->code_len;
    int arg = 0;
    op_t opcode;
    while(c->lb != NULL) {
    while(b != NULL && i < len) {
        instr *instruction = &b->code[i];
        opcode = instruction->opcode;
        if(HAS_OPARG(opcode)) {
            arg = instruction->oparg;
        }
        switch(opcode) {
            HANDLE_INVOKE(INVOKE) {
                ray_object* op1 = STACK_POP();
                list_object* l = new_list_object(arg);
                for(int i = 0; i < arg; ++i) {
                    ray_object* item = STACK_POP();
                    list_prepend(AS_OBJ(l), item);
                }
                ++i;
                logic_block* new_lb = OBJ_INVOKE(op1, l);

                // store runtime
                
                c->lb->stack_pos = stack_pos;
                c->lb->pc = i;

                compiler_push_logic_block(c, new_lb);

                b = c->lb->eval_block;
                stack = c->lb->stack;
                stack_pos = 0;
                i = 0;
                len = b->code_len;
            }
            break;
            HANDLE_ARG(STORE_NAME) {
                ray_object *op = STACK_GET();
                ray_object *v = list_get((ray_object*)c->lb->consts, arg);
                map_put((ray_object*)c->lb->locals, v, op);
                ++i;
            }
            break;
            HANDLE_ARG(LOAD_NAME) {
                ray_object* v = list_get((ray_object *)c->lb->consts, arg);
                ray_object *val = map_get((ray_object *)c->lb->locals, v);
                if(val == NULL) {
                    QUIT_VM("Undefined variable %s\n", STRING_OBJ_AS_STRING(v));
                }
#ifdef PARSE_DEBUG
                R_DEBUG("\b\b(%s)   ", STRING_OBJ_AS_STRING(OBJ_STR(val)));
#endif
                STACK_PUSH(val);
                ++i;
            }
            break;
            HANDLE_ARG(PUSH) {
                ray_object* v = list_get((ray_object *)c->lb->consts, arg);
                STACK_PUSH(v);
                ++i;
            }
            break;
            HANDLE(OP_ADD) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = OBJ_ADD(op1, op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    QUIT_VM("Unsupport %s add %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(OP_SUB) {
                ray_object *op2 = STACK_POP();
                ray_object *op1 = STACK_POP();
                ray_object *v = OBJ_SUB(op1, op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    QUIT_VM("Unsupport type %s sub type %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(OP_MUL) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = OBJ_MUL(op1, op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    QUIT_VM("Unsupport type %s mul type %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(OP_DIV) {
                ray_object *op2 = STACK_POP();
                ray_object *op1 = STACK_POP();
                ray_object *v = OBJ_DIV(op1, op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    QUIT_VM("Unsupport type %s div type %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(PRINT) {
                ray_object *v = STACK_POP();
                printf("%s\n", STRING_OBJ_AS_STRING(OBJ_STR(v)));
                ++i;
            }
            break;
            HANDLE(POP) {
               STACK_POP();
               ++i;
            }
            break;
            HANDLE(DUP) {
                ray_object *op = STACK_GET();
                STACK_PUSH(op);
                ++i;
            }
            break;
            HANDLE(JUMP) {
                b = b->code[i].jmp_block;
                i = 0;
                len = b->code_len;
            }
            break;
            VM_HANDLE_OP_BIN(EQ);
            VM_HANDLE_OP_BIN(NE);
            VM_HANDLE_OP_BIN(LT);
            VM_HANDLE_OP_BIN(GT);
            VM_HANDLE_OP_BIN(LE);
            VM_HANDLE_OP_BIN(GE);

            HANDLE(JUMP_TRUE) {
                ray_object *op = STACK_POP();
                if(OBJ_IS_TRUE(BOOL_EXACT(op))) {
                    if(OBJ_IS_FALSE(op)) {
                        ++i;
                    }
                    else {
                        b = b->code[i].jmp_block;
                        i = 0;
                        len = b->code_len;
                    }
                } else {
                    QUIT_VM("type error!\n");
                }
            }
            break;
            HANDLE(JUMP_FALSE) {
                ray_object *op = STACK_POP();
                if(BOOL_EXACT(op)) {
                    if(OBJ_IS_TRUE(op)) {
                        ++i;
                    }
                    else {
                        b = b->code[i].jmp_block;
                        i = 0;
                        len = b->code_len;
                    }
                } else {
                    QUIT_VM("type error!\n");
                }
            }
            break;
        default:
            QUIT_VM("Unknown op code %d\n", opcode);
        }
#ifdef VM_DEBUG
        R_DEBUG("STACK SIZE:%d", stack_pos);
#endif
        if(i == len) {
            b = b->next;
            i = 0;
            if(b != NULL) {
                len = b->code_len;
            }
        }
    }
        assert(stack_pos == 1);
        ret = STACK_POP();
        compiler_pop_logic_block(c);
        if(c->lb != NULL) {
            stack_pos = c->lb->stack_pos;
            stack = c->lb->stack;
            i = c->lb->pc;
            b = c->lb->eval_block;
            len = b->code_len;
            STACK_PUSH(ret);
        }
    }
    c->lb = lb_save;

#ifdef VM_DEBUG
    R_DEBUG("\n");
#endif
    printf("Execute result: %s\n", STRING_OBJ_AS_STRING(OBJ_STR(ret)));

clear:
    ;
}


