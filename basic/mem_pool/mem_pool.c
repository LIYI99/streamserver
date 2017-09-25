
#include "mem_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


MEM_POOL_ND*    mem_pool_node_create(unsigned int  block_size, unsigned int block_nums,
        unsigned int pthread_lock_on)
{
   
    MEM_POOL_ND* s = NULL;
    s =  (MEM_POOL_ND *) malloc(sizeof(MEM_POOL_ND)+ block_nums * block_size);
    if(s == NULL)
        return NULL;
   
    memset(s,0x0,sizeof(MEM_POOL_ND));
    
    s->block_size = block_size;
    s->block_nums = block_nums;

    s->mem_nd = (MEM_MANAGE_ND*)malloc(sizeof(MEM_MANAGE_ND)*s->block_nums);
    if(s->mem_nd == NULL)
        goto ERR1;
    memset(s->mem_nd,0x0,sizeof(MEM_MANAGE_ND)*s->block_nums);
    
    s->p = s + sizeof(MEM_POOL_ND);

    s->lock_on = pthread_lock_on;
    if(s->lock_on)
        pthread_mutex_init(&s->pool_lock,NULL);

    //init manange node 
    unsigned int  k =  0;
    void*   pt = s->p;
    for(k = 0; k < s->block_nums; k++){
        
        s->mem_nd[k].index = k;
        s->mem_nd[k].p = pt;
        pt+= s->block_size;

    }
    
    return s;

ERR1:
    free(s);
    return NULL;


}
void            mem_pool_node_destroy(MEM_POOL_ND *s)
{
    if(s == NULL)
        return ;
    
    if(s->mem_nd != NULL)
        free(s->mem_nd);

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


static int  find_block_index(MEM_POOL_ND * s ,unsigned int next_index,int n)
{
    int k = 0,cnt = 0 ;

    for (k = 0 ; k < s->block_nums; k++, next_index++)
    {
        int index = next_index % s->block_nums;
            
        if( s->mem_nd[index].use == 0)
            cnt++;
        if( s->mem_nd[index].use == 2 || s->mem_nd[index].use == 1)
            cnt = 0;
        
        if( s->mem_nd[index].use == 1){
            k += s->mem_nd[index].size;
            next_index += s->mem_nd[index].size;
        }

        if( cnt == n )
        {
            int j = 0;
            j = next_index % s->block_nums - n ;
            if( j >= 0)
                break;

            k -= (n+j);             // draw back nums block of 0 index s->mem_nd nums
            next_index -= (n+j);    //draw back 0 index of s->mem_nd
            cnt = 0 ;
        }
    }
    
    //printf("xxxfunc:%s,cnt:%d need n:%d next_index m s->block_nums:%d\n",__func__,cnt,n,next_index%s->block_nums);
    if(cnt == n) 
        return next_index%s->block_nums;
    else 
        return -1;

}

void*   mem_pool_malloc(MEM_POOL_ND * s ,int size){
    if(s == NULL)
        return NULL;
    
    int n =  size / s->block_size;
    if( size % s->block_size )
        n += 1;
    if(n <= 0)
        return NULL;

    //printf("#####size:%d n:%d\n",size,n);
    void *p = NULL;
    int index;
    
    if(s->lock_on)
       pthread_mutex_lock(&s->pool_lock);
    
    while((s->block_nums - s->usecnts) > n)
    {
        index = find_block_index( s ,s->next_index,n);
        if(index < 0)
            break;
        //init malloc mem block  ALLOC_LIST *ls = NULL;
        int k ;
        for(k = 0 ;k < n; k++)
            s->mem_nd[index-k].use = 2;
     //   printf("xxxx1 idex - n :%d index:%d\n",index -n,index );
        //init malloc mem title node 
        s->usecnts+= n;
        n = n -1;

        s->mem_nd[index-n].use = 1;
        s->mem_nd[index-n].size = n+1; //write used nums block 
        p = s->mem_nd[index-n].p;

        s->next_index = index;
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
    int index = -1;
     
    if(s->lock_on)
        pthread_mutex_lock(&s->pool_lock);
    while(p <= (s->p + s->block_nums*s->block_size)){
        
        if( p  <  s->p )
            break;
        index = (p - s->p)/s->block_size;
        
        //printf("p ==%p, s->p==%p  p - s->p:%ld index:%d\n",p,s->p,p - s->p,index);

        if((p - s->p) % s->block_size != 0)
            index+=1;
       // printf("free index:%d\n",index);
        s->usecnts -= s->mem_nd[index].size;
        
        int i ,blocks ;
        blocks = s->mem_nd[index].size;
        s->mem_nd[index].size = 0;
        for(i = 0; i < blocks ; i++,index++ )
            s->mem_nd[index%s->block_nums].use = 0;
        break;
    }
    
    if(s->lock_on)
        pthread_mutex_unlock(&s->pool_lock);

    if(index >= 0)
        return;
   // printf("start free malloc node p:%p\n",p);
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



