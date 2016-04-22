/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO GRAFO *******************************
*************************************************************************************************
************************************************************************************************/
#include "graph.h"

void g_free_graph(vertex **graph, int size)
{
	if(!(*graph) || size < 1) return;
	int i;
	edge * edge_aux;
    heap_node * node;

	for (i = 0; i < size; i++)
	{
		edge_aux = (*graph)[i].adjacent;
		while(edge_aux)
		{
			(*graph)[i].adjacent = (*graph)[i].adjacent->next_adj;
			free(edge_aux);
			edge_aux = (*graph)[i].adjacent;
		}

        node = (heap_node *)((*graph)[i].h_node.key);
        free(node);
	}
	free(*graph);
	*graph = NULL;
}

vertex *g_create_graph(int size)
{
	if(size < 1) return NULL;

	vertex * graph = (vertex *) calloc(size, sizeof(vertex));
	if(!graph) return NULL;

	for (; size > 0; --size)
	{
		graph[size-1].h_node.key = calloc(1, sizeof(vtx_node));
        if(!graph[size-1].h_node.key) return NULL;
        ((vtx_node*)graph[size-1].h_node.key)->key = size-1;
		((vtx_node*)graph[size-1].h_node.key)->cost = INF;
		graph[size-1].pi = -1;

	}

	return graph;
}
void g_print_graph(vertex *graph, int size)
{

	if(!graph || size < 1)
	{
		puts("Grafo invalido ou vazio!");
		return;
	}
	int i;
	edge * edge_aux;

	for(i = 0; i < size; ++i)
	{
		edge_aux = graph[i].adjacent;
		printf("Vertice\t\t%d\npi:\t\t%d\ncost:\t\t%d\narestas:\t", i, graph[i].pi == -3? -1 :graph[i].pi, ((vtx_node*)graph[i].h_node.key)->cost);

		while(edge_aux)
		{
			printf("%d {cost %d} - ", edge_aux->head_vertex, edge_aux->cost );
			edge_aux = edge_aux->next_adj;
		}

		printf("NULL\n---------------------------------------------------------------\n");
	}

	puts("\n");
}


edge * g_insert_edge(vertex *graph, int tail, int head, int cost)
{
	if(!graph) return NULL;

	edge * edge_added = (edge *) calloc(1, sizeof(edge));

	if(edge_added)
	{
		edge_added->head_vertex = head;
		edge_added->tail_vertex = tail;
		edge_added->cost = cost;
		edge_added->hot_line = 0;

		edge_added->next_adj = graph[tail].adjacent;
		graph[tail].adjacent = edge_added;

		edge_added->next_pred = graph[head].predecessor;
		graph[head].predecessor = edge_added;
	}

	return edge_added;

}
