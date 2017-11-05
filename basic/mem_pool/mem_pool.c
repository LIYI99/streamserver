
#include "mem_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "rbtree.h"
#include "darr_queue.h"
#define     RBTREE_NODE_POOL_MAX        10000


typedef struct mem_rbtree_s  mem_rbtree_t;


struct  mem_rbtree_s{
    
    rbtree_t        tree;
    DARR_QUEUE      *tree_node_pool;
    rbtree_node_t   *nodes;

};

typedef struct malloc_list {
    void* p;
    struct malloc_list *last;
    struct malloc_list *next;

}MALLOC_LIST;

struct mem_pool_node{
    pthread_mutex_t     pool_lock;
    unsigned int        lock_on;
    unsigned int        mem_size;
    unsigned int        used_size;
    unsigned int        used_cnts;
    void*               p;
    mem_rbtree_t        mem_tree;
    MALLOC_LIST         malloc_list;
};



static inline void mem_tree_init(mem_rbtree_t *tree ,void* mem_start ,void* mem_end)
{
    
    rbtree_node_t *temp = NULL,*save = NULL;
    
    //init node pool  
    save = tree->nodes; 
    int i;
    printf("nodes:%p, tree_node_t size:%ld\n",tree->nodes,sizeof(rbtree_node_t));
    for(i = 0 ;i < RBTREE_NODE_POOL_MAX ; i++){
        temp  = save++;
        queue_en((void *)&temp, tree->tree_node_pool);
    }
   
    temp  = (rbtree_node_t *)queue_de(tree->tree_node_pool);
    rbtree_sentinel_init(temp);
    rbtree_init(&tree->tree,temp, rbtree_insert_value);

    //add start mem address    
    temp  = (rbtree_node_t *)queue_de(tree->tree_node_pool);
    if(temp != NULL){
        temp->key = (rbtree_key_t)(mem_start);
        temp->size = MEM_ALINE;
        rbtree_insert(&tree->tree, temp);
    }else{
        printf(" get tree node fail \n");
    }
    
    //add end address
    temp  = (rbtree_node_t *)queue_de(tree->tree_node_pool);
    if(temp != NULL){
        temp->key = (rbtree_key_t)(mem_end - MEM_ALINE);
        temp->size = MEM_ALINE;
        rbtree_insert(&tree->tree, temp);
    }else{
        printf(" get tree node fail \n");
    }

    return;

}



MEM_POOL_ND*    mem_pool_node_create( unsigned int mem_size,
        unsigned int pthread_lock_on)
{
   
    MEM_POOL_ND* s = NULL;
    s =  (MEM_POOL_ND *) malloc(sizeof(MEM_POOL_ND)+ mem_size);
    if(s == NULL)
        return NULL;
   
    memset(s,0x0,sizeof(MEM_POOL_ND));
    
    
    s->mem_tree.tree_node_pool = queue_create(sizeof(void *), RBTREE_NODE_POOL_MAX);
    if(s->mem_tree.tree_node_pool == NULL)
        goto ERR1;
    
    s->mem_tree.nodes = (rbtree_node_t *)malloc(sizeof(rbtree_node_t)*RBTREE_NODE_POOL_MAX);
    if(s->mem_tree.nodes == NULL)
        goto ERR2;
    memset(s->mem_tree.nodes,0x0,sizeof(rbtree_node_t)*RBTREE_NODE_POOL_MAX);
    
     
    s->mem_size = mem_size;
    s->p = s + sizeof(MEM_POOL_ND);
    s->lock_on = pthread_lock_on;
    if(s->lock_on)
        pthread_mutex_init(&s->pool_lock,NULL);

    mem_tree_init(&s->mem_tree,s->p,s->p + mem_size);
    s->used_cnts = 2;
    s->used_size = 2*MEM_ALINE ;
    return s;

ERR2:
    queue_destroy(&(s->mem_tree.tree_node_pool));
ERR1:
    free(s);
    return NULL;


}
void    mem_pool_node_destroy(MEM_POOL_ND *s)
{

    if(s == NULL)
        return ;
    if(s->mem_tree.tree_node_pool != NULL)
        queue_destroy(&(s->mem_tree.tree_node_pool));

    if(s->mem_tree.nodes != NULL)
        free(s->mem_tree.nodes);


    MALLOC_LIST  *ls ,*lx ;
    ls = lx = NULL;
    for(ls = s->malloc_list.next ; ls != NULL;   ){
        lx = ls->next;
        if(ls->p != NULL)
            free(ls->p);
        free(ls);

        ls = lx;
    }

    if(s->malloc_list.p != NULL)
        free(s->malloc_list.p);

    if(s->lock_on){
        pthread_mutex_destroy(&s->pool_lock);
    }

    free(s);

    return;
} 



void*   mem_pool_malloc(MEM_POOL_ND * s ,int size){

    if(s == NULL)
        return NULL;

    int aline_size =  size + (MEM_ALINE - size%MEM_ALINE);

    void *p = NULL;

    if(s->lock_on)
        pthread_mutex_lock(&s->pool_lock);

    if((s->mem_size - s->used_size) > aline_size)
    {
        p = rbtree_LDR_MEM(&s->mem_tree.tree,aline_size);
    }

    rbtree_node_t *node = NULL ;

    while( p )
    {
        node  = (rbtree_node_t *)queue_de(s->mem_tree.tree_node_pool);
        if(node != NULL)
        {
            node->key =  (rbtree_key_t)p;
            node->size = aline_size;
            rbtree_insert(&s->mem_tree.tree,node);
            s->used_size += aline_size;
            s->used_cnts++;
        }else{
            printf("not have class node \n");
        }

        break;


    }

    if(s->lock_on)
        pthread_mutex_unlock(&s->pool_lock);
    
   
    if(p != NULL)
        return p;

    p = malloc(size);
    if(p == NULL)
        return p;


    //add p int mem pool nd list
    MALLOC_LIST *ls = NULL;
    ls = (MALLOC_LIST *) malloc(sizeof(MALLOC_LIST));
    if(ls == NULL){
        free(p);
        return NULL;
    }
    ls->p = p;
    ls->next = NULL;
    ls->last = NULL;

    MALLOC_LIST *lx = &s->malloc_list;
    if(s->lock_on)
        pthread_mutex_lock(&s->pool_lock);

    while(lx->next)
        lx = lx->next;
    lx->next = ls;
    ls->last = lx;

    if(s->lock_on)
        pthread_mutex_unlock(&s->pool_lock);


    return p;

}

void  mem_pool_free(MEM_POOL_ND* s,void* p)
{
   
    if(s == NULL || p == NULL)
       return ;
    
    if(s->lock_on)
        pthread_mutex_lock(&s->pool_lock);
    
    rbtree_node_t *node = NULL;
    while( (p > s->p) && (p < (s->p+s->mem_size)))
    {
       node = rbtree_find(&(s->mem_tree.tree), (rbtree_key_t)p);
        if(node){
            rbtree_delete(&(s->mem_tree.tree),node);
            queue_en((void *)&node, s->mem_tree.tree_node_pool);
            s->used_cnts--;
            s->used_size-= node->size;
        }
        break;

    }
    if(s->lock_on)
        pthread_mutex_unlock(&s->pool_lock);
    
    if(node){
        return;
    }

    MALLOC_LIST *ls = NULL;
    if(s->lock_on)
        pthread_mutex_lock(&s->pool_lock);
    ls = s->malloc_list.next;
    while(ls != NULL)
    {
      if(ls->p == p)
          break;
      ls = ls->next;
    }
    
    if(ls == NULL){
        if(s->lock_on)
            pthread_mutex_unlock(&s->pool_lock);
        fprintf(stderr,"not malloc this buf->p:%p\n",p);
        return;
    }
   // printf("free ls->next:%p \n",ls->next);
    if(ls->next != NULL)
        ls->next->last = ls->last;
    ls->last->next = ls->next;
    if(s->lock_on)
        pthread_mutex_unlock(&s->pool_lock);
    
    free(ls->p);
    free(ls);
    
    return;


}



