#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
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
		if(((vertex *) min)->pi != -2)												// -2 indica que o nó é a origem
		{
			edge_aux = find_edge_pred((vertex *) min, ((vertex *) min)->pi);
			if(!edge_aux) return;
			edge_aux->hot_line = 1;
		}
		edge_aux = ((vertex *) min)->adjacent;

		while(edge_aux)
		{
			if((heap_is_added((heap_node *)(graph + edge_aux->head_vertex))))
			{
				if(((vtx_node*)graph[edge_aux->head_vertex].h_node.key)->cost > ((vtx_node*)min->key)->cost + edge_aux->cost )									//relax();
					heap_update((graph + edge_aux->head_vertex), ((vtx_node*)min->key)->key, ((vtx_node*)min->key)->cost + edge_aux->cost, queue);
			}
			else
			{
				if(((vtx_node*)graph[edge_aux->head_vertex].h_node.key)->cost > ((vtx_node*)min->key)->cost + edge_aux->cost)
				{
					edge_p = find_edge_pred(graph+edge_aux->head_vertex, graph[edge_aux->head_vertex].pi);
					if(edge_p) edge_p->hot_line = 0;

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


void rr_add_edge(vertex *graph, int tail, int head, int cost){

	if(!graph) return;

	edge * edge_added = g_insert_edge(graph, tail, head, cost);

	int new_cost = ((vtx_node*)graph[tail].h_node.key)->cost + edge_added->cost;

	if (((vtx_node*)graph[head].h_node.key)->cost <= new_cost)
		return;

	heap * queue = heap_new();
	if(!queue) return;

	edge_added->hot_line = 1;
	edge_added = find_edge_pred(graph+head, graph[head].pi);
	if(edge_added)
		edge_added->hot_line = 0;
	((vtx_node*)graph[head].h_node.key)->cost = new_cost;
	graph[head].pi = tail;

	if(heap_insert((heap_node *)(graph + head), queue))
		printf("No %d nao inserido\n", ((vtx_node*)graph[head].h_node.key)->key);

	rr_recalculate_shortest_path(graph, queue);

    return;
}



void rr_remove_edge(vertex *graph, int tail, int head)
{
 	heap *queue = heap_new();
	if(!queue) return;

	edge * edge_removed = find_edge_adj(graph+tail, head);
	edge ** edge_adj = find_pointer_edge_adj(graph+tail, edge_removed );
	edge ** edge_pred = find_pointer_edge_pred(graph+head, edge_removed );
	if(!edge_adj || !edge_pred)	return;

	*edge_adj = edge_removed->next_adj;
	*edge_pred = edge_removed->next_pred;

	if(edge_removed->hot_line == 0)
	{
		free(edge_removed);
		return;
	}

	edge_removed->hot_line == 0;
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
			if(edge_aux->hot_line)
			{
				edge_aux->hot_line = 0;
				((vtx_node*)graph[edge_aux->head_vertex].h_node.key)->cost = INF;
				graph[edge_aux->head_vertex].pi = -3;					// -3 indica que o nó foi afetado e está na lista

				list_vtx_insert(aux_list, graph+edge_aux->head_vertex);
//				break;
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
		else if(edge_p->next_adj)
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

	if(!edge_p)
		return NULL;

	if(edge_p == edge_ref)
		return &head->predecessor;

	while(1)
	{
		if (edge_p->next_pred == edge_ref)
		{
			break;
		}
		else if(!edge_p->next_pred)
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

	if(!edge_adj)
		return NULL;

	while(edge_adj->head_vertex != key)
	{
		edge_adj = edge_adj->next_adj;

		if(!edge_adj)
			return NULL;
	}

	return edge_adj;
}

edge * find_edge_pred(vertex * head, int key)
{
	edge * edge_pred = head->predecessor;

	if(!edge_pred || key == -1)
		return NULL;

	while(edge_pred->tail_vertex != key)
	{
		edge_pred = edge_pred->next_pred;

		if(!edge_pred)
			return NULL;
	}

	return edge_pred;
}
/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO GRAFO *******************************
*************************************************************************************************
************************************************************************************************/

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
		printf("Vertice\t\t%d\npi:\t\t%d\ncost:\t\t%d\narestas:\t", i, graph[i].pi, ((vtx_node*)graph[i].h_node.key)->cost);

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


/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/

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

	list * l = (list *) calloc(1, sizeof(list));
	if(l)
	{
		l->vtx = v_list;
		if(h_list -> last)
			h_list -> last -> next = l;
		else
			h_list->first = l;
		h_list -> last = l;
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
