/*
 * 2015 03 01
 * liyi 
 * */


#ifndef __CC_MEM_POOL_H__
#define __CC_MEM_POOL_H__



#include "darr_queue.h"
#include <pthread.h>
#include <time.h>
//每一个单独的内存池结点在线程上是安全的

#ifdef  __cplusplus
    extern  "C"{
#endif


typedef struct mem_pool_node    MEM_PL_ND;

struct mem_pool_node{
    unsigned int        block_size;             //if block_size = 0 the pool is no rules pool 
    unsigned int        pool_max_nums;
    unsigned int        pool_cut_nums;
    unsigned int        pool_now_nums;
    int                 mem_cite_count;
    pthread_mutex_t     pool_lock;
    int                 freeing_falgs;
    DARR_QUEUE          *pool_queue;
    DARR_QUEUE          *pool_queue_copy;
    int                 re_val;
     
    MEM_PL_ND           *myself_nd;
};

typedef struct mem_node{
    MEM_PL_ND          *pool_nd;         
    unsigned int        pool_free_flags;
    unsigned int        isbusy;             //是否处于空闲状态
    int                 size;
    unsigned int        data_len;
    void                *buf;

}MEM_ND;

int mem_nd_write(char *in_buf,int len,MEM_ND *mem);
int mem_nd_write_v2(char *in_buf,int len,int offset,MEM_ND *mem);
int mem_nd_read(char *out_buf,int len,MEM_ND *mem);
void *mem_nd_re_bufpoint(MEM_ND *mem);

MEM_PL_ND  *mem_pool_node_create(int block_size,int pool_cut_nums,int pool_max_nums);
int       mem_pool_node_init(MEM_PL_ND *_nd);
void       mem_pool_node_destroy(MEM_PL_ND **_nd);
MEM_ND     *mem_pool_node_malloc(MEM_PL_ND *_nd,int size);
void       mem_pool_node_free(MEM_ND *mem);
void        mem_pool_node_manage(MEM_PL_ND *_nd);
int inline mem_pool_node_re_size(MEM_PL_ND *nd);
int inline  mem_pool_node_re_cutnunms(MEM_PL_ND *_nd);
int inline  mem_pool_node_re_maxnums(MEM_PL_ND *_nd);
int inline  mem_pool_node_re_citecount(MEM_PL_ND *_nd);
int inline  mem_pool_node_re_nownums(MEM_PL_ND *_nd);
int inline  mem_pool_node_get_re_val(MEM_PL_ND *_nd);
//re_buf get info message
void        mem_pool_node_disp_info(MEM_PL_ND *_nd,char *re_buf);




//同时开启多种内存池
typedef  struct mem_pool_node_set{
    int             _block_size;
    MEM_PL_ND*      _mem_pl_nd;
} MEM_PL_ND_ST;

#define     MAX_TYPE_SIZE_NODE 100

typedef  struct any_mem_pool_node {
    char            any_mem_name[32];
    MEM_PL_ND_ST    _mem_pl_nd_st[MAX_TYPE_SIZE_NODE];
    int             now_nums;
    int             max_nums;
    int             manange_time_interval;
    struct timeval  last_manage;
    int             inited_flags;
}ANY_MEM_PL_ND;


int     any_mem_pl_nd_init(char *name);
void    any_mem_pl_nd_close();
int     any_mem_pl_nd_add_node(int block_size,int pool_cut_nums,int pool_max_nums);
int     any_mem_pl_nd_del_node(int block_size);
void*   any_mem_pl_nd_manange(void *data);
MEM_ND*  any_mem_pl_nd_alloc(int need_size);
void    any_mem_pl_nd_free(MEM_ND *mem);
void    any_mem_pl_nd_get_info(char *get_buf);

#ifdef  __cplusplus
}
#endif


#endif
