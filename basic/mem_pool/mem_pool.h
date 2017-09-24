/*
 * 2015 03 01
 * liyi 
 * */


#ifndef __CC_MEM_POOL_H__
#define __CC_MEM_POOL_H__



#include <pthread.h>
#include <time.h>
//每一个单独的内存池结点在线程上是安全的

#ifdef  __cplusplus
    extern  "C"{
#endif


typedef struct mem_pool_node    MEM_POOL_ND;
typedef struct malloc_list {
    void* p;
    struct malloc_list *last;
    struct malloc_list *next;

}MALLOC_LIST;


typedef struct mem_manage_node{
    unsigned int        index;
    void*               p;
    unsigned int        size;               // user request size ,It is a ordinary block if vaule be equal to 0.
    unsigned int        use;
    unsigned int        persistent_state;   // if value is not 0,than it is a start title.
}MEM_MANAGE_ND;

struct mem_pool_node{
                        //and a logsvr                
    pthread_mutex_t     pool_lock;
    unsigned int        lock_on;
    unsigned int        block_size;              
    unsigned int        block_nums;
    unsigned int        next_index;
    unsigned int        usecnts;
    MEM_MANAGE_ND*      mem_nd;
    void*               p;
    MALLOC_LIST         malloc_list;

};



MEM_POOL_ND*    mem_pool_node_create(unsigned int  block_size, unsigned int block_nums,
        unsigned int pthread_lock_on);
void            mem_pool_node_destroy(MEM_POOL_ND *mem_pool_nd);

void*           mem_pool_malloc(MEM_POOL_ND * mem_pool_nd ,int size);
void            mem_pool_free(MEM_POOL_ND* mem_pool_nd,void* p);




#ifdef  __cplusplus
}
#endif


#endif
