#include<stdio.h>
#include "allocator.h"

int main() {
    debug_memory(1);
    char *a = (char *)r_malloc(sizeof(char));
    char *b; 
    *a = 1;
    printf("%p\n", a); 
    debug_memory(1);
    a = (char *)r_malloc(sizeof(char));
    *a = 2;
    printf("%p\n", a); 
    debug_memory(1);
    r_free(a);
    b = (char *)r_malloc(sizeof(char));
    *b = 3;
    printf("%p\n", b); 
    debug_memory(1);
    long * save[100];
    for(int i = 0; i < 10000; ++i) {
        long *c = (long *)r_malloc(sizeof(long));
        *c = i;
        save[i] = c;
        printf("%p\n", c);
        debug_memory(sizeof(long));
    }
    for(int i = 50; i < 100; ++i) {
        r_free(save[i]);
    }
    for(int i = 0; i < 50; ++i) {
        long *c = (long *)r_malloc(sizeof(long));
        *c = i;
        save[i] = c;
        printf("%p\n", c);
        debug_memory(sizeof(long));
    }
    return 0;
 }

