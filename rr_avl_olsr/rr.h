#ifndef _RR_
#define _RR_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include "avl.h"

#define HEAP_SIZE 1000
#define INF (INT_MAX-500)

typedef struct avl_tree heap;
typedef struct avl_node heap_node;


typedef struct vtx_node{
    int cost, key;                      // cost = custo mínimo no caminho mínimo | key = nome do vértice
} vtx_node;                                 // primeiramente usar um vetor

typedef struct edge {
    int head_vertex, cost, tail_vertex; // tail -> head
    char hot_line;                      // hot_line = aresta faz parte do caminho mínimo
    struct edge *next_pred;             // prox na lista de predecessores
    struct edge *next_adj;              // prox na lista de adjacentes
} edge;

typedef struct vertex {
    heap_node h_node;
    int pi;
    edge *adjacent;                     // lista de adjacentes
    edge *predecessor;                  // lista de predecessores
} vertex;

typedef struct list{
    vertex * vtx;
    struct list * next;
} list;

typedef struct head_list
{
    list * first;
    list * last;
} head_list;

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE RAMALINGAM E REPS *******************************
*************************************************************************************************
************************************************************************************************/
void rr_recalculate_shortest_path(vertex *graph, heap *queue);
void rr_add_edge(vertex *graph, int tail, int head, int cost);
void rr_remove_edge(vertex *graph,  int tail, int head);
head_list *rr_mark_affected(vertex *graph, edge *edge_marked);
void rr_estimate_new_pi(vertex *graph, head_list *affected_list, heap *queue);
edge * find_edge_pred(vertex * head, int key);
edge * find_edge_adj(vertex * tail, int key);
edge ** find_pointer_edge_pred(vertex *head, edge *edge_ref);
edge ** find_pointer_edge_adj(vertex *tail, edge *edge_ref);

void rr_print_sssp(vertex * graph);

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO GRAFO *******************************
*************************************************************************************************
************************************************************************************************/
void g_free_graph(vertex **graph, int size);

vertex *g_create_graph(int size);

void g_print_graph(vertex *graph, int size);

edge * g_insert_edge(vertex *graph, int tail, int head, int cost);


/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/


heap * heap_new();

heap_node * heap_extract(heap * queue);

void heap_update(vertex * vtx, int new_pi, int new_cost, heap * queue);

int heap_insert(heap_node * node_to_insert, heap * queue);       // Retorna 0 se o nó foi inserido com sucesso no heap e 1 caso contrário

void heap_print(heap * queue);

int cmp_key(const void *p1, const void *p2);

#define heap_is_added(node) \
    avl_is_node_added(node)

#define heap_is_empty(queue) \
    avl_is_empty(queue)

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DE LISTA *******************************
*************************************************************************************************
************************************************************************************************/

head_list * list_vtx_new();

list * list_vtx_insert(head_list * h_list, vertex * v_list);

vertex * list_vtx_remove(head_list * h_list);


#endif
