
#include "object_pool.h"

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define     MAX_TEST_NODE       10
#define     MAX_TEST_TIMES      1000*100000

struct  test_x{
    int8_t          x;
    float           f;
    unsigned int    i;
    long     int    l;

};

static void  test_memset(void *data)
{
    
    struct test_x *x =  (struct test_x *)data;
    
    x->x = 9;
    x->f = 9.1;
    x->i = 9888;
    x->l = 9999;
   // printf("call :%s \n",__func__);
    return ; 
}

static void test_setx(struct test_x *x){

    
    x->x = 1;
    x->f = 1.2;
    x->i = 1.3;
    x->l = 1.6;
    return ;

}
static void test_print(void *p){
     
    printf("test_x->x:%d,x->f:%f,x->f:%d x->l:%ld\n",
            ((struct test_x *)p)->x,((struct test_x *)p)->f,
            ((struct test_x *)p)->i,((struct test_x*)p)->l);


    return ;

}

int main(int argv,char **argc){

    
    unsigned node_size = sizeof(struct test_x);
    object_pool_t*  s = NULL;
    
    s = object_pool_create(node_size,MAX_TEST_NODE,NULL);

    void * p = NULL;
    
    void*  ts[MAX_TEST_NODE];


 struct  timeval tv1 , tv2;



    int k = 0;
    gettimeofday(&tv1,NULL);

    for(;k  < MAX_TEST_TIMES;k++){
        
        p =  object_pool_get(s);
        object_pool_free(s,p);
        
    }
    gettimeofday(&tv2,NULL);
    printf("malloc and free :%d times,use usec:%ld\n",MAX_TEST_TIMES,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);

    
    gettimeofday(&tv1,NULL);

    for(k  = 0;k  < MAX_TEST_TIMES;k++){
        
        p =  malloc(sizeof(struct test_x));
        free(p);
        
    }
    gettimeofday(&tv2,NULL);
    printf("malloc and free :%d times,use usec:%ld\n",MAX_TEST_TIMES,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);



#if 0
    for(;;k++){
        p = object_pool_get(s);
        if(p == NULL)
            break;
        ts[k] = p;
         printf("test_x->x:%d,x->f:%f,x->f:%d x->l:%ld\n",
            ((struct test_x *)p)->x,((struct test_x *)p)->f,
            ((struct test_x *)p)->i,((struct test_x*)p)->l);


        test_setx((struct test_x *)p);
        printf("test_x->x:%d,x->f:%f,x->f:%d x->l:%ld\n",
            ((struct test_x *)p)->x,((struct test_x *)p)->f,
            ((struct test_x *)p)->i,((struct test_x*)p)->l);

    }
    int i = 0;
    for(i = 0;i< k;i++){
        
        object_pool_free(s,ts[i]);
    }
    
    object_pool_deinfo(s,test_print);
#endif

    printf("k %d\n",k);






}
