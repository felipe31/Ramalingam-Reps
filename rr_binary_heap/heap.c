/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/
#include "heap.h"

heap * heap_new()
{
	heap * new_heap = (heap *) malloc(sizeof(heap));
	if(new_heap)
	{
		new_heap->node_vector = (heap_node **) malloc(HEAP_SIZE*sizeof(heap_node *));
		new_heap->control = 0;
	}

	return new_heap;
}

heap_node * heap_extract(heap * queue)
{
	if(!queue) return NULL;

	heap_node * extracted = queue->node_vector[0];
	queue->control--;
	if(queue->control > 0)
	{
		queue->node_vector[0] = queue->node_vector[queue->control];
		heapfy(queue, 0);
	}

	((vtx_node*)extracted->key)->mark = 0;
	return extracted;
}

int heap_checks_presence(heap_node * h_node, heap * queue)  	// Retorna 0 se o nó não pertence ao heap,
{															// -1 caso haja erros e retorna a posição caso o nó pertença ao heap
	if(!queue || !h_node) return -1;

	heap_node ** vector = queue->node_vector;
	int i;

	for (i = 0; i < queue->control; ++i)
	{
		if(vector[i] == h_node)
			return i;
	}
	return 0;
}

void heap_update(vertex * vtx, int new_pi, int new_cost, heap * queue)
{
	vtx->pi = new_pi;
	((vtx_node*)vtx->h_node.key)->cost = new_cost;
	heap_build(queue);
}

int heap_insert(heap_node * node_to_insert, heap * queue)       // Retorna 0 se o nó foi inserido com sucesso no heap e -1 caso contrário
{
	if(!queue || !node_to_insert) return -1;
	if(queue->control == HEAP_SIZE) return -1;

	((vtx_node*)node_to_insert->key)->mark = 1;
	queue->node_vector[queue->control] = node_to_insert;
	queue->control++;
	heap_build(queue);
	return 0;
}

void heapfy(heap * queue, int idx)
{
	int l_child, r_child, smallest;
	heap_node * aux;

	l_child = 2 * idx + 1;
	r_child = 2 * idx + 2;
	if ((l_child < queue->control)  && (((vtx_node*)queue->node_vector[l_child]->key)->cost < ((vtx_node*)queue->node_vector[idx]->key)->cost))
		smallest = l_child;
	else
		smallest = idx;
	if ((r_child < queue->control)  && (((vtx_node*)queue->node_vector[r_child]->key)->cost < ((vtx_node*)queue->node_vector[smallest]->key)->cost))
		smallest = r_child;
	if (smallest != idx)
	{
		aux = queue->node_vector[smallest];
		queue->node_vector[smallest] = queue->node_vector[idx];
		queue->node_vector[idx] = aux;

		heapfy(queue, smallest);
	}
	return;
}

void heap_build(heap * queue)
{
	int i;
	for (i = (queue->control-2)/2; i > -1 ; i--)
		heapfy(queue, i);

	return;
}

void heap_print(heap * queue)
{
	int i;
	if(!queue)
		printf("Heap invalido\n");
	else if(queue->control)
	{
		for ( i = 0; i < queue->control; i++)
		{
			printf("Elemento %d:\ncost:\t%d\npi:\t%d\nkey:\t%d\n\n", i, ((vtx_node*)queue->node_vector[i]->key)->cost, ((vertex *)(queue->node_vector[i]))->pi, ((vtx_node*)queue->node_vector[i]->key)->key);
		}
	}
	else
		printf("Heap vazio!\n\n");

		puts("\n");
	return;
}
