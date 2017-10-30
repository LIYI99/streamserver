

#include "rbtree.h"



static inline void  rbtree_left_rotate(rbtree_node_t **root,
        rbtree_node_t *sentinel, rbtree_node_t *node);
static inline void  rbtree_right_rotate(rbtree_node_t **root,
        rbtree_node_t *sentinel, rbtree_node_t *node);



void rbtree_insert_(rbtree_t *tree ,rbtree_node_t *node)
{
    rbtree_t_node_t **root,*temp,*sentinel;

    root = (rbtree_t_node_t **) &tree->root;
    sentinel = tree->sentinel;
    //fisrt insert 
    if(*root == sentinel )
    {
        node->parent = NULL;
        node->left = sentinel;
        node->right = sentinel;
        rbt_black(node);
        *root= node ;
        
        return;
    }

    //insert node 
    tree->insert(*root,node,sentinel);
    
    while(node != *root && rbt_is_red(node->parent))
    {
         if(node->parent == node->parent->parent->left){
            temp = node->parent->parent->right;
            if(rbt_is_red(temp)){
                rbt_black(node->parent);
                rbt_black(temp);
                rbt_red(node->parent->parent);
                node = node->parent->parent;
            }else{
                
                if(node == node->parent->right)
                {
                    node = node->parent;
                    rbtree_left_rotate(root,sentinel,node);
                }
                rbt_black(node->parent);
                rbt_red(node->parent->parent);
                rbtree_right_rotate(root,sentinel,node->parent->parent);
            }

         }else{
            temp = node->parent->parent->left;
            if(rbt_is_red(temp)){
                rbt_black(node->parent);
                rbt_black(temp);
                rbt_red(node->parent->parent);
                node = node->parent->parent;
            }else{
                if(node == node->parent->left)
                {
                    node = node->parent;
                    rbtree_right_rotate(root,sentinel ,node);
                }
                rbt_black(node->parent);
                rbt_red(node->parent->parent);
                rbtree_left_rotate(root,sentinel,node->parent->parent);
            }
         
         }
    }
    rbt_black(*root);

}





void    rbtree_delete(rbtree_t *tree, rbtree_node_t *node)
{
    
    rbtree_key_uint_t   red;
    rbtree_t_node_t      **root,*sentinel,*subst,*temp,*w;

    root = (rbtree_node_t* ) &tree->root;
    if(node->left == sentinel){
        temp = node->right;
        subst = node;
    }else if (node->right == sentinel){
        temp = node->left;
        subst = node;
    
    }else{
        subst = rbtree_min(node->right,sentinel);
        
        if(subst->left != sentinel){
            temp = subst->left;

        } else {
            temp = subst->right;
        }
    
    }
    
    if(subst == *root){
        *root = temp;
        rbt_black(temp);

        node->left = NULL;
        node->right = NULL;
        node->parent= NULL;
        node->key = 0;
        
        return ;

    }
    
    red = rbt_is_red(subst);


    if(subst == subst->parent->left)
    {
        subst->parent->left = temp;
    }else{
        subst->parent->right = temp;
    }
    
    if(subst == node)
    {
        temp->parent = subst->parent;
    }else{

        if(subst->parent == node)
        {
            temp->parent = subst;
        }else{
            temp->parent = subst->parent;
        }
        subst->left = node->left;
        subst->right = node->right;
        subst->parent =  node->parent;
        rbt_copy_color(subst,node);

        if(node == *root){
            *root = subst;
        }else{
            if(node == node->parent->left){
                node->parent->left = subst;
            }else{
                node->parent->right = subst;
            }
        }

        if(subst->letf != sentinel){
            subst->left->parent = subst;
        }

        if(subst->right != sentinel){
            subst->right->parent = subst;
        }

    }

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->key = 0 ;

    if(red){
        return ;
    }

    while(temp != *root && rbt_is_black(temp)){
    
    
    }

}
rbtree_node_t*    rbtree_find(rbtree_t *tree, rbtree_key_t key);
void    rbtree_insert_value(rbtree_node_t *root,rbtree_node_t *node,rbtree_node_t *sentinel);
void    rbtree_insert_timer_value(rbtree_node_t *root,rbtree_node_t *node,rbtree_node_t *sentinel);

void    rbtree_LRD(rbtree_t *tree,  rbtree_display_pt  display);


