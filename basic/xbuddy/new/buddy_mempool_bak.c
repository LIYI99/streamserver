#include <stdlib.h>
//#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "buddy_mempool.h"



struct buddy_mem_s{
  unsigned  nums;
  unsigned  block_size;
  void*     p;
  unsigned longest[0]; 
};

#define     LEFT_LEAF(index) ((index) * 2 + 1)
#define     RIGHT_LEAF(index) ((index) * 2 + 2)
#define     PARENT(index) ( ((index) + 1) / 2 - 1)

#define     IS_POWER_OF_S(x) (!((x)&((x)-1)))
#define     MAX(a, b) ((a) > (b) ? (a) : (b))

#define     ALLOC malloc
#define     FREE free


static unsigned fixsize(unsigned size) {
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  return size+1;
}

buddymem_t* buddymem_create( int block_size,int nums )
{

    buddymem_t* s = NULL;
    unsigned    node_size = 0;

    if( nums < 1 || block_size < sizeof(int) || !IS_POWER_OF_S(nums))
        return NULL;
    
    s =  (buddymem_t *)malloc(sizeof(buddymem_t) + 
            nums*2*sizeof(unsigned) + block_size*nums);
    if(s == NULL)
        return NULL;
    
    memset(s,0x0,sizeof(buddymem_t));
    node_size = block_size *nums;

    s->nums = nums;
    s->p = ((void *)s)+sizeof(buddymem_t) + sizeof(unsigned)*nums*2;
    
    int i;
    for(i = 0; i < 2*nums -1 ; i++){
        if(IS_POWER_OF_S(i + 1))
            node_size /= 2;
        s->longest[i] = node_size;

    }
    
    return s;

}

void    buddymem_destroy( buddymem_t*  s)
{
    
    if(s)
        free(s);
    return;

}
void*   buddymem_alloc(buddymem_t *s, int size)
{
    if(s == NULL || size <= 0)
        return NULL;
    
    if(size % s->block_size !=  0)
        size = size + (s->block_size - (size%s->block_size));
    
    unsigned index = 0,node_size = 0,offset = 0;

    if(s->longest[index] < size)
        return NULL;
    
    node_size = s->nums *s->block_size;
    
    for( node_size; node_size != size; node_size /= 2 )
    {
        if (s->longest[LEFT_LEAF(index)] >= size)
            index = LEFT_LEAF(index);
        else
            index = RIGHT_LEAF(index);
    }
    
    s->longest[index] = 0;
    offset = (index + 1) * node_size - ( s->nums * s->block_size);
    
    while(index) 
    {
        index = PARENT(index);
        s->longest[index] = 
        MAX(s->longest[LEFT_LEAF(index)], s->longest[RIGHT_LEAF(index)]);
    }
    
    return s->p + offset;

}


void    buddymem_free(buddymem_t* s, void *p){

    if(s == NULL || p == NULL)
        return ;
    if(p > (s->p + s->block_size*s->nums))
        return;
    if(p < s->p)
        return;

    unsigned index = 0, offset = 0,node_size = s->block_size;

    offset = p - s->p;
    index = offset /s->block_size + s->nums -1;

    for (; s->longest[index] ; index = PARENT(index)) {
        node_size *= 2;
        if (index == 0)
            return;
    }
    s->longest[index] = node_size;
    unsigned left_longest = 0,right_longest = 0;
    while ( index ) 
    {
        index = PARENT(index);
        node_size *= 2;
        left_longest = s->longest[LEFT_LEAF(index)];
        right_longest = s->longest[RIGHT_LEAF(index)];
        if (left_longest + right_longest == node_size) 
            s->longest[index] = node_size;
        else
            s->longest[index] = MAX(left_longest, right_longest);
    }
    
    return;
}


int     buddymem_size(buddymem_t* s, void *p)
{

    if(s == NULL || p == NULL)
        return 0;
    if(p > s->p + s->block_size*s->nums)
        return 0;
    if(p < s->p)
        return 0;
    
    unsigned node_size = s->block_size;
    unsigned index = 0,offset = 0;

    offset  = (p - s->p)/s->block_size ;
    for (index = offset + s->nums - 1; s->longest[index] ; index = PARENT(index))
        node_size *= 2;
    
    return  node_size;
}


void    buddymem_dump(buddymem_t* s){
    
    return ;


}

#if 0
void buddy_mem_nd_dump(struct buddy_mem_nd* self) {
  char canvas[65];
  int i,j;
  unsigned node_size, offset;

  if (self == NULL) {
    printf("buddy_mem_nd_dump: (struct buddy_mem_nd*)self == NULL");
    return;
  }

  if (self->size > 64) {
    printf("buddy_mem_nd_dump: (struct buddy_mem_nd*)self is too big to dump");
    return;
  }

  memset(canvas,'_', sizeof(canvas));
  node_size = self->size * 2;

  for (i = 0; i < 2 * self->size - 1; ++i) {
    if ( IS_POWER_OF_2(i+1) )
      node_size /= 2;

    if ( self->longest[i] == 0 ) {
      if (i >=  self->size - 1) {
        canvas[i - self->size + 1] = '*';
      }
      else if (self->longest[LEFT_LEAF(i)] && self->longest[RIGHT_LEAF(i)]) {
        offset = (i+1) * node_size - self->size;

        for (j = offset; j < offset + node_size; ++j)
          canvas[j] = '*';
      }
    }
  }
  canvas[self->size] = '\0';
  puts(canvas);
}
#endif

