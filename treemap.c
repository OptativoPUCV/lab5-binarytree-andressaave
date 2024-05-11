#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) 
{
    TreeMap * treeMap = (TreeMap *)malloc(sizeof(TreeMap));
    if (treeMap == NULL) 
    {
        return NULL;
    }
    treeMap->lower_than = lower_than;
    treeMap->root = NULL;

    return treeMap;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if (tree == NULL)return;
    if (tree->root == NULL)
    {
        tree->root = createTreeNode(key, value);
        return;
    }

    TreeNode * aux = tree->root;
    while (aux != NULL)
        {
            if (is_equal(tree, key, aux->pair->key))
            {
                return;
            }
            if (tree->lower_than(key, aux->pair->key))
            {
                if (aux->left == NULL)
                {
                    TreeNode *new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->left = new;
                    tree->current = new;
                    return;
                }
                if (aux->left != NULL) aux = aux->left;
            }
            if (tree->lower_than(aux->pair->key, key))
            {
                if (aux->right == NULL)
                {
                    TreeNode *new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->right = new;
                    tree->current = new;
                    return;
                }
                if (aux->right != NULL) aux = aux->right;
            }
        }
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        if (x->left == NULL)break;
        x = x->left;
    }

    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL)
    {
        if (node->parent->left == node) node->parent->left = NULL;
        if (node->parent->right == node) node->parent->right = NULL;
    }
    else if (node->left == NULL || node->right == NULL)
    {
        if (node->left != NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->left;
                node->left->parent = node->parent;
            }
            if (node->parent->right == node)
            {
                node->parent->right = node->left;
                node->left->parent = node->parent;
            }
        }
        else if (node->right != NULL)
        {
            if (node->parent->left == node)
            {
                node->parent->left = node->right;
                node->right->parent = node->parent;
            }
            if (node->parent->right == node)
            {
                node->parent->right = node->right;
                node->right->parent = node->parent;
            }
        }
    }
    else if (node->left != NULL && node->right != NULL)
    {
        TreeNode *aux = minimum(node->right);
        node->pair->key = aux->pair->key;
        node->pair->value = aux->pair->value;
        removeNode(tree, aux);
    }

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;
    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *node = tree->root;
    while (node != NULL)
        {
            if (is_equal(tree, node->pair->key, key))
            {
                tree->current = node;
                return node->pair;
            }
            if (tree->lower_than(node->pair->key, key))
                node = node->right;
            else
                node = node->left;
        }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *node = NULL;
    TreeNode *aux = tree->root;

    while (aux != NULL)
        {
            if (is_equal(tree, aux->pair->key, key))
            {
                tree->current = aux;
                return aux->pair;
            }
            else if (tree->lower_than(aux->pair->key, key))
            {
                aux = aux->right;
            }
            else
            {
                node = aux;
                aux = aux->left;
            }
        }
    if (node != NULL)
    {
        tree->current = node;
        return node->pair;
    }
    else return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *node = tree->root;
    while (node->left != NULL)
        {
            node = node->left;
        }
    tree->current = node;
    return node->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode *node = tree->current;
    if (node->right != NULL)
    {
        node = node->right;
        while (node->left != NULL)
            node = node->left;
        tree->current = node;
        return node->pair;
    }
    else
    {
        TreeNode *aux = node->parent;
        while (aux != NULL && node == aux->right)
        {
            node = aux;
            aux = aux->parent;
        }
        tree->current = aux;
        if (aux != NULL) return aux->pair;
        else return NULL;
    }
}