#include "mem_pool.h"


MEM_PL_ND   *_mem_pl_nd = NULL;

int main(int argc,char **argv){
  
    _mem_pl_nd = mem_pool_node_create(1024,1000,10240);
    sleep(1);

    mem_pool_node_init(_mem_pl_nd);
    mem_pool_node_disp_info(_mem_pl_nd);
    MEM_ND *mem = mem_pool_node_malloc(_mem_pl_nd,512);
    printf("mem:%p\n",mem);
    printf("---------------get a mem info---------------\n");
    
    mem_pool_node_disp_info(_mem_pl_nd);
    char buf[128];
    strcpy(buf,"12345678\n");
    printf("buf:%s buf len:%d",buf,strlen(buf));
     
    mem_nd_write(buf,strlen(buf)+1,mem);
    buf[0] = '\0';
    mem_nd_read(buf,128,mem);
    printf("read the mem test buf:%s\n",buf);
    printf("---------------free a mem info---------------\n");
    mem_pool_node_disp_info(_mem_pl_nd);
    mem_pool_node_destroy(&_mem_pl_nd);
    
    mem_pool_node_free(mem);
    sleep(5);
}
