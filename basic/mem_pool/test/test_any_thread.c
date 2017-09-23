
#define __DEBUG__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "thread_pool.h"
#include "mem_pool.h"

#define THD_NUM 200 

static pthread_mutex_t lock;
static unsigned exit_cnt;
TH_POOL     *pTp;
MEM_PL_ND   *_mem_pl_nd = NULL;

static void test_mem_pool(int usleeps){
    
    char buf[128];
    MEM_ND *mem = mem_pool_node_malloc(_mem_pl_nd,32);
    if(!mem)
        return;
    sprintf(buf,"test___usleep%d\n",usleeps);
    mem_nd_write(buf,strlen(buf)+1,mem);
    buf[0] = '\0';
    mem_nd_read(buf,128,mem);
    usleep(usleeps);
    printf("read the mem test buf:%s\n",buf);
    mem_pool_node_free(mem);
}

void *proc_fun(void *arg){
	int i;
	int idx=*(int*)arg;
	i = 1000000.0 + (int)(9000000.0 * rand() / RAND_MAX);
	fprintf(stderr, "Begin: job %d, sleep %d us\n", idx, i);
	usleep(i);
	fprintf(stderr, "End:   job  %d\n", idx);
	pthread_mutex_lock(&lock);
	pthread_mutex_unlock(&lock);
    test_mem_pool(i); 
    exit_cnt++;

    printf("exit_cnt:%d\n",exit_cnt);
    if(exit_cnt == (THD_NUM)){
		printf("here -f \n");
        thread_pool_exit(pTp);
    }
    return NULL;
}

int main(int argc, char **argv){
    
    _mem_pl_nd = mem_pool_node_create(1024,50,512);
    mem_pool_node_init(_mem_pl_nd);
    mem_pool_node_disp_info(_mem_pl_nd,NULL);
    sleep(3);

    pTp= thread_pool_create(20, THD_NUM);
	int i;
    exit_cnt = 0;
	pthread_mutex_init(&lock, NULL);
	thread_pool_init(pTp);
	srand((int)time(0));
	for(i=0; i < THD_NUM; i++){
        printf("n:ci :%d\n",i);
		thread_pool_process_up(pTp, proc_fun, &i);
	}
    
	thread_pool_run(&pTp);
    printf("xxxxxxx\n");
	free(pTp);
	fprintf(stderr, "All jobs done!\n");
    sleep(3);
    printf("--------12---------\n");
    mem_pool_node_disp_info(_mem_pl_nd,NULL);
    printf("-----mem_pl_nd:%p------------\n",_mem_pl_nd);
    mem_pool_node_destroy(&_mem_pl_nd);
    sleep(5);
	return 0;
}

