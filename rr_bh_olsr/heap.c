/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "rr.h"

heap * ROOT;

heap * heap_new()
{
	heap * root = (struct heap_root *) calloc(1, sizeof(struct heap_root));
	ROOT = root;
	if(!root) return NULL;
	heap_init(root, cmp_key);
	return root;
}


int heap_insert(heap_node * node_to_insert, heap * queue)       // Retorna 0 se o nó foi inserido com sucesso no heap e -1 caso contrário
{
	if(!queue || !node_to_insert) return -1;
	_heap_insert(queue, node_to_insert);
	return 0;
}


heap_node * heap_extract(heap * queue)
{
	if(!queue) return NULL;
	return heap_extract_min(queue);
}


void heap_update(heap_node * node, int new_cost, heap * queue)
{
	if(!queue || !node) return;
	((vtx_node *)node->key)->cost = new_cost;
	heap_decrease_key(queue, node);
}
/*
void heap_print(heap * queue)
{
    vertex * vtx;
    if(!queue)
        printf("Heap invalido\n");
    else if(!heap_is_empty(queue))
    {
        avl_for_each_element(queue, vtx, h_node)
        {
            printf("Elemento %d:\ncost:\t%d\npi:\t%d\n\n", ((vtx_node*)vtx->h_node.key)->key, ((vtx_node*)vtx->h_node.key)->cost, vtx->pi);
        }
    }
    else
        printf("Heap vazio!\n");

    puts("\n");
}
*/
int heap_is_added(heap_node * node)
{
    heap_is_node_added(ROOT, node);
}

int
cmp_key(const void *p1, const void *p2) {
  const vtx_node* ss1, *ss2;

  ss1 = p1;
  ss2 = p2;

  return memcmp(&(ss1->cost), &(ss2->cost), sizeof(ss1->cost));
}
