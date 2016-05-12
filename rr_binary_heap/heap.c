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

void heap_update(heap_node * node, int new_cost, heap * queue)
{
	int old = ((vtx_node*) node->key)->cost;
	((vtx_node*) node->key)->cost = new_cost;
	if(old < new_cost)
		heap_increase_key(queue, node->position);
	else
		heap_decrease_key(queue, node->position);
}

int heap_insert(heap_node * node_to_insert, heap * queue)       // Retorna 0 se o nó foi inserido com sucesso no heap e -1 caso contrário
{
	if(!queue || !node_to_insert) return -1;
	if(queue->control == HEAP_SIZE) return -1;

	((vtx_node*)node_to_insert->key)->mark = 1;
	queue->node_vector[queue->control] = node_to_insert;
	queue->node_vector[queue->control]->position = queue->control;
	queue->control++;
	heap_decrease_key(queue, queue->control-1);
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
		queue->node_vector[smallest]->position = smallest;
		queue->node_vector[idx] = aux;
		queue->node_vector[idx]->position = idx;

		heapfy(queue, smallest);
	}
	return;
}

void heap_decrease_key(heap * queue, int pos)
{
	int pi  = (pos-1)/2;
	heap_node * aux;
	while (pos > -1 && ((vtx_node*)queue->node_vector[pos]->key)->cost < ((vtx_node*)queue->node_vector[pi]->key)->cost)
	{
		aux = queue->node_vector[pi];
		queue->node_vector[pi] = queue->node_vector[pos];
		queue->node_vector[pi]->position = pi;

		queue->node_vector[pos] = aux;
		queue->node_vector[pos]->position = pos;

		pos = pi;
		pi = (pi-1)/2;
	}

	return;
}
void heap_increase_key(heap * queue, int pos)
{
	int son = pos*2+1;
	if(son+1 < queue->control)
	{
		if (((vtx_node*)queue->node_vector[son]->key)->cost > ((vtx_node*)queue->node_vector[son+1]->key)->cost)
			son++;
	}

	printf("aaa\n" );
	heap_node * aux;
	while ((son < queue->control) && (((vtx_node*)queue->node_vector[son]->key)->cost < ((vtx_node*)queue->node_vector[pos]->key)->cost))
	{
		aux = queue->node_vector[son];
		queue->node_vector[son] = queue->node_vector[pos];
		queue->node_vector[son]->position = son;

		queue->node_vector[pos] = aux;
		queue->node_vector[pos]->position = pos;

		pos = son;
		son = pos*2+1;
		if(son+1 < queue->control)
		{
			if (((vtx_node*)queue->node_vector[son]->key)->cost > ((vtx_node*)queue->node_vector[son+1]->key)->cost)
			son++;
		}
	}


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
		//	printf("Elemento %d:\ncost:\t%d\npi:\t%d\nkey:\t%d\n\n", i, ((vtx_node*)queue->node_vector[i]->key)->cost, ((vertex *)(queue->node_vector[i]))->pi, ((vtx_node*)queue->node_vector[i]->key)->key);
			printf("Elemento %d:\ncost:\t%d\npi:\t\nkey:\t%d\n\n", i, ((vtx_node*)queue->node_vector[i]->key)->cost, ((vtx_node*)queue->node_vector[i]->key)->key);
		}
	}
	else
		printf("Heap vazio!\n\n");

		puts("\n");
	return;
}


int mai ()
{
	heap * q = heap_new();
	heap_node n1, n2, n3, n4, n5 , n6, n7, n8, n9, n10;
	n1.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n2.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n3.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n4.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n5.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n6.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n7.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n8.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n9.key = (vtx_node*)calloc(1, sizeof(vtx_node));
	n10.key = (vtx_node*)calloc(1, sizeof(vtx_node));

	((vtx_node*)n10.key)->cost = 1;
	((vtx_node*)n10.key)->key = 0;
	((vtx_node*)n1.key)->cost = 3;
	((vtx_node*)n1.key)->key = 1;
	((vtx_node*)n2.key)->cost = 5;
	((vtx_node*)n2.key)->key = 2;
	((vtx_node*)n3.key)->cost = 7;
	((vtx_node*)n3.key)->key = 3;
	((vtx_node*)n4.key)->cost = 9;
	((vtx_node*)n4.key)->key = 4;
	((vtx_node*)n5.key)->cost = 13;
	((vtx_node*)n5.key)->key = 5;
	((vtx_node*)n6.key)->cost = 10;
	((vtx_node*)n6.key)->key = 6;
	((vtx_node*)n7.key)->cost = 15;
	((vtx_node*)n7.key)->key = 7;
	((vtx_node*)n8.key)->cost = 17;
	(	(vtx_node*)n8.key)->key = 8;
	((vtx_node*)n9.key)->cost = 19;
	((vtx_node*)n9.key)->key = 9;


	heap_insert(&n1, q);
	heap_insert(&n2, q);
	heap_insert(&n3, q);
	heap_insert(&n5, q);
	heap_insert(&n4, q);
	heap_insert(&n6, q);
	heap_insert(&n8, q);
	heap_insert(&n9, q);
	heap_insert(&n7, q);
	heap_insert(&n10, q);

	int i;
	puts("");
	for(i=0; i<q->control; i++)
		printf("[%d, %d] ", ((vtx_node*)q->node_vector[i]->key)->cost, i);



	puts("-----");
	//heap_decrease_key(q, n7.position);
	heap_update(&n7, 0, q);
	for(i=0; i<q->control; i++)	printf("[%d, %d] ", ((vtx_node*)q->node_vector[i]->key)->cost, i); puts("-----");
	heap_update(&n7, 20, q);
	for(i=0; i<q->control; i++)	printf("[%d, %d] ", ((vtx_node*)q->node_vector[i]->key)->cost, i); puts("-----");

	return 0;
}
