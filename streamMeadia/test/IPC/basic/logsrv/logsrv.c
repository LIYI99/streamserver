

#include"logsrv.h"
#include<string.h>
#include<stdarg.h>

#define     CACHE_SALESIZE  512 
#define     DEFAULT_PATH    "./logsrv.txt"


static void*    basic_log_write_file(void*data)
{
    if ( !data )
        return NULL;

    basic_log_t *s =  (basic_log_t *)data;

    while(1){

        pthread_cond_wait(&s->wait_d, &s->cond_lock);
        if(s->fp == NULL){
            pthread_mutex_unlock(&s->cond_lock);
            break;
        }    
        pthread_mutex_unlock(&s->cond_lock);
        
        pthread_mutex_unlock(&s->cache_lock);
        fwrite(s->p,s->pos - s->p,1,s->fp);
        s->pos = s->p;
        pthread_mutex_unlock(&s->cache_lock);

    }
    
    return 0;
}

  
static unsigned int     default_path_cnt = 0;
basic_log_t*    basic_log_init(char* logs_name,unsigned int level, unsigned int cache_size,
            char* path)
{
        basic_lot_t* s = NULL;
        s = (basic_log_t*)malloc(sizeof(basic_log_t));
        if(s == NULL)
            return NULL;
        unsigned int size_n = 0;
        if(logs_name)
            size_n = strlen(logs_name);
        else
            size_n = 0;
        if(size_n)
            s->logs_name = malloc(size_n+1);
        else
            s->logs_name = NULL;
        if(s->logs_name)
            strcpy(s->logs_name,logs_name);
        else
            sprintf(stderr,"logs_name is NULL\n");

       
        s->p =  malloc(size);
        if(s->p == NULL)
            goto ERR1;
        s->size = cache_size;
        s->pos = s->p;
        
               if(path)
            size_n = strlen(path);
        else
        {
            size_n = strlen(DEFAULT_PATH);
            size_n  += 4;
        }
        
        size_n +=1;
        s->path = malloc(size_n);

        if(s->path == NULL)
            goto ERR2;

        if(s->path && path)
            strcpy(s->path,path);

        else 
            sprintf(s->path,"%s_%d",DEFAULT_PATH,default_path_cnt++);

        s->fp = fopen(s->path,"rt+");
        if(f->fp == NULL)
            goto ERR3;
        
        s->logcnt =0;
        
        pthread_mutex_init(&s->cache_lock);
        pthread_mutex_init(&s->cond_lock);
        pthread_cond_init(&s->wait_d);
        pthread_create(&s->write_t,NULL, basic_log_write_file,s);


        return s;

    ERR3:

        free(s->path);

    ERR2:
        free(s->p);

    ERR1:
        
        if(s->logs_name)
            free(s->logs_name);
        
        free(s);
        
        return NULL;

}

void    baisc_log_destory(basic_log_t *log_t)
{
    
    if(!log_t)
        return ;

    if(log_t->logs_name)
        free(log_t->logs_name);
    if(log_t->path)
        free(log_t->path);
    if(log_t->fp)
        fclose(lot_t->fp);
    if(log_t->p)
        free(log_t->p);
    
    lot_t->p = NULL;
    lot_t->fp = NULL;
    
    pthread_cond_signal(&log_t->wait_d);
    pthread_join(log_t->write_t,NULL);

    pthread_mutex_destroy(&log_t->cache_lock);
    pthread_mutex_destroy(&log_t->cond_lock);
    pthread_cond_destroy(&log_t->wait_d);
   

    free(log_t);
    
    return;

}
 void    basic_log_error_core(unsigned int  level, basic_log_t *s ,
        const char *fmt, ...)

{
    
    if(level > log->level)
       return 0;
    char logs[512];
    va_list args;
    unsigned int n;
    
    va_start(args);
    n = _vnsprintf(logs, 512,fmt,args);
    va_end(args);
    fprintf(STDERR_FILENO,"%s",logs);
    
    //must be quikly
    pthread_mutex_lock(&s->cache_lock);
    if((s->size - (s->pos - s->p)) < n )
        n = (s->size - (s->pos -s->p));
    memcpy(s->pos,logs,n);
    s->pos += n;
    s->logcnt++;
    pthread_mutex_unlock(&s->cache_lock);
    
    if((s->size - (s->pos - s->p)) < CACHE_SALESIZE  )
    {
        if(s->fp){
            pthread_mutex_lock(&s->cond_lock);
            pthread_cond_signal(&s->wait_d);
        }else{
            pthread_mutex_lock(&s->cache_lock);
            s->pos = s->p;
            pthread_mutex_unlock(&s->cache_lock);
        }
    }
    
    
    return ;

}


