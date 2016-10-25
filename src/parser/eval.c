#include "eval.h"
#include "object.h"
#include "map_object.h"
#include <stdio.h>
#include <stdlib.h>
void eval(code_block *b) {
    ray_object **stack = (ray_object**)malloc(sizeof(ray_object*) * STACK_SIZE);
    int stack_pos = 0;
    int i = 0;
    int len = b->code_len;
    ray_object* arg = NULL;
    op_t opcode;
    while(i < len) {
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
                    map_put((ray_object*)b->locals, op1, op2);
                    STACK_PUSH(op2);
                } else {
                    printf("Invalid name!\n");
                    exit(-1);
                }
                ++i;
            }
            break;
            HANDLE(LOAD_NAME) {
                ray_object *val = map_get((ray_object *)b->locals, arg);
                if(val == NULL) {
                    printf("Undefined variable %s", STRING_OBJ_AS_STRING(arg));
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
        }
    }
}


