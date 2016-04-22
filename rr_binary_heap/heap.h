/************************************************************************************************
*************************************************************************************************
******************************* ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/
#ifndef _HEAP_
#define _HEAP_
#define HEAP_SIZE 30000
#include "rr.h"

heap * heap_new();
heap_node * heap_extract(heap * queue);
int heap_checks_presence(heap_node * heap_node, heap * queue);               // Retorna 0 se o nó não pertence ao heap, -1 caso haja erros e retorna a posição caso o nó pertença ao heap
void heap_update(vertex * vtx, int new_pi, int new_cost, heap * queue);
int heap_insert(heap_node * node_to_insert, heap * queue);                   // Retorna 0 se o nó foi inserido com sucesso no heap e 1 caso contrário
void heapfy(heap * queue, int i);
void heap_build(heap * queue);
void heap_print(heap * queue);


#define heap_is_added(node) \
    ((vtx_node*)((heap_node*)node)->key)->mark

#define heap_is_empty(queue) \
    queue->control == 0? 1 : 0

#endif
