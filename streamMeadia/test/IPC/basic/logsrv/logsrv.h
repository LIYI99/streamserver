#ifndef __LOG_SRV_H__
#define __LOG_SRV_H__

#include <pthread.h>

#define     log_stderr      STDERR_FILENO
#define     LOGS_STDERR     0
#define     LOGS_ERR        1
#define     LOGS_WING       2
#define     LOGS_NOTICE     3
#define     LOGS_INFO       4
#define     LOGS_DEBUG      5



#ifdef  __cplusplus
    extern  "C"{
#endif
    
    

    typedef struct basic_log_s basic_log_t;

    struct basic_log_s{
        char*   logs_name;
        unsigned int    level;
        void*   p;
        pthread_mutex_t cache_lock;
        unsigned int    size;
        void*   pos;
        char*   path;
        FILE*   fp;
        unsigned int    logcnt;
        
        pthread_t       write_t;
        pthread_mutex_t cond_lock;
              pthread_cond_t  wait_d;
       
};


    
    basic_log_t*    basic_log_init(char* logs_name,unsigned int level, unsigned int cahce_size,
            char* path);
    void    baisc_log_destory(basic_log_t *log_t);
    void    basic_log_error_core(unsigned int  level, basic_log_t *log ,
        const char *fmt, ...);




#ifdef  __cplusplus
}
#endif



#endif

