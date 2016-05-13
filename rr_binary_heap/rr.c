#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "graph.h"
#include "rr.h"

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE RAMALINGAM E REPS *******************************
*************************************************************************************************
************************************************************************************************/

void rr_recalculate_shortest_path(vertex *graph, heap *queue)
{
	if(!graph)
		return;

	heap_node * min = NULL;
	edge * edge_aux, * edge_p;
	int pos;

	while(!heap_is_empty(queue))
	{
		min = heap_extract(queue);
		if(!min) return;

		edge_aux = ((vertex *) min)->adjacent;
		while(edge_aux)
		{
			if((heap_is_added((heap_node *)(graph + edge_aux->head_vertex))))
			{
				if(((vtx_node*)graph[edge_aux->head_vertex].h_node.key)->cost > ((vtx_node*)min->key)->cost + edge_aux->cost )
				{									//relax();
					(graph + edge_aux->head_vertex)->pi = ((vtx_node*)min->key)->key;
					heap_update(&((graph + edge_aux->head_vertex)->h_node), ((vtx_node*)min->key)->cost + edge_aux->cost, queue);
				}
			}
			else
			{
				if(((vtx_node*)graph[edge_aux->head_vertex].h_node.key)->cost > ((vtx_node*)min->key)->cost + edge_aux->cost)
				{
					((vtx_node*)graph[edge_aux->head_vertex].h_node.key)->cost = ((vtx_node*)min->key)->cost + edge_aux->cost;
					graph[edge_aux->head_vertex].pi = ((vtx_node*)min->key)->key;
					if(heap_insert((heap_node *) &graph[edge_aux->head_vertex], queue))
						printf("No %d nao inserido\n", edge_aux->head_vertex);
				}
			}
			edge_aux = edge_aux->next_adj;
		}
	}
}


void rr_add_edge(vertex *graph, int tail, int head, int cost, heap * queue){
	if(!graph || !queue) return;

	edge * edge_added = g_insert_edge(graph, tail, head, cost);

	int new_cost =((vtx_node*)graph[tail].h_node.key)->cost + edge_added->cost;
	if (((vtx_node*)graph[head].h_node.key)->cost <= new_cost) return;

	((vtx_node*)graph[head].h_node.key)->cost = new_cost;
	graph[head].pi = tail;

	if(heap_insert((heap_node *)(graph + head), queue))
		printf("No %d nao inserido\n", ((vtx_node*)graph[head].h_node.key)->key);

	rr_recalculate_shortest_path(graph, queue);
    return;
}



void rr_remove_edge(vertex *graph, int tail, int head, heap * queue)
{
	if(!queue || !graph || tail < 0 || head < 0) return;

	edge * edge_removed = find_edge_adj(graph+tail, head);
	edge ** edge_adj = find_pointer_edge_adj(graph+tail, edge_removed );
	edge ** edge_pred = find_pointer_edge_pred(graph+head, edge_removed );
	if(!edge_adj || !edge_pred)	return;

	*edge_adj = edge_removed->next_adj;
	*edge_pred = edge_removed->next_pred;

	if(graph[edge_removed->head_vertex].pi != edge_removed->tail_vertex)
	{
		free(edge_removed);
		return;
	}

	head_list *affected_list = rr_mark_affected(graph, edge_removed);

    rr_estimate_new_pi(graph, affected_list, queue);
    rr_recalculate_shortest_path(graph, queue);

    free(edge_removed);
	return;
}

head_list *rr_mark_affected(vertex *graph, edge *edge_marked)
{
	if(!graph || !edge_marked) return NULL;

	head_list * aux_list = list_vtx_new();
	head_list * affected_list = list_vtx_new();

	((vtx_node*)graph[edge_marked->head_vertex].h_node.key)->cost = INF;
	graph[edge_marked->head_vertex].pi = -3;
	list_vtx_insert(aux_list, graph+edge_marked->head_vertex);

	vertex * vtx;
	edge * edge_aux;
	while (aux_list->first)
	{
		vtx = list_vtx_remove(aux_list);

		list_vtx_insert(affected_list, vtx);

		edge_aux = vtx->adjacent;
		while(edge_aux)
		{
			if(graph[edge_aux->head_vertex].pi == edge_aux->tail_vertex)
			{
				((vtx_node*)graph[edge_aux->head_vertex].h_node.key)->cost = INF;
				graph[edge_aux->head_vertex].pi = -3;					// -3 indica que o nó foi afetado e está na lista

				list_vtx_insert(aux_list, graph+edge_aux->head_vertex);
			}
			edge_aux = edge_aux->next_adj;
		}
	}
	return affected_list;
}

void rr_estimate_new_pi(vertex *graph, head_list *affected_list, heap *queue)
{
	if(!graph || !affected_list || !queue) return;

	vertex * vtx;
	edge * edge_aux;
	while(affected_list->first)
	{
		vtx = list_vtx_remove(affected_list);
		edge_aux = vtx -> predecessor;
		while (edge_aux)
		{
			if((graph+edge_aux->tail_vertex)->pi != -3)							// -3 indica que o nó foi afetado e está na lista
			{
				if(((vtx_node*)vtx->h_node.key)->cost > ((vtx_node*)graph[edge_aux->tail_vertex].h_node.key)->cost + edge_aux->cost )
				{
					((vtx_node*)vtx->h_node.key)->cost = ((vtx_node*)graph[edge_aux->tail_vertex].h_node.key)->cost + edge_aux->cost;
					vtx->pi = edge_aux->tail_vertex;
				}
			}
			edge_aux = edge_aux->next_pred;
		}
		if(((vtx_node*)vtx->h_node.key)->cost < INF)
			heap_insert((heap_node *)vtx, queue);
	}

	return;
}


void rr_print_sssp(vertex * graph)
{

}

edge ** find_pointer_edge_adj(vertex *tail, edge *edge_ref)
{
	edge * edge_p = tail->adjacent;

	if(edge_p == edge_ref)
		return &tail->adjacent;

	while(1)
	{
		if (edge_p->next_adj == edge_ref)
		{
			break;
		}
		else if(edge_p->next_adj == NULL)
		{
			return NULL;
		}

		edge_p = edge_p->next_adj;
	}

	return &edge_p->next_adj;
}

edge ** find_pointer_edge_pred(vertex *head, edge *edge_ref)
{
	edge * edge_p = head->predecessor;

	if(edge_p == NULL)
		return NULL;

	if(edge_p == edge_ref)
		return &head->predecessor;

	while(1)
	{
		if (edge_p->next_pred == edge_ref)
		{
			break;
		}
		else if(edge_p->next_pred == NULL)
		{
			return NULL;
		}

		edge_p = edge_p->next_pred;
	}

	return &edge_p->next_pred;
}


edge * find_edge_adj(vertex * tail, int key)
{
	edge * edge_adj = tail->adjacent;

	if(edge_adj == NULL)
		return NULL;

	while(edge_adj->head_vertex != key)
	{
		edge_adj = edge_adj->next_adj;

		if(edge_adj == NULL)
			return NULL;
	}

	return edge_adj;
}

edge * find_edge_pred(vertex * head, int key)
{
	edge * edge_pred = head->predecessor;

	if(edge_pred == NULL || key == -1)
		return NULL;

	while(edge_pred->tail_vertex != key)
	{
		edge_pred = edge_pred->next_pred;

		if(edge_pred == NULL)
			return NULL;
	}

	return edge_pred;
}


/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DE LISTA *******************************
*************************************************************************************************
************************************************************************************************/

head_list * list_vtx_new()
{
	return (head_list *) calloc(1, sizeof(head_list));
}

list * list_vtx_insert(head_list * h_list, vertex * v_list)
{
	if(!h_list || !v_list) return NULL;

	list * l = (list *) malloc(sizeof(list));
	if(l)
	{
		l->vtx = v_list;
		if(h_list -> last)
			h_list -> last -> next = l;
		else
			h_list->first = l;
		h_list -> last = l;
		l->next = NULL;
	}
	return l;
}

vertex * list_vtx_remove(head_list * h_list)
{
	if(!h_list) return NULL;
	if(!h_list->first) return NULL;

	vertex * vtx = h_list->first->vtx;
	list * aux = h_list->first;

	h_list->first = h_list->first->next;

	if(!h_list->first)
		h_list->last = NULL;

	free(aux);
	return vtx;
}
