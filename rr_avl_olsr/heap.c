/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/
#include <string.h>
#include "heap.h"

heap * heap_new()
{
	struct avl_tree * _routing_tree = (struct avl_tree *) calloc(1, sizeof(struct avl_tree));

	if(!_routing_tree) return NULL;

	avl_init(_routing_tree, cmp_key, false);
	return _routing_tree;
}


int heap_insert(heap_node * node_to_insert, heap * queue)       // Retorna 0 se o nó foi inserido com sucesso no heap e -1 caso contrário
{
	if(!queue || !node_to_insert) return -1;

	avl_insert(queue, node_to_insert);
	return 0;
}


heap_node * heap_extract(heap * queue)
{
	if(!queue) return NULL;

    vertex *vtx;
    vtx = (vertex *) avl_first_element_safe(queue, vtx, h_node);

    if(!vtx) return NULL;

    avl_remove(queue, &(vtx->h_node));

	return &(vtx->h_node);
}


void heap_update(vertex * vtx, int new_pi, int new_cost, heap * queue)
{
    avl_remove(queue, &(vtx->h_node));
    vtx->pi = new_pi;
    ((vtx_node *)vtx->h_node.key)->cost = new_cost;
    avl_insert(queue, &(vtx->h_node));
}

void heap_print(heap * queue)
{
    vertex * vtx;
    if(!queue)
        printf("Heap invalido\n");
    else if(!avl_is_empty(queue))
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


int
cmp_key(const void *p1, const void *p2) {
  const vtx_node* ss1, *ss2;

  ss1 = p1;
  ss2 = p2;

  return memcmp(&(ss1->cost), &(ss2->cost), sizeof(ss1->cost));
}
