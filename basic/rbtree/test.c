


#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>




int main(int argc ,char **argv)

{

    

    rbtree_t        test_tree;
    rbtree_node_t   sentinel;
    
    memset(&sentinel,0x0,sizeof(rbtree_node_t));

    //init tree 
    rbtree_init(&test_tree,&sentinel,rbtree_insert_value);
    printf("sentinel:%p\n",&sentinel);

    //insert
    int k ,i;
    rbtree_node_t *node = NULL,*save1 = NULL,*save2 = NULL;
    for(i = 0,k = 3 ; i <  100 ; i++,k+=2){
        
        node = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
        if(node == NULL){
            printf("malloc node fail \n");
            return -1;
        }

        memset(node,0x0,sizeof(rbtree_node_t));
        node->key = k;
        rbtree_insert(&test_tree,node);
        if(i == 30){
            save1 = node;
            printf("save1->key:%d\n",save1->key);
        }
        if(i == 80){
            save2 = node;
            printf("save2->key:%d\n",save2->key);

        }

    }
    printf("i:%d,k:%d \n",i,k);
    //tralve
  
    rbtree_LDR(&test_tree,NULL);
    
    printf("LINE:%d\n",__LINE__);

    //insert
    node = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
        if(node == NULL){
            printf("malloc node fail \n");
            return -1;
    }
    memset(node,0x0,sizeof(rbtree_node_t));
    node->key = 6;
    rbtree_insert(&test_tree,node);
    
    //tralve
    rbtree_LDR(&test_tree,NULL);
    
    //del
    rbtree_delete(&test_tree, save1);
    rbtree_delete(&test_tree,save2);
    save2 = save1 = NULL;
    //tralve
    
    rbtree_LDR(&test_tree,NULL);

    //find
    
    save1 = rbtree_find(&test_tree, 11);
    if(save1)
        printf("find key = 11,node:%p\n",save1);
    save2 = rbtree_find(&test_tree, 21);
    if(save2)
        printf("find key = 21 ,node:%p\n",save2);

    
    //del
    rbtree_delete(&test_tree, save1);
    rbtree_delete(&test_tree,save2);

        
    //tralve
    rbtree_LDR(&test_tree,NULL);

    
    return 0;

    
    
    

    
    



}

