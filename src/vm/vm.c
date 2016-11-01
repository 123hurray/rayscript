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
                DEC_REF(op1);\
                DEC_REF(op2);\
                INC_REF(val);\
                STACK_PUSH(val);\
                ++i;\
            }\
            break



void eval(compiler *c) {
    ray_object* ret = NULL;
    INC_REF(c->lb);
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
                    DEC_REF(item);
                }
                ++i;
                logic_block* new_lb = OBJ_INVOKE(op1, l);
                DEC_REF(op1);
                DEC_REF(l);

                // store runtime
                
                c->lb->stack_pos = stack_pos;
                c->lb->pc = i;

                compiler_push_logic_block(c, new_lb);
                DEC_REF(new_lb);

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
                    if(c->lb->globals != c->lb->locals) {
                        val = map_get((ray_object *)c->lb->globals, v);
                    }
                    if(val == NULL) {
                        QUIT_VM("Undefined variable %s\n", STRING_OBJ_AS_STRING(v));
                    }
                }
#ifdef PARSE_DEBUG
                R_DEBUG("\b\b(%s)   ", STRING_OBJ_AS_STRING(OBJ_STR(val)));
#endif
                INC_REF(val);
                STACK_PUSH(val);
                ++i;
            }
            break;
            HANDLE_ARG(PUSH) {
                ray_object* v = list_get((ray_object *)c->lb->consts, arg);
                INC_REF(v);
                STACK_PUSH(v);
                ++i;
            }
            break;
            HANDLE(OP_ADD) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = OBJ_ADD(op1, op2);
                DEC_REF(op1);
                DEC_REF(op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    DEC_REF(v);
                    QUIT_VM("Unsupport %s add %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(OP_SUB) {
                ray_object *op2 = STACK_POP();
                ray_object *op1 = STACK_POP();
                ray_object *v = OBJ_SUB(op1, op2);
                DEC_REF(op1);
                DEC_REF(op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    DEC_REF(v);
                    QUIT_VM("Unsupport type %s sub type %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(OP_MUL) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = OBJ_MUL(op1, op2);
                DEC_REF(op1);
                DEC_REF(op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    DEC_REF(v);
                    QUIT_VM("Unsupport type %s mul type %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(OP_DIV) {
                ray_object *op2 = STACK_POP();
                ray_object *op1 = STACK_POP();
                ray_object *v = OBJ_DIV(op1, op2);
                DEC_REF(op1);
                DEC_REF(op2);
                if(!OBJ_IS_NIL(v)) {
                    STACK_PUSH((ray_object *)v);
                    ++i;
                } else {
                    DEC_REF(v);
                    QUIT_VM("Unsupport type %s div type %s\n", OBJ_GET_TYPE_NAME(op1), OBJ_GET_TYPE_NAME(op2));
                }
            }
            break;
            HANDLE(PRINT) {
                ray_object *v = STACK_POP();
                string_object* str = OBJ_STR(v);
                printf("%s\n", STRING_OBJ_AS_STRING(str));
                DEC_REF(str);
                DEC_REF(v);
                ++i;
            }
            break;
            HANDLE(POP) {
               ray_object* v = STACK_POP();
               DEC_REF(v);
               ++i;
            }
            break;
            HANDLE(DUP) {
                ray_object *op = STACK_GET();
                INC_REF(op);
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
                    DEC_REF(op);
                } else {
                    DEC_REF(op);
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
                    DEC_REF(op);
                } else {
                    DEC_REF(op);
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
        DEC_REF(ret);
        ret = STACK_POP();
        logic_block *old = compiler_pop_logic_block(c);
        DEC_REF(old);
        if(c->lb != NULL) {
            stack_pos = c->lb->stack_pos;
            stack = c->lb->stack;
            i = c->lb->pc;
            b = c->lb->eval_block;
            len = b->code_len;
            INC_REF(ret);
            STACK_PUSH(ret);
        }
    }
    c->lb = lb_save;

#ifdef VM_DEBUG
    R_DEBUG("\n");
#endif
    string_object* str = OBJ_STR(ret);
    printf("Execute result: %s\n", STRING_OBJ_AS_STRING(str));
    DEC_REF(str);
    DEC_REF(ret);

clear:
    ;
}


