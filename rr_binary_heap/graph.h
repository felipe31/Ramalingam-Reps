/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO GRAFO *******************************
*************************************************************************************************
************************************************************************************************/
#ifndef _GRAPH_
#define _GRAPH_
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "heap.h"

#define INF (INT_MAX-500)

typedef struct edge {
    int head_vertex, cost, tail_vertex; // tail -> head
    struct edge *next_pred;             // prox na lista de predecessores
    struct edge *next_adj;              // prox na lista de adjacentes
} edge;

typedef struct vertex {
    heap_node h_node;
    int pi;
    edge *adjacent;                     // lista de adjacentes
    edge *predecessor;                  // lista de predecessores
} vertex;

void g_free_graph(vertex **graph, int size);
vertex *g_create_graph(int size);
void g_print_graph(vertex *graph, int size);
edge * g_insert_edge(vertex *graph, int tail, int head, int cost);

#endif
