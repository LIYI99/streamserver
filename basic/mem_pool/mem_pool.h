/*
 * 2017 11 5
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

#define     MEM_ALINE                   sizeof(int)


typedef struct mem_pool_node    MEM_POOL_ND;

MEM_POOL_ND*    mem_pool_node_create( unsigned int mem_size,unsigned int pthread_lock_on);
void            mem_pool_node_destroy(MEM_POOL_ND *mem_pool_nd);

void*           mem_pool_malloc(MEM_POOL_ND * mem_pool_nd ,int size);
void            mem_pool_free(MEM_POOL_ND* mem_pool_nd,void* p);




#ifdef  __cplusplus
}
#endif


#endif
