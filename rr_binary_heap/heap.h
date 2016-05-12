/************************************************************************************************
*************************************************************************************************
******************************* ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/
#ifndef _HEAP_
#define _HEAP_
#define HEAP_SIZE 30000
#include <stdio.h>
#include <stdlib.h>

typedef struct heap_node{
    void * key;
    int position;
} heap_node;

typedef struct vtx_node{
    int cost, key;                                                      // cost = custo mínimo no caminho mínimo | key = nome do vértice
    char mark;
    int pi;
} vtx_node;                                                                 // primeiramente usar um vetor

typedef struct heap{
    heap_node ** node_vector;
    int control;
} heap;



heap * heap_new();
heap_node * heap_extract(heap * queue);
int heap_checks_presence(heap_node * heap_node, heap * queue);               // Retorna 0 se o nó não pertence ao heap, -1 caso haja erros e retorna a posição caso o nó pertença ao heap
void heap_update(heap_node * node, int new_cost, heap * queue);
int heap_insert(heap_node * node_to_insert, heap * queue);                   // Retorna 0 se o nó foi inserido com sucesso no heap e 1 caso contrário
void heapfy(heap * queue, int i);
void heap_decrease_key(heap * queue, int pos);
void heap_increase_key(heap * queue, int pos);
void heap_print(heap * queue);


#define heap_is_added(node) \
    ((vtx_node*)((heap_node*)node)->key)->mark

#define heap_is_empty(queue) \
    queue->control == 0? 1 : 0

#endif
