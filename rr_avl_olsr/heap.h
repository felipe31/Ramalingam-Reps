/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/
#ifndef _HEAP_
#define _HEAP_
#include "avl.h"
#include "rr.h"

heap * heap_new();
heap_node * heap_extract(heap * queue);
void heap_update(vertex * vtx, int new_pi, int new_cost, heap * queue);
int heap_insert(heap_node * node_to_insert, heap * queue);       // Retorna 0 se o nó foi inserido com sucesso no heap e 1 caso contrário
void heap_print(heap * queue);

int cmp_key(const void *p1, const void *p2);

#define heap_is_added(node) \
    avl_is_node_added(node)

#define heap_is_empty(queue) \
    avl_is_empty(queue)

#endif
