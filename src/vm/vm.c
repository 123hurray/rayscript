#include <config.h>
#include <assert.h>
#include <vm.h>
#include <object.h>
#include <map_object.h>
#include <stdio.h>
#include <stdlib.h>

#define QUIT_VM(v, ...) do{ \
    R_ERROR(v, ##__VA_ARGS__);\
    goto clear;\
}while(0)


void eval(compiler *c) {
    code_block * b = c->lb->eval_block;
    ray_object **stack = (ray_object**)malloc(sizeof(ray_object*) * STACK_SIZE);
    int stack_pos = 0;
    int i = 0;
    int len = b->code_len;
    ray_object* arg = NULL;
    op_t opcode;
    while(b != NULL && i < len) {
        instr *instruction = &b->code[i];
        opcode = instruction->opcode;
        if(HAS_OPARG(opcode)) {
            arg = instruction->oparg;
        }
        switch(opcode) {
            HANDLE(STORE_NAME) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_GET();
                if(OBJ_IS_TRUE(STRING_EXACT(op1))) {
                    map_put((ray_object*)c->lb->locals, op1, op2);
                } else {
                    QUIT_VM("Invalid name!\n");
                }
                ++i;
            }
            break;
            HANDLE(LOAD_NAME) {
                ray_object *val = map_get((ray_object *)c->lb->locals, arg);
                if(val == NULL) {
                    QUIT_VM("Undefined variable %s\n", STRING_OBJ_AS_STRING(arg));
                }
                STACK_PUSH(val);
                ++i;
            }
            break;
            HANDLE(PUSH) {
                STACK_PUSH(arg);
                ++i;
            }
            break;
            HANDLE(D_ADD) {
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
            HANDLE(D_SUB) {
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
            HANDLE(D_MUL) {
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
            HANDLE(D_DIV) {
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
                if(NUMBER_EXACT(v)) {
                    printf("%lf\n", NUMBER_OBJ_AS_NUMBER(v));
                }
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
            HANDLE(JUMP_FALSE) {
                ray_object *op = STACK_POP();
                if(NUMBER_EXACT(op)) {
                    if(OBJ_IS_FALSE(OBJ_EQUALS(op, new_number_object_from_long(0L)))) {
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
        }
#ifdef VM_DEBUG
        R_DEBUG("STACK SIZE:%d\n", stack_pos);
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
    ray_object *ret = STACK_POP();
    printf("Execute result:%lf\n", NUMBER_OBJ_AS_NUMBER(ret));

clear:
    ;
}


