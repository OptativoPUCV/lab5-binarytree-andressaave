#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lt) (void* key1, void* key2)) {
    TreeMap *map = (TreeMap *)malloc(sizeof(TreeMap));
    if (map == NULL) {
        // Manejar error de asignación de memoria
        return NULL;
    }

    map->root = NULL;
    map->current = NULL;
    map->lower_than = lt;

    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    
    if (searchTreeMap(tree, key) != NULL) {
        
        return;
    }

    
    TreeNode *new_node = createTreeNode(key, value);
    if (new_node == NULL) {
        
        return;
    }

    
    if (tree->root == NULL) {
        tree->root = new_node;
        tree->current = new_node; 
        return;
    }

    
    TreeNode *current_node = tree->root;
    while (current_node != NULL) {
        int cmp = tree->lower_than(key, current_node->pair->key);
        if (cmp < 0) {
            
            if (current_node->left == NULL) {
                current_node->left = new_node;
                new_node->parent = current_node;
                tree->current = new_node; 
                return;
            } else {
                current_node = current_node->left;
            }
        } else {
            
            if (current_node->right == NULL) {
                current_node->right = new_node;
                new_node->parent = current_node;
                tree->current = new_node; 
                
                return;
            } else {
                current_node = current_node->right;
            }
        }
    }
}

TreeNode * minimum(TreeNode * x) {
    if (x == NULL) {
        
        return NULL;
    }

    
    while (x->left != NULL) {
        x = x->left;
    }

    
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if (node == NULL) {
    
    return;
  }

  
  if (node->left == NULL && node->right == NULL) {
    
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
    } else {
        
        tree->root = NULL;
    }
    free(node->pair); 
    free(node); 
    return;
  }

  
  if (node->left == NULL || node->right == NULL) {
    TreeNode *child = (node->left != NULL) ? node->left : node->right;
    
    if (node->parent != NULL) {
        if (node->parent->left == node) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }
        child->parent = node->parent; 
    } else {
        
        tree->root = child;
        child->parent = NULL; 
    }
    free(node->pair); 
    free(node); 
    return;
  }

  
  TreeNode *successor = minimum(node->right); 
  node->pair = successor->pair;

  
  removeNode(tree, successor);

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair* searchTreeMap(TreeMap* tree, void* key) {
    if (tree == NULL || tree->root == NULL) {
        
        tree->current = NULL;
        return NULL;
    }

    TreeNode *current_node = tree->root;
    while (current_node != NULL) {
        int cmp = tree->lower_than(key, current_node->pair->key);
        if (cmp == 0) {
            
            tree->current = current_node;
            return current_node->pair;
        } else if (cmp < 0) {
            current_node = current_node->left; 
        } else {
            current_node = current_node->right; 
        }
    }

    
    tree->current = NULL;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  if (tree == NULL || tree->root == NULL) {
      
      return NULL;
  }

  TreeNode *current_node = tree->root;
  TreeNode *ub_node = NULL; 

  while (current_node != NULL) {
      int cmp = tree->lower_than(key, current_node->pair->key);
      if (cmp == 0) {
         
          return current_node->pair;
      } else if (cmp < 0) {
          
          ub_node = current_node;
          current_node = current_node->left;
      } else {
          
          current_node = current_node->right;
      }
  }

  
  if (ub_node != NULL) {
      return ub_node->pair;
  } else {
      
      return NULL;
  }
}

Pair * firstTreeMap(TreeMap * tree) {
    
    TreeNode *min_node = minimum(tree->root);

    if (min_node != NULL) {
       
        return min_node->pair;
    } else {
        
        return NULL;
    }
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree->current == NULL) {
        
        return NULL;
    }

    TreeNode *current_node = tree->current;

    
    if (current_node->right != NULL) {
        tree->current = minimum(current_node->right);
        return tree->current->pair;
    }

    
    TreeNode *parent_node = current_node->parent;
    while (parent_node != NULL && current_node == parent_node->right) {
        current_node = parent_node;
        parent_node = parent_node->parent;
    }

    
    if (parent_node == NULL) {
        tree->current = NULL;
        return NULL;
    } else {
        
        tree->current = parent_node;
        return parent_node->pair;
    }
}
