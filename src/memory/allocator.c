#include<stdlib.h>
#include<stdio.h>
#ifdef uint
#undef uint
#endif
typedef unsigned int uint;
#ifdef uchar
#undef uchar
#endif
typedef unsigned char uchar;
typedef uchar block;
struct bucket {
    block* freelist;
    size_t offset;
    size_t max_offset;
};
typedef struct bucket * pb;
#ifndef MAX_BLOCK_SIZE
#define MAX_BLOCK_SIZE 8
#endif
#define IAX(x) NULL
#define IA(x) IAX(x), IAX(x)
static pb bucketlist[MAX_BLOCK_SIZE * 2] = {
    IA(0), IA(1), IA(2), IA(3), IA(4), IA(5), IA(6), IA(7)
};
#define PAGE_SIZE (1 << 12)
#define PAGE_SIZE_MASK (PAGE_SIZE - 1)
#define SIZE2INDEX(s) ((s + 7) >> 3)
static uchar *pool;
int available_bucket_index = 0;
void * r_malloc(size_t size) {
    if(size > MAX_BLOCK_SIZE) {
        return malloc(size);
    }
    int i = SIZE2INDEX(size);
    pb list = bucketlist[i + i];
    if(list != NULL) {
        block * b = list->freelist;
        if(b != NULL) {
            list->freelist = (block *)(*(block **)b);
            return (void *)b;
        }
        else {
            if(list->offset < list->max_offset) {
                b = (char *)list + list->offset;
                list->offset += i << 3;
            }
            return (void *)b;
        }
    }
    else {
        if(pool == NULL) {
            pool = (uchar *) ((unsigned long)malloc(PAGE_SIZE * 16) & ~(unsigned long)PAGE_SIZE_MASK);
        }
        bucketlist[i + i] = (pb)(pool + available_bucket_index * PAGE_SIZE);
        ++available_bucket_index;
        list = bucketlist[i + i];
        list->freelist = NULL;
        list->offset = sizeof(struct bucket) + (i << 3);
        list->max_offset = PAGE_SIZE;
        return (void *)((char *)list + sizeof(struct bucket));
    }
}
void debug_memory(int size) {
    int i = SIZE2INDEX(size);
    pb list = bucketlist[i + i];
    if(list == NULL) {
        return;
    }
    for(int j = 0; j < (unsigned long)(list->offset); ++j) {
        printf("%d ", (int)*((uchar*)list + j));
    }
    printf("\n");
}
#define BUCKET_ADDR(p) ((pb)((unsigned long)p & ~(unsigned long)PAGE_SIZE_MASK))
void r_free(void *p) {
    pb bucket = BUCKET_ADDR(p);
    block* head = bucket->freelist;
    bucket->freelist = (block *)p;
    *(block **)p = head;
}
