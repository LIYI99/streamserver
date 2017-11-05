#ifndef __DARR_QUEUE_H__
#define __DARR_QUEUE_H__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef  __cplusplus
    extern  "C"{
#endif

typedef struct queue_t {
    void *data;
    int max;
    int end;
    int front;
    int size;
    void *save;
}DARR_QUEUE;

#define DEBUG0(...)
#define DEBUG1(...)     fprintf(stderr, __VA_ARGS__);

#define ERRP(con, ret, flag, ...)   do       \
            {                                \
                if (con)                     \
                {                            \
                    DEBUG##flag(__VA_ARGS__) \
                    ret;                     \
                }                            \
            } while (0)



typedef void (queue_op_t)(const void *);

extern DARR_QUEUE *queue_create(int size, int max);
extern int queue_isempty(DARR_QUEUE *handle);
extern int queue_isfull(DARR_QUEUE *handle);
extern int queue_len(DARR_QUEUE *handle);
extern int queue_en(void *data, DARR_QUEUE *handle);
//线程全安函数
extern int  queue_de_v2(DARR_QUEUE *handle,void *buf);
//非安全函数
extern void *queue_de(DARR_QUEUE *handle);
extern void queue_travel(queue_op_t *op, DARR_QUEUE *handle);
extern void queue_destroy(DARR_QUEUE **handle);

#ifdef  __cplusplus
}
#endif

#endif 
