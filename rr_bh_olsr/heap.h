/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/
#ifndef _HEAP_
#define _HEAP_
#include "heap_olsr.h"


typedef struct heap_root heap;
typedef struct heap_node heap_node;

typedef struct vtx_node{
    int cost, key;                                                      // cost = custo mínimo no caminho mínimo | key = nome do vértice
    char mark;
    int pi;
} vtx_node;  



heap * heap_new();
heap_node * heap_extract(heap * queue);
void heap_update(heap_node * node, int new_cost, heap * queue);
int heap_insert(heap_node * node_to_insert, heap * queue);       // Retorna 0 se o nó foi inserido com sucesso no heap e 1 caso contrário
void heap_print(heap * queue);

int cmp_key(const void *p1, const void *p2);

int heap_is_added(heap_node * node);

#define heap_is_empty(queue) \
    heap_is_empty(queue)

#endif
