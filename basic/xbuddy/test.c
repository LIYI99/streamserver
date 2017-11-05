#include "xbuddy_mempool.h"
#include <stdio.h>
#include <time.h>

struct buddy_mem_s{
  unsigned  nums;
  unsigned  block_size;
  void*     p;
  unsigned longest[0]; 
};

int main() {
    


    unsigned    block_size = 256,nums = 200;

    buddymem_t* s = NULL;
    s = buddymem_create(block_size,nums);
    printf("s:%p \n",s);


    void *p = NULL;
    
    unsigned    test_times = 10,i = 0;
    
    struct timeval tv1,tv2;
    printf("s:%p s->p:%p \n",s,s->p);
    

    gettimeofday(&tv1,NULL);

    for(i =  0 ; i < test_times ; i++){
        
        p = buddymem_alloc(s,512);
        printf("malloc p:%p s->p:%p \n",p,s->p);
        buddymem_free(s,p);
        p = buddymem_alloc(s,496);
        printf("malloc p:%p s->p:%p \n",p,s->p);

        buddymem_free(s,p);
    }
    gettimeofday(&tv2,NULL);
    printf("POOL_malloc and free :%d times,use usec:%ld\n",test_times,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);



    
    printf("test end \n");
    buddymem_destroy(s);
     
    return 0;





}
