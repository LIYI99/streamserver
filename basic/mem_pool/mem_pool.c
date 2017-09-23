#include "mem_pool.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int mem_nd_read(char *out_buf,int len,MEM_ND *mem){
    
    if(!mem || !out_buf)
        return -1;
    if(len > mem->size){
        len = mem->size;
    }
    memcpy(out_buf,mem->buf,len);
    return len;
}

int mem_nd_write(char *in_buf,int len,MEM_ND *mem){
    
    if(!mem || !in_buf)
        return -1;
    if(len > mem->size)
        return -2;
    memcpy(mem->buf,in_buf,len);
    return len;
}

int mem_nd_write_v2(char *in_buf,int len,int offset,MEM_ND *mem){
     
    if(!mem || !in_buf)
        return -1;
    if(len-offset > mem->size)
        return -2;
    memcpy(mem->buf+offset,in_buf,len);
    
    return len;
}

void * mem_nd_re_bufpoint(MEM_ND *mem){
    if(mem)
        return (void *)mem->buf;
    return NULL;
}

MEM_PL_ND  *mem_pool_node_create(int block_size,int pool_cut_nums,int pool_max_nums){
    MEM_PL_ND   *nd = NULL;
    nd = (MEM_PL_ND *)malloc(sizeof(MEM_PL_ND));
    if(!nd)
        return NULL;
    memset(nd ,0x0,sizeof(MEM_PL_ND));
    nd->block_size = block_size;
    nd->pool_max_nums = pool_max_nums;
    nd->pool_cut_nums = pool_cut_nums; 
    pthread_mutex_init(&nd->pool_lock, NULL);

    return nd;
}

static MEM_ND *mem_nd_create(MEM_PL_ND *_nd,int size){
    if(!_nd || size <= 0)
        return NULL;
    MEM_ND *mem_nd = NULL;
    mem_nd = (MEM_ND *)malloc(sizeof(MEM_ND));
    if(!mem_nd)
        return NULL;
    mem_nd->buf = (void *)malloc(size);
    if(!mem_nd->buf){
        free(mem_nd);
        return NULL;
    }
    mem_nd->pool_free_flags = 0;
    mem_nd->isbusy = 0;
    mem_nd->size = size;
    mem_nd->pool_nd = _nd;
    //printf("mem info : size:%d buf:%p\n",mem_nd->size,mem_nd->buf);

    return mem_nd;
}

static void  mem_nd_free(MEM_ND **_nd){
    if(!_nd || !*_nd)
        return ;
    free((*_nd)->buf);
    free(*_nd);
    *_nd = NULL;
    return;
}



int    mem_pool_node_init(MEM_PL_ND *_nd){
    
    if(!_nd)
        return -1;
    MEM_PL_ND *in_nd = _nd;

    in_nd->pool_queue = queue_create(sizeof(void *),in_nd->pool_max_nums);
    if(!in_nd->pool_queue){
        in_nd->re_val = -11;
        return -1;
    }
    
    in_nd->pool_queue_copy = queue_create(sizeof(void *),in_nd->pool_max_nums);
    if(!in_nd->pool_queue_copy){
        in_nd->re_val = -11;
        queue_destroy(&(in_nd->pool_queue));
        return -1;
    }
   //未做错误处理
    int k = 0;
    MEM_ND *_mem = NULL;
    for(k = 0 ;k < in_nd->pool_cut_nums;k++ ){
        _mem = mem_nd_create(_nd,in_nd->block_size);
        if(_mem){
            queue_en((void *)(&_mem),in_nd->pool_queue);
            queue_en((void *)(&_mem),in_nd->pool_queue_copy);
        }else{
            break;
        }
        _mem = NULL;
    }
    in_nd->pool_now_nums = k;
    //空间不够使用
    if(in_nd->pool_now_nums != in_nd->pool_cut_nums)
        in_nd->re_val = -12;
    in_nd->re_val = 0;
    in_nd->myself_nd = _nd;
    return 0 ;
}


static void mem_nd_write_flags(MEM_PL_ND *_nd){
    
    MEM_ND *mem = NULL;
    int   save = 0,ret = 0;
    void *data = (void *)&save;
    for(;;){
        ret = queue_de_v2(_nd->pool_queue_copy,data);
        if(ret < 0)
            break;
        mem = *(MEM_ND **)data;
        if(mem->pool_free_flags){
            queue_en((void *)&mem,_nd->pool_queue_copy);
            break;
        }else
            mem->pool_free_flags =1;
        queue_en((void *)&mem,_nd->pool_queue_copy);
    }
}

void       mem_pool_node_destroy(MEM_PL_ND **_nd){
     
    if(!_nd || !*_nd || (*_nd)->freeing_falgs)
        return;
    if((*_nd)->myself_nd == NULL){
        free(*_nd);
        *_nd = NULL;
        return; //未初始化
    }
        
    MEM_PL_ND   *in_nd = *_nd;
    MEM_ND      *mem = NULL;
    in_nd->freeing_falgs = 1;
    if(in_nd->mem_cite_count != 0){
         mem_nd_write_flags(in_nd);
    } 
    pthread_mutex_lock(&in_nd->pool_lock); 
    int save =0 ,ret =0;
    void *re = (void *)&save;
    for(;;){
        ret = queue_de_v2(in_nd->pool_queue,re);
        if(ret < 0)
            break;
        mem = *(MEM_ND **)re;
        mem_nd_free(&mem); 
    }
    pthread_mutex_unlock(&in_nd->pool_lock);
    pthread_mutex_destroy(&in_nd->pool_lock);
    DARR_QUEUE *qu =  in_nd->pool_queue;
    queue_destroy(&qu);
    qu = in_nd->pool_queue_copy;
    queue_destroy(&qu);
    free(in_nd);
    
    *_nd = NULL;
    return;

}
MEM_ND     *mem_pool_node_malloc(MEM_PL_ND *_nd,int size){

    if(size == 0 || _nd == NULL || size > _nd->block_size)
        return NULL;
    MEM_ND      *mem = NULL;
    MEM_PL_ND   *nd = _nd;
    if(nd->freeing_falgs){
        nd->re_val = -9;
        return NULL;
    }
    int save = 0 ,ret = 0;
    void *data = (void *)&save;
    pthread_mutex_lock(&nd->pool_lock);
    ret =  queue_de_v2(nd->pool_queue,data);
    pthread_mutex_unlock(&nd->pool_lock);
    if(ret < 0)
        mem = NULL;
    else
        mem =  *(MEM_ND **)data;
    if(mem == NULL && nd->pool_now_nums < nd->pool_max_nums){
        mem = mem_nd_create(nd->myself_nd,nd->block_size);
        if(mem){
            nd->pool_now_nums++;
            //printf("128v create mem:%p\n",mem);
            queue_en((void *)(&mem),nd->pool_queue_copy);
        }else
            nd->re_val =  -12;
    }
    if(mem != NULL){
        nd->mem_cite_count++;
        mem->isbusy = 1;            //已被调用
        return mem;
    }else{
        nd->re_val = -3; //pool is full
        return NULL;
    }
}

void     mem_pool_node_free(MEM_ND *mem){
        if(!mem) 
            return;
        MEM_ND *_mem = mem;
        MEM_PL_ND *nd = _mem->pool_nd;
        if(nd == NULL){
            mem_nd_free(&_mem);
            return;
        }
        //if mem pool node freed
        if(nd->freeing_falgs ||mem->pool_free_flags){
             //printf("pool freed ,now ourself free nd->freeing_falgs:%d pool_free_flags:%d mem;%p pool_nd:%p\n",
                     //nd->freeing_falgs,_mem->pool_free_flags,_mem,_mem->pool_nd);
             mem_nd_free(&_mem); 
        }else{
           if(mem->isbusy){  
                mem->isbusy = 0;
                pthread_mutex_lock(&nd->pool_lock);
                queue_en((void *)&_mem,nd->pool_queue);
                pthread_mutex_unlock(&nd->pool_lock);
                nd->mem_cite_count--;
           }
        }
}

static  void inline mem_pool_queue_clean (DARR_QUEUE *qu,void *data){
    
    if(!qu || !data)
        return;
    MEM_ND *mem = (MEM_ND *)data ,*mem2 = NULL;
    int save = 0 ,ret = 0;
    void *in_data = (void *)&save;
    int  len = 0 ,k = 0;;
    len = queue_len(qu);
    for(k =0; k< len; k++){
        ret = queue_de_v2(qu,in_data);
        if(ret < 0)
            break;
        mem2 = *(MEM_ND **)in_data;
        if(mem == mem2)
            break;
        else
            queue_en((void *)&mem2,qu);
        }

    return;
}

void        mem_pool_node_manage(MEM_PL_ND *_nd){
   
    if(_nd->pool_now_nums < _nd->pool_cut_nums)
       return;
    MEM_ND *mem = NULL;
    //pool useing samller 0.5
    int save = 0,ret = 0;
    void *da = (void *)&save;

    for(;;){
        if((_nd->mem_cite_count *10 /_nd->pool_now_nums) < 5
                &&_nd->pool_now_nums > _nd->pool_cut_nums){
            pthread_mutex_lock(&_nd->pool_lock);
            ret = queue_de_v2(_nd->pool_queue,da);
            pthread_mutex_unlock(&_nd->pool_lock);
            
            if(ret < 0)
                break;
            mem = *(MEM_ND **)da;
            mem_pool_queue_clean (_nd->pool_queue_copy,mem);
            mem_nd_free(&mem);
            _nd->pool_now_nums--;
        }else
            break;
    }
    return;
}


int inline mem_pool_node_re_size(MEM_PL_ND *nd){
    
    return nd->block_size;
}
int inline mem_pool_node_get_re_val(MEM_PL_ND *nd){
    return nd->re_val;
}

int inline  mem_pool_node_re_cutnunms(MEM_PL_ND *_nd){

    return _nd->pool_cut_nums;
}
int inline  mem_pool_node_re_maxnums(MEM_PL_ND *_nd){
    return _nd->pool_max_nums;
}
int inline  mem_pool_node_re_citecount(MEM_PL_ND *_nd){
    return _nd->mem_cite_count;
}
int inline  mem_pool_node_re_nownums(MEM_PL_ND *_nd){
    return _nd->pool_now_nums;
}

void mem_pool_node_disp_info(MEM_PL_ND *_nd,char *rebuf){
    
    char buf[512];

    sprintf(buf,"--- mem pool info-----\nblock size:%d\npool_max_nums:%d\npool_cut_nums:%d\npool_now_nums:%d\nmem_cite_count:%d\n",_nd->block_size,
            _nd->pool_max_nums,_nd->pool_cut_nums,_nd->pool_now_nums,_nd->mem_cite_count);
    if(rebuf)
        strcpy(rebuf,buf);

    printf("%s",buf);    
}



//--------------------------------------------------------------------------//

static ANY_MEM_PL_ND        _any_mem_pl_nd_only;  //唯一句柄 

#define ANY_MEM_POOL_ND_MANAGE_TIME     5      //5s

int any_mem_pl_nd_init(char *name){
    
    memset((void *)&_any_mem_pl_nd_only,0x0,sizeof(ANY_MEM_PL_ND));
    
    _any_mem_pl_nd_only.max_nums = MAX_TYPE_SIZE_NODE;
    _any_mem_pl_nd_only.manange_time_interval = ANY_MEM_POOL_ND_MANAGE_TIME;
    
    if( !name && strlen(name) < 30 )
        strcpy(_any_mem_pl_nd_only.any_mem_name,"any mem pool node is Jk");
    else
        strcpy(_any_mem_pl_nd_only.any_mem_name,name);

    _any_mem_pl_nd_only.inited_flags = 1; //每一个进程，只能开启一个内存池
    
    return 0;
}

void any_mem_pl_nd_close(){
    
    if(!_any_mem_pl_nd_only.inited_flags)
        return ;
    _any_mem_pl_nd_only.inited_flags =0 ;
    
    int i ;
    for(i = 0 ; i < _any_mem_pl_nd_only.now_nums ; i++){
        mem_pool_node_destroy(&_any_mem_pl_nd_only._mem_pl_nd_st[i]._mem_pl_nd);
    }
    memset((void *)&_any_mem_pl_nd_only,0x0,sizeof(ANY_MEM_PL_ND));
    
    return;
}
static void test_disp(){
    int i = 0;
    
    for(i = 0 ; i < _any_mem_pl_nd_only.now_nums ; i++){
             printf("----1 now_nums:%d i:%d  _block_size:%d mem_pl_nd:%p\n",_any_mem_pl_nd_only.now_nums,i,
                    _any_mem_pl_nd_only._mem_pl_nd_st[i]._block_size,_any_mem_pl_nd_only._mem_pl_nd_st[i]._mem_pl_nd);

    }
}
int     any_mem_pl_nd_add_node(int block_size,int pool_cut_nums,int pool_max_nums){
    

    if(_any_mem_pl_nd_only.now_nums == _any_mem_pl_nd_only.max_nums)
        return -1;  //满

    MEM_PL_ND *_mem_pl_nd = NULL;
    _mem_pl_nd = mem_pool_node_create(block_size,pool_cut_nums,pool_max_nums);
    if(!_mem_pl_nd)
        return -1;
    int ret = 0;
    mem_pool_node_init(_mem_pl_nd);
    if(ret < 0){ //初始化失败.
        mem_pool_node_destroy(&_mem_pl_nd);
        return -1;    
    }

    int  i,k = 0;
    //从小到大

    for(i = 0 ; i < _any_mem_pl_nd_only.now_nums ; i++){
        if(_any_mem_pl_nd_only._mem_pl_nd_st[i]._block_size > block_size){
                       memmove((void*)(&(_any_mem_pl_nd_only._mem_pl_nd_st[i+1])),
                   (void *)(&(_any_mem_pl_nd_only._mem_pl_nd_st[i])),
                   sizeof(MEM_PL_ND_ST)*(_any_mem_pl_nd_only.now_nums-i));
            _any_mem_pl_nd_only._mem_pl_nd_st[i]._block_size = block_size;
            _any_mem_pl_nd_only._mem_pl_nd_st[i]._mem_pl_nd = _mem_pl_nd;
            k = 1;
            test_disp();
            //printf("----1 now_nums:%d i:%d  _block_size:%d mem_pl_nd:%p\n",_any_mem_pl_nd_only.now_nums,i,
              //      _any_mem_pl_nd_only._mem_pl_nd_st[i]._block_size,_any_mem_pl_nd_only._mem_pl_nd_st[i]._mem_pl_nd);
            break;
        }
    }

   if(!k){
       printf("-----2\n");
        _any_mem_pl_nd_only._mem_pl_nd_st[_any_mem_pl_nd_only.now_nums]._block_size = block_size;
        _any_mem_pl_nd_only._mem_pl_nd_st[_any_mem_pl_nd_only.now_nums]._mem_pl_nd = _mem_pl_nd;
   }
    _any_mem_pl_nd_only.now_nums++;
    return 0;
}

int     any_mem_pl_nd_del_node(int block_size){
    if(block_size <= 0 ||
            block_size > _any_mem_pl_nd_only._mem_pl_nd_st[_any_mem_pl_nd_only.now_nums]._block_size)
        return -1;
    int i;
    
    MEM_PL_ND_ST    _nd_st;
    _nd_st._mem_pl_nd = NULL;



    for(i = 0 ; i < _any_mem_pl_nd_only.now_nums ; i++){
        if(_any_mem_pl_nd_only._mem_pl_nd_st[i]._block_size == block_size){
            _nd_st = _any_mem_pl_nd_only._mem_pl_nd_st[i];
             memmove((void*)&_any_mem_pl_nd_only._mem_pl_nd_st[i],
                   (void *)&_any_mem_pl_nd_only._mem_pl_nd_st[i+1],
                   sizeof(MEM_PL_ND_ST)*(_any_mem_pl_nd_only.now_nums -1));
 
            break;
        }
    }
    
    if(_nd_st._mem_pl_nd)
        mem_pool_node_destroy(&_nd_st._mem_pl_nd);
    return 0;    
}


void *any_mem_pl_nd_manange(void *data){

    int i = 0;
    for(i = 0 ; i < _any_mem_pl_nd_only.now_nums ; i++){
         mem_pool_node_manage(_any_mem_pl_nd_only._mem_pl_nd_st[i]._mem_pl_nd);
    }
    return NULL;
}

MEM_ND*  any_mem_pl_nd_alloc(int need_size){
    
    int i = 0;
    
    MEM_PL_ND *_mem_nd = NULL;
    for(i = 0 ; i < _any_mem_pl_nd_only.now_nums ; i++){
        if(need_size <= _any_mem_pl_nd_only._mem_pl_nd_st[i]._block_size){
            _mem_nd =  _any_mem_pl_nd_only._mem_pl_nd_st[i]._mem_pl_nd;
            //printf("here break\n");
            break;
        }
    }
    //printf("mem_pl_nd:%p need_size:%d i:%d\n",_mem_nd,need_size,i);
    MEM_ND  *mem = NULL; 
    if(_mem_nd)
        mem = mem_pool_node_malloc(_mem_nd,need_size);
    
    if(!mem){ //获取失败
        mem = (MEM_ND *)malloc(sizeof(MEM_ND));
        if(mem != NULL){
            mem->buf = (void *)malloc(need_size);
            if(mem->buf){
                mem->pool_nd = NULL;
                mem->pool_free_flags = 1; //使用后自动释放
                mem->isbusy= 0;
                mem->size = need_size;
            }
        }
    }
    
    if(!mem)
        return NULL;
    if(!mem->buf){
        free(mem);
        return NULL;
    }
    //printf("mem:%p\n",mem);
    return mem;
}


void    any_mem_pl_nd_free(MEM_ND *mem){
    if(mem)
        mem_pool_node_free(mem);
    return;
}

void    any_mem_pl_nd_get_info(char *get_buf){
    
    if(!get_buf)
        return ;


    char  buf[512],save[512];
    int     i = 0;
    for(i = 0 ; i < _any_mem_pl_nd_only.now_nums ; i++){
        mem_pool_node_disp_info(_any_mem_pl_nd_only._mem_pl_nd_st[i]._mem_pl_nd,buf);
        sprintf(save,"ID:%d block size:%d info:%s",i
                ,_any_mem_pl_nd_only._mem_pl_nd_st[i]._block_size,buf);
        strcat(get_buf,save);
    }
    return;

}



























