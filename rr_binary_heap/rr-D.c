#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "heap.h"
#define INF (INT_MAX-500)

int n_ver;

typedef heap_node node;
typedef vtx_node* keyType;

typedef struct edge{
    int head_vertex, cost;
    struct edge *next;
} edge;

typedef struct vertex{
    int pi;
    int cost;
    node h_node;
    edge *adjacent;
} vertex;

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/

heap* heapCreateRoot(){

    return heap_new();
}


void heapDecreaseKey(heap *root, node *h_node, int new_cost){
    heap_update(h_node, new_cost, root);
}

void heapInsert(heap *root, node *new_node){
    heap_insert(new_node, root);
}

void heapIncreseKey(heap *root, node *h_node, int new_cost){
    heap_update(h_node, new_cost, root);
}

node *heapExtractMin(heap *root){
    return heap_extract(root);
}

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO GRAFO *******************************
*************************************************************************************************
************************************************************************************************/

void initGraph(vertex *vertices, int size){

    int i;
    vertices[0].pi = -2;
    vertices[0].adjacent = NULL;
    vertices[0].h_node.key = (vtx_node*) malloc(sizeof(vtx_node));
    ((vtx_node*)vertices[0].h_node.key)->cost = -1;
    ((vtx_node*)vertices[0].h_node.key)->key = 0;
    vertices[0].cost = 0;
    for(i=1; i<size; i++) {
        vertices[i].pi = -1;
        vertices[i].adjacent = NULL;
        vertices[i].h_node.key = (vtx_node*) malloc(sizeof(vtx_node));
        ((vtx_node*)vertices[i].h_node.key)->cost = -1;
        ((vtx_node*)vertices[i].h_node.key)->key = i;
        vertices[i].cost = INF;
    }
}

void removeEdge(vertex *vertices, int tail, int head){

    edge *aux = vertices[tail].adjacent;
    if(!aux)
        return;
    if(aux->head_vertex == head){
        vertices[tail].adjacent = vertices[tail].adjacent->next;
        free(aux);
    }
    else{
        edge *aux2 = aux->next;
        while(aux2){
            if(aux2->head_vertex == head){
                aux->next = aux2->next;
                free(aux2);
                break;
            }
            aux = aux2;
            aux2 = aux2->next;
        }
    }
}

void removeInv(edge **inv, int tail, int head){

    edge *aux = inv[tail];
    if(!aux)
        return;
    if(aux->head_vertex == head){
        inv[tail] = inv[tail]->next;
        free(aux);
    }
    else{
        edge *aux2 = aux->next;
        while(aux2){
            if(aux2->head_vertex == head){
                aux->next = aux2->next;
                free(aux2);
                break;
            }
            aux = aux2;
            aux2 = aux2->next;
        }
    }
}

int checkEdge(vertex *vertices, int tail, int head){

    edge *aux = vertices[tail].adjacent;
    while(aux){
        if(aux->head_vertex == head){
            return aux->cost;
        }
        aux = aux->next;
    }
    return 0;
}

void insertEdge(vertex *vertices, int tail, int head, int cost){

    edge *new_edge = (edge*)malloc(sizeof(edge));
    new_edge->next = vertices[tail].adjacent;
    new_edge->head_vertex = head;
    new_edge->cost = cost;
    vertices[tail].adjacent = new_edge;
}

void insertInv(edge **inv, int ori, int dest){

    edge *new_edge = (edge*)malloc(sizeof(edge));
    new_edge->next = inv[ori];
    new_edge->head_vertex = dest;
    inv[ori] = new_edge;
}

void freeGraph(vertex *vertices, int size){

    int i;
    edge *aux, *aux2;
    for(i=1; i<size; i++){
        aux = vertices[i].adjacent;
        while(aux){
            aux2 = aux->next;
            free(aux);
            aux = aux2;
        }
        vertices[i].adjacent = NULL;
    }
    //free(vertices);
}

void freeInv(edge **inv, int size){

    int i;
    edge *aux, *aux2;
    for(i=1; i<size; i++){
        aux = inv[i];
        while(aux){
            aux2 = aux->next;
            free(aux);
            aux = aux2;
        }
        inv[i] = NULL;
    }
    //free(vertices);
}
void g_print_graph(vertex *graph, int size) {
	if(!graph || size < 1) {
		puts("Grafo invalido ou vazio!");
		return;
	}
	int i;
	edge * edge_aux;
	for(i = 0; i < size; ++i) {
		edge_aux = graph[i].adjacent;
		printf("Vertice\t\t%d\npi:\t\t%d\ncost:\t\t%d\narestas:\t", i, graph[i].pi, graph[i].cost);
		while(edge_aux) {
			printf("%d {cost %d} - ", edge_aux->head_vertex, edge_aux->cost);
			edge_aux = edge_aux->next;
		}
		printf("NULL\n---------------------------------------------------------------\n");
	}
	puts("\n");
}
/*
void printGraph(vertex *vertices, int ver){
    int i;
    edge *aux;
    node n;
    for (i = 1; i < ver; i++) {
        aux = vertices[i].adjacent;
        printf("%d pi: %d\tcost: %d\n", i, vertices[i].pi, vertices[i].cost);
        n = vertices[i].h_node;
        if(n.key != -1)
            printf("key: %d\tvertex: %d\tparent: %p\tleft: %p\tright: %p\n", n.key, n.vertex, n.parent, n.left, n.right);
        else
            puts("h_node: NULL");
        while(aux){
            printf("head_vertex: %d\tcost: %d\n", aux->head_vertex, aux->cost);
            aux = aux->next;
        }
    }
}
*/
void dijkstra(vertex *vertices, heap *root){
    int min_vertex, old_cost, new_cost, adjacent;
    //printGraph(vertices, n_ver);
    node *min_node;
    while(!heap_is_empty(root)){
        min_node = heapExtractMin(root);
        min_vertex = ((vtx_node*)min_node->key)->key;
        ((vtx_node*)vertices[min_vertex].h_node.key)->cost = -1;
        //printf("\nmin_vertex: %d\n", min_vertex);
        edge *aux = vertices[min_vertex].adjacent;
        while(aux){
            adjacent = aux->head_vertex;
            new_cost = vertices[min_vertex].cost + aux->cost;
                //printf("new_cost: %d\tadjacent: %d\n", new_cost, adjacent);
            if(new_cost < vertices[adjacent].cost){
                if(((vtx_node*)vertices[adjacent].h_node.key)->cost == -1){
                    ((vtx_node*)vertices[adjacent].h_node.key)->cost = new_cost;
                    heapInsert(root, &(vertices[adjacent].h_node));
                }
                else{
                    heapDecreaseKey(root, &(vertices[adjacent].h_node), new_cost);
                }
                vertices[adjacent].pi = min_vertex;
                vertices[adjacent].cost = new_cost;
                //printGraph(vertices, n_ver);
            }
            aux = aux->next;
        }
    }
}

void resetaGrafo(vertex *vertices, int ver){
    int i;
    vertices[1].pi = 1;
    vertices[1].cost = 0;
    for(i=2; i<ver; i++) {
        vertices[i].pi = -1;
        vertices[i].cost = INF;
    }
}

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE RAMALINGAM E REPS *******************************
*************************************************************************************************
************************************************************************************************/

void inserir(vertex *vertices, edge **inv, heap *root, int ver, int ori, int dest, int custo){
    //printGraph(vertices, ver);
    insertEdge(vertices, ori, dest, custo);
    insertInv(inv, dest, ori);
    if((vertices[ori].cost + custo) < vertices[dest].cost){
        //resetaGrafo(vertices, ver);
        vertices[dest].cost = vertices[ori].cost + custo;
        vertices[dest].pi = ori;
        ((vtx_node*)vertices[dest].h_node.key)->cost = vertices[dest].cost;
        heapInsert(root, &(vertices[dest].h_node));
        dijkstra(vertices, root);
    }
}

void afetados(vertex *vertices, edge **inv, heap *root, int ver, int afetado) {
    vertices[afetado].cost = INF;
    vertices[afetado].pi = -1;

    edge *aux;
    aux = vertices[afetado].adjacent;
    while(aux){
        if(vertices[aux->head_vertex].pi == afetado)
            afetados(vertices, inv, root, ver, aux->head_vertex);
        aux = aux->next;
    }
    aux = inv[afetado];
    while(aux){
        if(vertices[aux->head_vertex].cost != INF){
            if(((vtx_node*)vertices[aux->head_vertex].h_node.key)->cost == -1){
                ((vtx_node*)vertices[aux->head_vertex].h_node.key)->cost = vertices[aux->head_vertex].cost;
                heapInsert(root, &(vertices[aux->head_vertex].h_node));
                //printf("count: %d\troot_node: %p\tlast_node: %p\n", root->count, root->root_node, root->last_node);
                //heapPrint(root->root_node);
            }
        }
        aux = aux->next;
    }
}

void remover(vertex *vertices, edge **inv, heap *root, int ver, int ori, int dest){
    removeEdge(vertices, ori, dest);
    removeInv(inv, dest, ori);
    if(vertices[dest].pi == ori){
        afetados(vertices, inv, root, ver, dest);
        dijkstra(vertices, root);
    }
}


int main(){

    int size_g, size_edg, source, target, cost;
    scanf("%d%d", &size_g, &size_edg);
    if(size_g < 1 || size_edg < 1)
        return -1;

    heap *root = heapCreateRoot();
    vertex *vertices = (vertex*)calloc(size_g, sizeof(vertex));
    edge **inv = (edge**)calloc(size_g, sizeof(edge*));
    initGraph(vertices, size_g);

    ((vtx_node*)vertices[0].h_node.key)->key = 0;
    ((vtx_node*)vertices[0].h_node.key)->cost = 0;

    heapInsert(root, &(vertices[0].h_node));

    while(scanf("%d%d%d", &source, &target, &cost) != EOF) {
        inserir(vertices, inv, root, size_g, source, target, cost);
    }

    g_print_graph(vertices, size_g);

    freeGraph(vertices, size_g);
    freeInv(inv, size_g);
    free(vertices);
    free(inv);
    free(root);
    return 0;
}
