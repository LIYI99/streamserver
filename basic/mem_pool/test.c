
#include<unistd.h>
#include<time.h>
#include "mem_pool.h"
#include<stdio.h>
#include <stdlib.h>
#include "rbtree.h"
#include "darr_queue.h"

#define TEST_TIMES      1000*10000


static unsigned int testcnt = 0 ;
pthread_mutex_t     testlock;

static void*    test_func(void *data){
   
    MEM_POOL_ND *s =  (MEM_POOL_ND *) data;
    
    struct  timeval tv1 , tv2;
    int i = 0,k = 0;
    void *p = NULL;
    
    gettimeofday(&tv1,NULL);

    for(i = 0 ; i <  TEST_TIMES; i++){
        k =  rand()%10000+1;
        p =  mem_pool_malloc(s,k);
        //printf("p :%p\n",p);
        mem_pool_free(s,p);
        p = NULL;

    }
    gettimeofday(&tv2,NULL);
    printf("malloc and free :%d times,use usec:%ld\n",TEST_TIMES,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);

    return NULL;
}

static void*    test_func_cmp(void *data){
   
    MEM_POOL_ND *s =  (MEM_POOL_ND *) data;
    
    struct  timeval tv1 , tv2;
    int i = 0,k = 0;
    void *p = NULL;
    
   
    
    gettimeofday(&tv1,NULL);

    for(i = 0 ; i <  TEST_TIMES; i++){
        k =  35678;
        p =  mem_pool_malloc(s,k);
        mem_pool_free(s,p);
        p = NULL;

    }
    gettimeofday(&tv2,NULL);
    printf("POOL_malloc and free :%d times,use usec:%ld\n",TEST_TIMES,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);
    
    gettimeofday(&tv1,NULL);
    for(i = 0 ; i <  TEST_TIMES; i++){
        k =  35678*1000;
        p =  malloc(k);
        free(p);
        p = NULL;

    }
    gettimeofday(&tv2,NULL);
    printf("SYSTEM_malloc and free :%d times,use usec:%ld\n",TEST_TIMES,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);


    return NULL;
}



int main(int argv,char**argc){


    MEM_POOL_ND *   pool_nd;
    pool_nd =  mem_pool_node_create(1024*1024,1);
    if(pool_nd == NULL){
        printf("create pool_nd fail\n");
    }
  
    pthread_t   test_id[1000];
    
    pthread_mutex_init(&testlock,NULL);
    
    int k = 0,thds = 1;
    

    for(k =  0 ; k < thds ; k ++){
    
        pthread_create(&test_id[k],NULL,test_func_cmp,(void *)pool_nd);
        
    }

    for (k = 0; k < thds ; k++){
        
        pthread_join(test_id[k],NULL);
    }
  
    mem_pool_node_destroy(pool_nd);
    printf("test end \n");  
    

    return 0;
}
