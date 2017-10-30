



#include "rbtree.h"



static inline void  rbtree_left_rotate(rbtree_node_t **root,
    rbtree_node_t *sentinel, rbtree_node_t *node);
static inline void  rbtree_right_rotate(rbtree_node_t **root,
    rbtree_node_t *sentinel, rbtree_node_t *node);






void    rbtree_insert(rbtree_t *tree, rbtree_node_t *node){

     
    rbtree_node_t  **root, *temp, *sentinel;
    root = (rbtree_node_t **) &tree->root;
    sentinel = tree->sentinel;
    
    //first insert 
    if (*root == sentinel) {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        ngx_rbt_black(node);
        *root = node;
        return;
    }
    //insert node 
    tree->insert(*root,node,sentinel);
    
    //re-balance tree 
    while (node != *root && rbt_is_red(node->parent)) {

        if (node->parent == node->parent->parent->left) {
            temp = node->parent->parent->right; //get uncle node 
            
            /* if uncle node is read ,need  set uncle node color
               and set Grandpa node balck ,and get parent node use next check
            */ 
            if (ngx_rbt_is_red(temp)) {                  
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
            /*  
             * now need left roate ,and let right_rotate 
             * */
                if (node == node->parent->right) {
                    node = node->parent;
                    ngx_rbtree_left_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_right_rotate(root, sentinel, node->parent->parent);
            }

        } else {
            temp = node->parent->parent->left;

            if (ngx_rbt_is_red(temp)) {
                ngx_rbt_black(node->parent);
                ngx_rbt_black(temp);
                ngx_rbt_red(node->parent->parent);
                node = node->parent->parent;

            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    ngx_rbtree_right_rotate(root, sentinel, node);
                }

                ngx_rbt_black(node->parent);
                ngx_rbt_red(node->parent->parent);
                ngx_rbtree_left_rotate(root, sentinel, node->parent->parent);
            }
        }
    }

    rbt_black(*root);

}


void    rbtree_delete(rbtree_t *tree, rbtree_node_t *node);
rbtree_node_t*    rbtree_find(rbtree_t *tree, rbtree_key_t key);
void    rbtree_insert_value(rbtree_node_t *root,rbtree_node_t *node,rbtree_node_t *sentinel);
void    rbtree_insert_timer_value(rbtree_node_t *root,rbtree_node_t *node,rbtree_node_t *sentinel);

void    rbtree_LRD(rbtree_t *tree,  rbtree_display_pt  display);


