#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
void eval(code_block *b) {
    ray_object **stack = (ray_object**)malloc(sizeof(ray_object*) * STACK_SIZE);
    int stack_pos = 0;
    int i = 0;
    int len = b->code_len;
    ray_object* arg = NULL;
    op_t opcode = NULL;
    while(i < len) {
        instr *instruction = &b->code[i];
        opcode = instruction->opcode;
        if(HAS_OPARG(opcode)) {
            arg = instruction->oparg;
        }
        switch(opcode) {
            HANDLE(PUSH) {
                STACK_PUSH(arg);
                ++i;
            }
            break;
            HANDLE(D_ADD) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = (ray_object *)malloc(sizeof(ray_object));
                *v = *op1 + *op2;
                STACK_PUSH(v);
                ++i;
            }
            break;
            HANDLE(D_SUB) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = (ray_object *)malloc(sizeof(ray_object));
                *v = *op1 - *op2;
                STACK_PUSH(v);
                ++i;
            }
            break;
            HANDLE(D_MUL) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = (ray_object *)malloc(sizeof(ray_object));
                *v = *op1 * *op2;
                STACK_PUSH(v);
                ++i;
            }
            break;
            HANDLE(D_DIV) {
                ray_object *op1 = STACK_POP();
                ray_object *op2 = STACK_POP();
                ray_object *v = (ray_object *)malloc(sizeof(ray_object));
                *v = *op1 / *op2;
                STACK_PUSH(v);
                ++i;
            }
            break;
            HANDLE(PRINT) {
                ray_object *v = STACK_POP();
                printf("%lf\n", *v);
                ++i;
            }
            break;
        }
    }
}


