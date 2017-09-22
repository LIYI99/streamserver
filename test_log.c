
#include<unistd.h>
#include<time.h>
#include "logsrv.h"

#define DEBUG_PRINT(fmt,...) \
    printf("fmt,...");



static unsigned int testcnt = 0 ;
pthread_mutex_t     testlock;

static void*    test_log(void *data){
   
    basic_log_t *s = (basic_log_t *)data;
    time_t  temp;
    int i,k =1000000 ;

    for(i = 0 ; i < 1000;i++)
    {
        
        time(&temp);
        pthread_mutex_lock(&testlock);
        basic_log_error_core(LOGS_ERR,s,"DATE:%s THPID:%d FUNC:%s LINE:%d CNT:%d\n",
                ctime(&temp),pthread_self(),__func__,__LINE__,k + testcnt);
        testcnt++;

        pthread_mutex_unlock(&testlock);
        usleep(10000);
        
    }
    return NULL;
}



int main(int argv,char**argc){

    basic_log_t*    log_t;

    log_t = basic_log_init("test_logsvr",LOGS_DEBUG,1024*4,"./testlogsrv_x.txt");
      pthread_t   test_id[1000];
    
   // printf("log_t:%p level:%d log_t->fp:%p\n",log_t,log_t->level,log_t->fp); 
    pthread_mutex_init(&testlock,NULL);
    
    int k = 0,thds = 100;
    

    for(k =  0 ; k < thds ; k ++){
    
        pthread_create(&test_id[k],NULL,test_log,(void *)log_t);
        
    }

    for (k = 0; k < thds ; k++){
        
        pthread_join(test_id[k],NULL);
    }
      
 //   sleep(5); 
    baisc_log_destory(log_t);

    pthread_mutex_destroy(&testlock);
    

    return 0;
}
