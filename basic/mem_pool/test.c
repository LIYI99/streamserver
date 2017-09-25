
#include<unistd.h>
#include<time.h>
#include "mem_pool.h"
#include<stdio.h>
#include <stdlib.h>





static unsigned int testcnt = 0 ;
pthread_mutex_t     testlock;

static void*    test_func(void *data){
   
    MEM_POOL_ND *s =  (MEM_POOL_ND *) data;

    int i = 0,k = 0;
    void *p = NULL;
    for(i = 0 ; i < 1000; i++){
        k =  rand()%100000+1;
        printf("need size:%d\n",k);
        p =  mem_pool_malloc(s,k);

        usleep(k*3);
        printf("p :%p\n",p);
        mem_pool_free(s,p);
        p = NULL;

    }

  
    return NULL;
}



int main(int argv,char**argc){


    MEM_POOL_ND *   pool_nd;
    pool_nd =  mem_pool_node_create(1024,1024,1);

    if(pool_nd == NULL){
        printf("create pool_nd fail\n");
    }
    printf("s->lock_on:%d s->block_size:%d s->block_nums:%d \n \
            s->next_index:%d s->p:%p,s->mem_nd:%p,s:%p\n",
            pool_nd->lock_on,pool_nd->block_size,pool_nd->block_nums,
            pool_nd->next_index,pool_nd->p,pool_nd->mem_nd,pool_nd);


    pthread_t   test_id[1000];

    pthread_mutex_init(&testlock,NULL);
    
    int k = 0,thds = 100;
    

    for(k =  0 ; k < thds ; k ++){
    
        pthread_create(&test_id[k],NULL,test_func,(void *)pool_nd);
        
    }

    for (k = 0; k < thds ; k++){
        
        pthread_join(test_id[k],NULL);
    }
      
    printf("pool_nd->usecnts:%d,s->malloc_list.next:%p\n",pool_nd->usecnts,
            pool_nd->malloc_list.next);
    mem_pool_node_destroy(pool_nd);


    

    return 0;
}
