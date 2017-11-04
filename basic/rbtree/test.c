


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
    printf("test insert 100 nums data\n");

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
            printf("save1->key:%ld\n",save1->key);
        }
        if(i == 80){
            save2 = node;
            printf("save2->key:%ld\n",save2->key);

        }

    }
    
    rbtree_LDR(&test_tree,NULL);

    printf("test del and balance\n");
    save1 = NULL;
    for(i = 0,k = 3 ; i < 40; i++ , k+= 2){

        save1 = rbtree_find(&test_tree,k);
        if(save1)
            printf("find node:%p,k:%d save1->left:%p,save1->right:%p i:%d\n",save1,k,save1->left,
                save1->right,i);
        if(save1 != NULL){
            rbtree_delete(&test_tree,save1);
        
        }
        save1 = NULL;

    }

    printf("left key:\n");
    rbtree_LDR_recursive(test_tree.root->left,test_tree.sentinel);
    printf("right key:\n");
    rbtree_LDR_recursive(test_tree.root->right,test_tree.sentinel);



     //tralve
     
  
    //insert
    printf("test insert \n");
    node = (rbtree_node_t *)malloc(sizeof(rbtree_node_t));
        if(node == NULL){
            printf("malloc node fail \n");
            return -1;
    }
    memset(node,0x0,sizeof(rbtree_node_t));
    node->key = 6;
    rbtree_insert(&test_tree,node);
    
    
    save1 = rbtree_find(&test_tree,node->key);
    
    if(save1 != node){
        printf("insert fail\n");
    }else 
        printf("insert good !\n");


       
    //del
    printf("test del single \n");
    printf("del key:%ld befor tralve :\n",save2->key); 
    rbtree_LDR(&test_tree,NULL);
    rbtree_delete(&test_tree,save2);
    save2 = save1 = NULL;
    //tralve
    printf("deled tralve:\n");
    rbtree_LDR(&test_tree,NULL);

    //find
    printf("test use key find node and del:\n");
    printf("find key1:137,key:157\n");
    save1 = rbtree_find(&test_tree, 137);
    if(save1)
        printf("find key = 11,node:%p\n",save1);
    save2 = rbtree_find(&test_tree, 157);
    if(save2)
        printf("find key = 21 ,node:%p\n",save2);
    //del
    if(save1)
        rbtree_delete(&test_tree, save1);
    if(save1)
        rbtree_delete(&test_tree,save2);
    //tralve
    printf("deled tralve \n");
    rbtree_LDR(&test_tree,NULL);
    
    printf("test tree balance,del 50 nums data\n");
    save1 = NULL;
    for(i = 0,k = 83 ; i < 50; i++ , k+= 2){

        save1 = rbtree_find(&test_tree,k);
        if(save1)
            printf("find node :%p ,k:%d save1->left:%p,save1->right:%p\n",save1,k,save1->left,
                save1->right);
        if(save1 != NULL){
            rbtree_delete(&test_tree,save1);
        
        }
        save1 = NULL;

    }
    printf("left key:\n");
    rbtree_LDR_recursive(test_tree.root->left,test_tree.sentinel);
    printf("right key:\n");
    rbtree_LDR_recursive(test_tree.root->right,test_tree.sentinel);

    
    return 0;

    
    
    

    
    



}

