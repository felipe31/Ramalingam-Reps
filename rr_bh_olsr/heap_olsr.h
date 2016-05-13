
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

#ifndef _OLSRV2_HEAP_H
#define _OLSRV2_HEAP_H

#include "common_types.h"

/**
 * Element included into a heap_root.
 */
struct heap_node {
    /**
     * node's key based on the link cost type.
     */
    const void *key;

    /**
     * Pointer to parent node in the heap_root, NULL if root node.
     */
    struct heap_node *parent;

    /**
     * Pointer to left child, NULL if there isn't.
     */
    struct heap_node *left;

    /**
     * Pointer to right child, NULL if there isn't.
     */
    struct heap_node *right;
};

/**
 * Manager struct of the heap_root.
 * One of them is necessary for each heap_root.
 */
struct heap_root {
    /**
     * Number of nodes in the heap_root.
     */
    unsigned int count;

    /**
     * Pointer to the root node of the heap_root, NULL if heap_root is empty.
     */
    struct heap_node *root_node;

    /**
     * Pointer to the rightest node of the lowest level in the heap_root,
     * NULL if heap_root is empty.
     */
    struct heap_node *last_node;

    /**
     * Prototype for avl comparators
     * @param k1 first key
     * @param k2 second key
     * @return +1 if k1>k2, -1 if k1<k2, 0 if k1==k2
     */
    int (*comp)(const void *k1, const void *k2);
};

EXPORT void heap_init(struct heap_root *root,
    int (*comp) (const void *k1, const void *k2));
EXPORT void heap_decrease_key(struct heap_root *root, struct heap_node *node);
EXPORT void _heap_insert(struct heap_root *root, struct heap_node *node);
EXPORT struct heap_node *heap_extract_min(struct heap_root *root);
EXPORT bool heap_is_node_added(struct heap_root *root, struct heap_node *node);

/**
 * @param root pointer to binary heap_root
 * @return size of heap_root, 0 if is empty
 */
static INLINE unsigned int
heap_get_size(struct heap_root *root)
{
    return root->count;
}

/**
 * @param root pointer to binary heap_root
 * @return pointer to root node
 */
static INLINE struct heap_node*
heap_get_root_node(struct heap_root *root)
{
    return root->root_node;
}

/**
 * @param root pointer to binary heap_root
 * @return true if the heap_root is empty, false otherwise
 */
static INLINE bool
heap_is_empty(struct heap_root *root)
{
    return root->count == 0;
}

/**
 * Initialize a heap_root node
 * @param node pointer to the heap_root node
 */
static INLINE void
heap_init_node(struct heap_node *node)
{
    node->parent = node->left = node->right = NULL;
}

#endif /* _OLSRV2_HEAP_H */
