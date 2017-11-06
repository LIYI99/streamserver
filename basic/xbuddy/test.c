#include "xbuddy_mempool.h"
#include <stdio.h>
#include <time.h>



int main() {
    


    unsigned int     block_size = 863,nums = 1025;    

    buddymem_t* s = NULL;
    s = buddymem_create(block_size,nums);
    

    void *p = NULL,*p1 = NULL;
    
    unsigned int    test_times = 10*1,i = 0;
    
    struct timeval tv1,tv2;
#if 0   
    for(i = 0 ; i  < 10 ; i++){
        
        p = buddymem_alloc(s,128);
        p = buddymem_alloc(s,256);

    }
#endif

    gettimeofday(&tv1,NULL);
    int k = 0;
    for(i =  0 ; i < test_times ; i++){
        k = rand()%10000 + 1;
        p = buddymem_alloc(s,256);
        if(i < 3)
            printf("malloc :%p\n",p);
        buddymem_free(s,p);
    }
    gettimeofday(&tv2,NULL);
    printf("POOL_malloc and free :%d times,use usec:%ld\n",test_times,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);


    gettimeofday(&tv1,NULL);
    for(i =  0 ; i < test_times ; i++){
        k = rand()%1000000+ 1;
        p = malloc(k);
        free(p);
    }
    gettimeofday(&tv2,NULL);
    printf("SYSTEM_malloc and free :%d times,use usec:%ld\n",test_times,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);

    int xx = 0;
    gettimeofday(&tv1,NULL);
    for(i =  0 ; i < test_times ; i++){
        k = rand()%10000+1;
       // xx = 1024*k; 
    }
    gettimeofday(&tv2,NULL);
    printf("  x*y:%d times,use usec:%ld\n",test_times,
            (tv2.tv_sec - tv1.tv_sec) *1000*1000 + tv2.tv_usec - tv1.tv_usec);





    
    printf("test end \n");
    buddymem_destroy(s);
     
    return 0;





}
