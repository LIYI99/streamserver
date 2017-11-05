#ifndef __RB_TREE_H__
#define __RB_TREE_H__ 


#include <stdint.h>



#ifdef __cplusplus 
    extern  "C"{
#endif 

typedef intptr_t   rbtree_key_t;;
typedef uintptr_t    rbtree_key_uint_t;
typedef uint8_t     u_char;


typedef struct rbtree_node_s rbtree_node_t;

struct rbtree_node_s {
    
    rbtree_key_t        key;  //key or data 
    rbtree_node_t       *left;
    rbtree_node_t       *right;
    rbtree_node_t       *parent;
    u_char              color;
    rbtree_key_uint_t   size;   //mem manger       
    void*               data;   //slave
};


typedef struct  rbtree_s    rbtree_t;

typedef void (*rbtree_insert_pt) (rbtree_node_t *root,
    rbtree_node_t *node, rbtree_node_t *sentinel);

typedef void (*rbtree_display_pt)(void *data);



struct rbtree_s {
    rbtree_node_t       *root;
    rbtree_node_t       *sentinel;
    rbtree_insert_pt    insert;

};

#define rbtree_init(tree,s,i)               \
    rbtree_sentinel_init(s);;               \
    (tree)->root = s;                       \
    (tree)->sentinel    = s;                \
    (tree)->insert = i;                     

void    rbtree_insert(rbtree_t *tree, rbtree_node_t *node);
void    rbtree_delete(rbtree_t *tree, rbtree_node_t *node);
rbtree_node_t*    rbtree_find(rbtree_t *tree, rbtree_key_t key);
void    rbtree_insert_value(rbtree_node_t *root,rbtree_node_t *node,rbtree_node_t *sentinel);
void    rbtree_insert_timer_value(rbtree_node_t *root,rbtree_node_t *node,rbtree_node_t *sentinel);

void    rbtree_LDR(rbtree_t *tree,  rbtree_display_pt  display);

void    rbtree_LDR_recursive(rbtree_node_t *node,rbtree_node_t *sentinel);
void*    rbtree_LDR_MEM(rbtree_t *tree,int size);


#define rbt_red(node)               ((node)->color = 1)
#define rbt_black(node)             ((node)->color = 0)
#define rbt_is_red(node)            ((node)->color)
#define rbt_is_black(node)          (!rbt_is_red(node))
#define rbt_copy_color(n1,n2)       (n1->color = n2->color)


#define rbtree_sentinel_init(node)   rbt_black(node)


static inline  rbtree_node_t *
    rbtree_min(rbtree_node_t *node, rbtree_node_t *sentinel)
{
    
    while (node->left != sentinel) {
        node = node->left;
    }

    return node;

    
}




#ifdef __cplusplus 
}
#endif 


#endif 



