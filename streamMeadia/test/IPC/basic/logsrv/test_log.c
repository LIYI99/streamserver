
#include<unistd.h>
#include<time.h>
#include "logsrv.h"

#define DEBUG_PRINT(fmt,...) \
    printf("fmt,...");



static int      testcnt = 0;

static void*    test_log(void *data){
   
    basic_log_t *s = (basic_log_t *)data;
    time_t  temp;
    int i ;
    for(i = 0 ; i < 1000;i++)
    {
        time(&temp);
          
       
        basic_log_error_core(LOGS_ERR,s,"DATE:%s THPID:%d FUNC:%s LINE:%d CNT:%d\n",
                ctime(&temp),pthread_self(),__func__,__LINE__,testcnt++);
        usleep(10000);

    }
    return NULL;
}

int main(int argv,char**argc){

    basic_log_t*    log_t;

    log_t = basic_log_init("test_logsvr",LOGS_DEBUG,1024*4,"/home/710S/streamMeadia/test/IPC/basic/logsrv/testlogsrv_x.txt");
      pthread_t   test_id[10];
    
    int k = 0;
    printf("log_t:%p level:%d log_t->fp:%p\n",log_t,log_t->level,log_t->fp); 

#if 1
    FILE * testp = NULL;
    testp = fopen("./testxx","wt+");

    char  buf_t[40960];
    for(k =  0 ; k < 40960 ; k++)
        buf_t[k] = 'c';

    for(k = 0; k == 10000;k++)
        fwrite(buf_t,40960,1,testp);

    fclose(testp);
    sleep(1);
    return 0;
#endif 
    sleep(3);
    for(k =  0 ; k < 1 ; k ++){
    
        pthread_create(&test_id[k],NULL,test_log,(void *)log_t);
        
    }

    for (k = 0; k < 1 ; k++){
        
        pthread_join(test_id[k],NULL);
    }
     
    for(;;)
        sleep(1);
    baisc_log_destory(log_t);


    

    return 0;
}
