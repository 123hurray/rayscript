#include <config.h>
#include <assert.h>
#include <vm.h>
#include <object.h>
#include <map_object.h>
#include <stdio.h>
#include <stdlib.h>



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
                ray_object *op2 = STACK_POP();
                if(STRING_EXACT(op1)) {
                    map_put((ray_object*)c->lb->locals, op1, op2);
                    STACK_PUSH(op2);
                } else {
                    printf("Invalid name!\n");
                    exit(-1);
                }
                ++i;
            }
            break;
            HANDLE(LOAD_NAME) {
                ray_object *val = map_get((ray_object *)c->lb->locals, arg);
                if(val == NULL) {
                    printf("Undefined variable %s\n", STRING_OBJ_AS_STRING(arg));
                    exit(-1);
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
                number_object *v;
                if(NUMBER_EXACT(op1) && NUMBER_EXACT(op2)) {
                    v = new_number_object(NUMBER_OBJ_AS_NUMBER(op1) + NUMBER_OBJ_AS_NUMBER(op2));
                } else {
                    printf("Unsupport type %s", op2->type->name);
                }
                STACK_PUSH((ray_object *)v);
                ++i;
            }
            break;
            HANDLE(D_SUB) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                number_object *v;
                if(NUMBER_EXACT(op1) && NUMBER_EXACT(op2)) {
                    v = new_number_object(NUMBER_OBJ_AS_NUMBER(op2) - NUMBER_OBJ_AS_NUMBER(op1));
                }
                STACK_PUSH((ray_object *)v);
                ++i;
            }
            break;
            HANDLE(D_MUL) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                number_object *v;
                if(NUMBER_EXACT(op1) && NUMBER_EXACT(op2)) {
                   v = new_number_object(NUMBER_OBJ_AS_NUMBER(op1) * NUMBER_OBJ_AS_NUMBER(op2));
                }
                STACK_PUSH((ray_object *)v);
                ++i;
            }
            break;
            HANDLE(D_DIV) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                number_object *v;
                if(NUMBER_EXACT(op1) && NUMBER_EXACT(op2)) {
                    v = new_number_object(NUMBER_OBJ_AS_NUMBER(op2) / NUMBER_OBJ_AS_NUMBER(op1));
                }
                STACK_PUSH((ray_object *)v);
                ++i;
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
                    if(NUMBER_OBJ_AS_NUMBER(op) > 0.00001
                            || NUMBER_OBJ_AS_NUMBER(op) < -0.0001) {
                        ++i;
                    }
                    else {
                        b = b->code[i].jmp_block;
                        i = 0;
                        len = b->code_len;
                    }
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

    printf("Execute result:%lf\n", NUMBER_OBJ_AS_NUMBER(STACK_POP()));
}


