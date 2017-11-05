#include "darr_queue.h"


// modfy for storge tree node porint
DARR_QUEUE *queue_create(int size, int max)
{
    DARR_QUEUE * handle = NULL;

    handle = (DARR_QUEUE *)malloc(sizeof(DARR_QUEUE));
    ERRP(handle == NULL, goto ERR1, 0);

    handle->data = (void *)malloc(size * max);
    ERRP(handle->data == NULL, goto ERR2, 0);

    handle->save = (void *)malloc(size);
    ERRP(NULL == handle->save, goto ERR3, 0);

    handle->size = size;
    handle->max = max;
    handle->end = 0;
    handle->front = 0;
     return handle;
ERR3:
    free(handle->data);
ERR2:
    free(handle);
ERR1:
    return NULL;
}

int queue_isempty(DARR_QUEUE *handle)
{
    if (handle->end == handle->front)
        return 1;
    return 0;
}

int queue_isfull(DARR_QUEUE *handle)
{
    if ((handle->end + 1) % handle->max == handle->front)
        return 1;
    return 0;
}

int queue_len(DARR_QUEUE *handle)
{
    if (handle->end >= handle->front)
        return handle->end - handle->front;
    return handle->max - (handle->front - handle->end);
}

int queue_en(void *data, DARR_QUEUE *handle)
{
    if (queue_isfull(handle))
        return -1;

    void **st = NULL;
    st  = (void **)(handle->data);
    
    //quikly
    st[handle->end] = *((void **)data);
    //memcpy(handle->data + handle->end * handle->size, 
    //        data, handle->size);
    handle->end++;
    handle->end %= handle->max;
    return 0;
}

void *queue_de(DARR_QUEUE *handle)
{
    if (queue_isempty(handle))
        return NULL;
    *((void **)handle->save)  = ((void **)handle->data)[handle->front];
    //memcpy(handle->save, handle->data + handle->front * handle->size, handle->size);
    handle->front++;
    handle->front %= handle->max;
    
    return *((void **)handle->save);
}
//线程安全
int queue_de_v2(DARR_QUEUE *handle,void *buf)
{
    if (queue_isempty(handle))
        return -1;
    if(!buf)
        return -1;

    memcpy(buf, handle->data + handle->front * handle->size, handle->size);
    handle->front++;
    handle->front %= handle->max;
   // printf("__de the queue state  front:%d ,end;%d data;%p handle:%p\n",
     //       handle->front,handle->end,*(void **)buf,handle);
    return 0;
}

void queue_travel(queue_op_t *op, DARR_QUEUE *handle)
{
    int i, j;
    int len = queue_len(handle);

    for (i = 0, j = handle->front; i < len; i++, j++)
    {
        j %= handle->max;
        op(handle->data + j * handle->size); 
    }
}

void queue_destroy(DARR_QUEUE **handle)
{
    DARR_QUEUE *val = *handle;
    free(val->data);
    free(val->save);
    free(val);

    //*handle = NULL;
}

