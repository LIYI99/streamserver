


#include<stdio.h>
#include<string.h>
#include "rbtree.h"



static inline void  rbtree_left_rotate(rbtree_node_t **root,
        rbtree_node_t *sentinel, rbtree_node_t *node);
static inline void  rbtree_right_rotate(rbtree_node_t **root,
        rbtree_node_t *sentinel, rbtree_node_t *node);



void rbtree_insert(rbtree_t *tree ,rbtree_node_t *node)
{
    rbtree_node_t **root,*temp,*sentinel;

    root = (rbtree_node_t **) &tree->root;
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


void rbtree_delete_(rbtree_t *tree,rbtree_node_t *node){

    
    rbtree_node_t **root,*sentinel,*subst,*temp,*w;
    rbtree_key_uint_t red;

    root = &tree->root;

    if(node->left == sentinel){
        temp = node->right;
        subst = node;
    }else if(node->right == sentinel){
        temp= node->left;
        subst = node;
    }else{
        
        subst = rbtree_min(node->right,sentinel);
        
        if(subst->left != sentinel){
            temp = subst->left;
        }else{
            temp = subst->right;
        }
    }

    if(subst == *root){
        *root = temp;
        rbt_black(temp);
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        node->key = 0;

    }
    
    red = rbt_is_red(subst);

    if(subst = subst->parent->left)
    {
        subst->parent->left = temp; 
    }else{
        subst->parent->right = temp;

    }
   



}


void    rbtree_delete(rbtree_t *tree, rbtree_node_t *node)
{
    
    rbtree_key_uint_t   red;
    rbtree_node_t      **root,*sentinel,*subst,*temp,*w;

    root = (rbtree_node_t** ) &tree->root;
    sentinel = tree->sentinel;
    
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

        if(subst->left != sentinel){
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
    //printf("red:%ld temp:%p,temp->color:%d\n",red,temp,temp->color);
    if(red){
        return ;
    }

    while(temp != *root && rbt_is_black(temp)){
        
        if(temp == temp->parent->left){
            
       //printf("L:red:%ld temp:%p,temp->color:%d temp->parent:%p\n",red,temp,temp->color,temp->parent);

            w = temp->parent->right;
            if(rbt_is_red(w)){
                rbt_black(w);
                rbt_red(temp->parent);
                rbtree_left_rotate(root,sentinel,temp->parent);
                w = temp->parent->right;
            }
            
            if(rbt_is_black(w->left) && rbt_is_black(w->right)){
                rbt_red(w);
                temp = temp->parent;

            }else{
            
                if(rbt_is_black(w->right)){
                    rbt_black(w->left);
                    rbt_red(w);
                    rbtree_right_rotate(root,sentinel,w);
                    w = temp->parent->right;
                }

                rbt_copy_color(w,temp->parent);
                rbt_black(temp->parent);
                rbt_black(w->right);
                rbtree_left_rotate(root,sentinel,temp->parent);
                temp = *root; 
            
            }
        
        }else{
          //printf(" R:red:%ld temp:%p,temp->color:%d\n",red,temp,temp->color);

            w = temp->parent->left;
            if(rbt_is_red(w)){
                rbt_black(w);
                rbt_red(temp->parent);
                rbtree_right_rotate(root,sentinel,temp->parent);
                w = temp->parent->left;
            }
        
            if(rbt_is_black(w->left) && rbt_is_black(w->right)){
                rbt_red(w);
                temp =  temp->parent;
            }else{
            
                if(rbt_is_black(w->left)){
                    rbt_black(w->right);
                    rbt_red(w);
                    rbtree_left_rotate(root,sentinel,w);
                    w = temp->parent->left;
                }

                rbt_copy_color(w,temp->parent);
                rbt_black(temp->parent);
                rbt_black(w->left);
                rbtree_right_rotate(root,sentinel,temp->parent);

                temp = *root;
            
            }

        }
    }
    rbt_black(temp);

}





static inline void rbtree_left_rotate(rbtree_node_t **root,rbtree_node_t* sentinel,rbtree_node_t *node){
    
  
        rbtree_node_t *temp  = NULL;
        
        
        temp = node->right;

        if((node->right = temp->left) != sentinel)
            temp->left->parent = node;
        temp->parent = node->parent;

        if(node  == *root)
            *root = temp;
        else if(node == node->parent->right)
            node->parent->right = temp;
        else
            node->parent->left = temp;
        node->parent = temp;
        temp->left = node;
}

static inline void rbtree_right_rotate(rbtree_node_t **root,rbtree_node_t* sentinel,rbtree_node_t *node)
{
    
    rbtree_node_t *temp;
    temp =  node->left;
    node->left  = temp->right;

    if(temp->right != sentinel){
        temp->right->parent = node;
    }

    temp->parent = node->parent;
    if(node == *root){
        *root = temp;
    }else if(node == node->parent->right){
        node->parent->right = temp;
    }else{
        node->parent->left = temp;
    }

    temp->right = node;
    node->parent = temp;
    
}


void    rbtree_insert_value(rbtree_node_t *temp,rbtree_node_t *node,rbtree_node_t *sentinel)
{
    rbtree_node_t **p;
    for(;;){
        p =  (node->key < temp->key) ? &temp->left : &temp->right;
        if(*p == sentinel){
            break;
        }
        
        temp = *p;

    }
    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    rbt_red(node);
    
}

void    rbtree_insert_timer_value(rbtree_node_t *temp,rbtree_node_t *node,rbtree_node_t *sentinel)
{
    rbtree_node_t **p;
    for(;;){

        p =  ((rbtree_key_t)(node->key -temp->key) < 0) ? &temp->left : &temp->right;
        if(*p == sentinel){
            break;
        }

        temp = *p;
    }
    *p = node;
    node->parent = temp;
    node->left = sentinel;
    node->right = sentinel;
    rbt_red(node);

}




rbtree_node_t*    rbtree_find(rbtree_t *tree, rbtree_key_t key)
{
    
    
    rbtree_node_t **p,*temp,*node ,*sentinel;
    sentinel = tree->sentinel;

    temp  = tree->root;
    for(;;){
        if(key == temp->key){
            *p = temp;
            break;
        }

        p =  (key < temp->key) ? &temp->left : &temp->right;
        if(*p == sentinel){
            break;
        }
        
        temp = *p;
    }
    if(*p != sentinel)
        return *p;
    else
        return NULL;
}

#define     S_MAX       1000

struct rbtree_stack{
        
        rbtree_node_t *data[1000];
        rbtree_key_t   top;

};

static void  strack_init(struct rbtree_stack* s)  
{  
    s->top = -1; 
}  
  
static inline rbtree_key_t stack_empty(struct rbtree_stack* s)  
{  
    if(s->top == -1)  
    {  
        return 1;  
    }  
    else  
    {  
        return 0;  
    }  
}  
  
static inline rbtree_key_t  stack_full(struct rbtree_stack* s)
{  
    if(s->top == S_MAX - 1)  
    {  
        return 1;  
    }  
    else  
    {  
        return 0;  
    }  
}  
  
static void stack_push(struct rbtree_stack* s,rbtree_node_t **node)  
{  
    
    if(!stack_full(s))  
    {  
        s->top++;  
        (s->data)[s->top] = *node; 

    }  
    else  
    {  
        printf("stack full\n");  
    }  
}    

static  rbtree_node_t* stack_pop(struct rbtree_stack* s)  
{  
    rbtree_node_t *node = NULL;
    if(!stack_empty(s))  
    {  
        node = (s->data)[s->top];
        s->top--;  

    }  
    else  
    {  
        printf("Empty\n");  
    }  
    
    return node;

    
}  

void    rbtree_LDR_recursive(rbtree_node_t *node,rbtree_node_t *sentinel)
{   
        if(node != sentinel){
            rbtree_LDR_recursive(node->left,sentinel);
            printf("key:%ld\n",node->key);
            rbtree_LDR_recursive(node->right,sentinel);
        }
    
}



void    rbtree_LDR(rbtree_t *tree,  rbtree_display_pt  display)
{
    
    rbtree_node_t *temp,*sentinel;
    sentinel = tree->sentinel;
    temp = tree->root;
    
    //rbtree_LDR_recursive(temp ,sentinel);
    //return;

    struct rbtree_stack S;
    S.top = -1;
    //stack_push(&S,&temp);
 

    while(temp != sentinel || !(stack_empty(&S)))
    {
            while(temp != sentinel){
                 if(temp != sentinel){
                    stack_push(&S,&temp);
                }

                temp = temp->left;
        }
            
            if(!stack_empty(&S)){
                temp = stack_pop(&S);
                printf("temp:%prbtree->key: %ld\n",temp,temp->key);
                temp = temp->right;
            }
    }
}



