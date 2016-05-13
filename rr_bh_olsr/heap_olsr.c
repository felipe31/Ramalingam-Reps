
/*
 * The olsr.org Optimized Link-State Routing daemon version 2 (olsrd2)
 * Copyright (c) 2004-2015, the olsr.org team - see HISTORY file
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 */

#include <string.h>

#include "common_types.h"
#include "heap.h"

static unsigned int heap_perfect_log2 (unsigned int number);
static struct heap_node* heap_find_parent_insert_node(struct heap_root *root);
static void heap_swap_left(struct heap_root *root, struct heap_node *node);
static void heap_swap_right(struct heap_root *root, struct heap_node *node);
static void heap_increase_key(struct heap_root *root, struct heap_node *node);
static struct heap_node* heap_find_last_node(struct heap_root *root);

/**
 * Initialize a new heap_root struct
 * @param root pointer to binary heap_root
 */
void
heap_init(struct heap_root *root, int (*comp) (const void *k1, const void *k2))
{
  memset(root, 0, sizeof(*root));
  root->comp = comp;
}

/**
 * @param node pointer to node of the heap_root
 * @return true if node is currently in the heap_root, false otherwise
 */
bool
heap_is_node_added(struct heap_root *root, struct heap_node *node)
{
  if(node){
    if(node->parent || node->left || node->right)
      return true;
    if(node == heap_get_root_node(root))
      return true;
  }
  return false;
}

/**
 * test if the last binary heap_root's level is full
 * @param number of elements in the heap_root
 * @return the difference between the binary heap_root's size and a full binary heap_root with the same height.
 */
static unsigned int
heap_perfect_log2 (unsigned int number)
{
    int log = 0, original_number=number;
    while (number >>= 1) ++log;
    return original_number - (1 << log);
}

/**
 * finds the parent node of the new node that will be inserted in the binary heap_root
 * @param root pointer to binary heap_root
 * @return the pointer to parent node
 */

static struct heap_node*
heap_find_parent_insert_node(struct heap_root *root)
{
    struct heap_node *aux = root->last_node;
    unsigned int N = root->count+1;
    if ( !heap_perfect_log2(N) ){
        aux = root->root_node;
        while (aux->left) {
            aux = aux->left;
        }
    }
    else if ( N % 2 == 0){
        while(aux->parent->right == aux)
            aux = aux->parent;
        if(!aux->parent->right)
            return aux->parent;
        aux = aux->parent->right;
        while(aux->left)
            aux = aux->left;
    }
    else{
        aux = aux->parent;
    }
    return aux;
}


/**
 * updates the heap_root after node's key value be changed to a better value
 * @param root pointer to binary heap_root
 * @param node pointer to the node changed
 */
void
heap_decrease_key(struct heap_root *root, struct heap_node *node)
{
    struct heap_node *parent = node->parent;
    struct heap_node *left = node->left;
    struct heap_node *right = node->right;
    if(!parent)
        return;
    if(root->comp(parent->key,node->key) > 0){
        if(root->last_node == node)
            root->last_node = parent;
    }
    while(parent && root->comp(parent->key, node->key) > 0){
        if(parent->left == node){
            node->left = parent;
            node->right = parent->right;
            if(node->right)
                node->right->parent = node;
            node->parent = parent->parent;
            if(node->parent){
                if(node->parent->left == parent)
                    node->parent->left = node;
                else
                    node->parent->right = node;
            }
            else
                root->root_node = node;
        }
        else{
            node->right = parent;
            node->left = parent->left;
            if(node->left)
                node->left->parent = node;
            node->parent = parent->parent;
            if(node->parent){
                if(node->parent->left == parent)
                    node->parent->left = node;
                else
                    node->parent->right = node;
            }
            else
                root->root_node = node;
        }
        parent->left = left;
        parent->right = right;
        parent->parent = node;
        if(left)
            left->parent = parent;
        if(right)
            right->parent = parent;
        parent = node->parent;
        left = node->left;
        right = node->right;
    }
}

/**
 * inserts the node in the binary heap_root
 * @param root pointer to binary heap_root
 * @param node pointer to node that will be inserted
 */
void
_heap_insert(struct heap_root *root, struct heap_node *node)
{
    struct heap_node *parent = NULL;

    heap_init_node(node);

    if(!root->count){
        root->root_node = root->last_node = node;
        root->count++;
    }
    else{
        parent = heap_find_parent_insert_node(root);
        if(parent->left){
            parent->right = node;
        }
        else{
            parent->left = node;
        }
        node->parent = parent;
        root->count++;
        root->last_node = node;
        heap_decrease_key(root, node);
    }
}

/**
 * swaps the node with its left child
 * @param root pointer to binary heap_root
 * @param node pointer to node that will be swapped
 */
static void
heap_swap_left(struct heap_root *root, struct heap_node *node)
{
    struct heap_node *parent = node->parent;
    struct heap_node *left = node->left;
    struct heap_node *right = node->right;

    node->parent = left;
    node->left = left->left;
    if(node->left)
        node->left->parent = node;
    node->right = left->right;
    if(node->right)
        node->right->parent = node;

    left->parent = parent;
    if(parent){
        if(parent->left == node)
            parent->left = left;
        else
            parent->right = left;
    }
    else
        root->root_node = left;
    left->left = node;
    left->right = right;
    if(right)
        right->parent = left;
    if(root->last_node == left)
        root->last_node = node;
}

/**
 * swaps the node with its right child
 * @param root pointer to binary heap_root
 * @param node pointer to node that will be swapped
 */
static void
heap_swap_right(struct heap_root *root, struct heap_node *node)
{
    struct heap_node *parent = node->parent;
    struct heap_node *left = node->left;
    struct heap_node *right = node->right;

    node->parent = right;
    node->left = right->left;
    if(node->left)
        node->left->parent = node;
    node->right = right->right;
    if(node->right)
        node->right->parent = node;

    right->parent = parent;
    if(parent){
        if(parent->left == node)
            parent->left = right;
        else
            parent->right = right;
    }
    else
        root->root_node = right;
    right->right = node;
    right->left = left;
    if(left)
        left->parent = right;
    if(root->last_node == right)
        root->last_node = node;
}

/**
 * updates the heap_root after node's key value be changed to a worse value
 * @param root pointer to binary heap_root
 * @param node pointer to the node changed
 */
static void
heap_increase_key(struct heap_root *root, struct heap_node *node)
{
    struct heap_node *left = node->left;
    struct heap_node *right = node->right;
    if(left && root->comp(node->key, left->key) > 0){
        if(right && root->comp(node->key, right->key) > 0){
            if(root->comp(left->key, right->key) < 0) {
                heap_swap_left(root, node);
            }
            else {
                heap_swap_right(root, node);
            }
        }
        else
            heap_swap_left(root, node);
        heap_increase_key(root, node);
    }
    else if(right && root->comp(node->key, right->key) > 0){
        heap_swap_right(root, node);
        heap_increase_key(root, node);
    }
}

/**
 * finds the last node of the binary heap_root
 * @param root pointer to binary heap_root
 * @return the pointer to last node
 */
static struct heap_node*
heap_find_last_node(struct heap_root *root)
{
    struct heap_node *aux = root->last_node;
    unsigned int N = root->count+1;
    if ( !heap_perfect_log2(N) ){
        aux = root->root_node;
        while (aux->right) {
            aux = aux->right;
        }
    }
    else if ( N % 2 == 0){
        while(aux->parent->left == aux)
            aux = aux->parent;
        aux = aux->parent->left;
        while(aux->right)
            aux = aux->right;
    }
    return aux;
}

/**
 * extracts the best node from binary heap_root
 * @param root pointer to binary heap_root
 * @return the pointer to best node
 */
struct heap_node *
heap_extract_min(struct heap_root *root)
{
    struct heap_node *min_node = root->root_node;
    struct heap_node *new_min = root->last_node;
    if(!min_node)
        return NULL;
    root->count--;
    if(root->count == 0){
        root->last_node = root->root_node = NULL;
    }
    else if(root->count == 1){
        root->last_node = root->root_node = new_min;
        new_min->parent = NULL;
    }
    else{
        if(new_min->parent->left == new_min){
            new_min->parent->left = NULL;
            root->last_node = new_min->parent;
            root->last_node = heap_find_last_node(root);
        }
        else{
            new_min->parent->right = NULL;
            root->last_node = new_min->parent->left;
        }
        new_min->left = min_node->left;
        if(new_min->left)
            new_min->left->parent = new_min;
        new_min->right = min_node->right;
        if (new_min->right)
            new_min->right->parent = new_min;
        new_min->parent = NULL;
        root->root_node = new_min;
        heap_increase_key(root, new_min);
    }
    heap_init_node(min_node);
    return min_node;
}
