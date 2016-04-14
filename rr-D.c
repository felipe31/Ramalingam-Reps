#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>

#define INF (INT_MAX-500)

int n_ver;

typedef int keyType;

typedef struct node{
    keyType key;
    int vertex;
    struct node *parent, *left, *right;
}node;

typedef struct heap{
    unsigned int count;
    struct node *root_node, *last_node;
}heap;

typedef struct edge{
    int head_vertex, cost;
    struct edge *next;
} edge;

typedef struct vertex{
    int predecessor;
    int cost;
    node heap_node;
    edge *adjacent;
} vertex;

/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO HEAP *******************************
*************************************************************************************************
************************************************************************************************/

heap* heapCreateRoot(){
    heap *new_heap = (heap*)malloc(sizeof(heap));
    new_heap->count = 0;
    new_heap->root_node = new_heap->last_node = NULL;
    return new_heap;
}

node* heapCreateNode(keyType key, int vertex){
    node *new_node = (node*)malloc(sizeof(node));
    new_node->key = key;
    new_node->vertex = vertex;
    new_node->parent = new_node->left = new_node->right = NULL;
    return new_node;
}

unsigned int perfectLog2 (unsigned int number) {
    unsigned int log = 0, pow=1, original_number=number;
    while (number >>= 1) ++log;
    int i;
    for (i = 0; i < log; i++) {
        pow <<= 1;
    }
    return original_number-pow;
}

node* heapFindParentInsertNode(heap *root){
    node *aux = root->last_node;
    unsigned int N = root->count+1;
    if ( !perfectLog2(N) ){
        aux = root->root_node;
        while (aux->left) {
            aux = aux->left;
        }
    }
    else if ( N % 2 == 0){
        while(aux->parent->right == aux)
            aux = aux->parent;
        if(!aux->parent->right)
            return aux->parent;
        aux = aux->parent->right;
        while(aux->left)
            aux = aux->left;
    }
    else{
        aux = aux->parent;
    }
    return aux;
}

void heapDecreaseKey(heap *root, node *heap_node, keyType key){
    heap_node->key = key;
    node *parent = heap_node->parent;
    node *left = heap_node->left;
    node *right = heap_node->right;
    if(!parent)
        return;
    if(parent->key > heap_node->key){
        if(root->last_node == heap_node)
            root->last_node = parent;
    }
    while(parent && (parent->key > heap_node->key)){
        if(parent->left == heap_node){
            heap_node->left = parent;
            heap_node->right = parent->right;
            if(heap_node->right)
                heap_node->right->parent = heap_node;
            heap_node->parent = parent->parent;
            if(heap_node->parent){
                if(heap_node->parent->left == parent)
                    heap_node->parent->left = heap_node;
                else
                    heap_node->parent->right = heap_node;
            }
            else
                root->root_node = heap_node;
        }
        else{
            heap_node->right = parent;
            heap_node->left = parent->left;
            if(heap_node->left)
                heap_node->left->parent = heap_node;
            heap_node->parent = parent->parent;
            if(heap_node->parent){
                if(heap_node->parent->left == parent)
                    heap_node->parent->left = heap_node;
                else
                    heap_node->parent->right = heap_node;
            }
            else
                root->root_node = heap_node;
        }
        parent->left = left;
        parent->right = right;
        parent->parent = heap_node;
        if(left)
            left->parent = parent;
        if(right)
            right->parent = parent;
        parent = heap_node->parent;
        left = heap_node->left;
        right = heap_node->right;
    }
}

node* heapInsert(heap *root, node *new_node){
    node *parent = NULL;
    new_node->parent = new_node->left = new_node->right = NULL;
    if(!root->count){
        root->root_node = root->last_node = new_node;
        root->count++;
    }
    else{
        parent = heapFindParentInsertNode(root);
        if(parent->left){
            parent->right = new_node;
        }
        else{
            parent->left = new_node;
        }
        new_node->parent = parent;
        root->count++;
        root->last_node = new_node;
        heapDecreaseKey(root, new_node, new_node->key);
    }
    return new_node;
}


void heapSwapLeft(heap *root, node *heap_node){
    node *parent = heap_node->parent;
    node *left = heap_node->left;
    node *right = heap_node->right;

    heap_node->parent = left;
    heap_node->left = left->left;
    if(heap_node->left)
        heap_node->left->parent = heap_node;
    heap_node->right = left->right;
    if(heap_node->right)
        heap_node->right->parent = heap_node;

    left->parent = parent;
    if(parent){
        if(parent->left == heap_node)
            parent->left = left;
        else
            parent->right = left;
    }
    else
        root->root_node = left;
    left->left = heap_node;
    left->right = right;
    if(right)
        right->parent = left;
    if(root->last_node == left)
        root->last_node = heap_node;
}

void heapSwapRight(heap *root, node *heap_node){
    node *parent = heap_node->parent;
    node *left = heap_node->left;
    node *right = heap_node->right;

    heap_node->parent = right;
    heap_node->left = right->left;
    if(heap_node->left)
        heap_node->left->parent = heap_node;
    heap_node->right = right->right;
    if(heap_node->right)
        heap_node->right->parent = heap_node;

    right->parent = parent;
    if(parent){
        if(parent->left == heap_node)
            parent->left = right;
        else
            parent->right = right;
    }
    else
        root->root_node = right;
    right->right = heap_node;
    right->left = left;
    if(left)
        left->parent = right;
    if(root->last_node == right)
        root->last_node = heap_node;
}

void heapIncreseKey(heap *root, node *heap_node, keyType key){
    heap_node->key = key;
    node *parent = heap_node->parent;
    node *left = heap_node->left;
    node *right = heap_node->right;
    if(left && (heap_node->key > left->key)){
        if(right && (heap_node->key > right->key)){
            if(left->key < right->key)
                heapSwapLeft(root, heap_node);
            else
                heapSwapRight(root, heap_node);
        }
        else
            heapSwapLeft(root, heap_node);
        heapIncreseKey(root, heap_node, key);
    }
    else if(right && (heap_node->key > right->key)){
        heapSwapRight(root, heap_node);
        heapIncreseKey(root, heap_node, key);
    }
}

node* heapFindLastNode(heap *root){
    node *aux = root->last_node;
    unsigned int N = root->count+1;
    if ( !perfectLog2(N) ){
        aux = root->root_node;
        while (aux->right) {
            aux = aux->right;
        }
    }
    else if ( N % 2 == 0){
        while(aux->parent->left == aux)
            aux = aux->parent;
        aux = aux->parent->left;
        while(aux->right)
            aux = aux->right;
    }
    return aux;
}

node *heapExtractMin(heap *root){
    node *min_node = root->root_node;
    node *new_min = root->last_node;
    if(!min_node)
        return NULL;
    root->count--;
    if(root->count == 0){
        root->last_node = root->root_node = NULL;
        return min_node;
    }
    if(root->count == 1){
        root->last_node = root->root_node = new_min;
        new_min->parent = NULL;
        return min_node;
    }
    else{
        if(new_min->parent->left == new_min){
            new_min->parent->left = NULL;
            root->last_node = new_min->parent;
            root->last_node = heapFindLastNode(root);
        }
        else{
            new_min->parent->right = NULL;
            root->last_node = new_min->parent->left;
        }
        new_min->left = min_node->left;
        if(new_min->left)
            new_min->left->parent = new_min;
        new_min->right = min_node->right;
        if (new_min->right)
            new_min->right->parent = new_min;
        new_min->parent = NULL;
        root->root_node = new_min;
        heapIncreseKey(root, new_min, new_min->key);
    }
    return min_node;
}

void heapPrint(node *root){
    if(root){
        printf("key: %d\tvertex: %d\tparent: %p\tleft: %p\tright: %p\n", root->key, root->vertex, root->parent, root->left, root->right);
        heapPrint(root->left);
        heapPrint(root->right);
    }
}
/************************************************************************************************
*************************************************************************************************
****************************** ALGORITMOS DE MANIPULAÇÃO DO GRAFO *******************************
*************************************************************************************************
************************************************************************************************/

void initGraph(vertex *vertices, int size){

    int i;
    vertices[0].predecessor = -2;
    vertices[0].adjacent = NULL;
    vertices[0].heap_node.key = -1;
    vertices[0].heap_node.vertex = 0;
    vertices[0].heap_node.parent = NULL;
    vertices[0].heap_node.left = NULL;
    vertices[0].heap_node.right = NULL;
    vertices[0].cost = 0;
    for(i=1; i<size; i++) {
        vertices[i].predecessor = -1;
        vertices[i].adjacent = NULL;
        vertices[i].heap_node.key = -1;
        vertices[i].heap_node.vertex = i;
        vertices[i].heap_node.parent = NULL;
        vertices[i].heap_node.left = NULL;
        vertices[i].heap_node.right = NULL;
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
		printf("Vertice\t\t%d\npi:\t\t%d\ncost:\t\t%d\narestas:\t", i, graph[i].predecessor, graph[i].cost);
		while(edge_aux) {
			printf("%d {cost %d} - ", edge_aux->head_vertex, edge_aux->cost);
			edge_aux = edge_aux->next;
		}
		printf("NULL\n---------------------------------------------------------------\n");
	}
	puts("\n");
}

void printGraph(vertex *vertices, int ver){
    int i;
    edge *aux;
    node n;
    for (i = 1; i < ver; i++) {
        aux = vertices[i].adjacent;
        printf("%d predecessor: %d\tcost: %d\n", i, vertices[i].predecessor, vertices[i].cost);
        n = vertices[i].heap_node;
        if(n.key != -1)
            printf("key: %d\tvertex: %d\tparent: %p\tleft: %p\tright: %p\n", n.key, n.vertex, n.parent, n.left, n.right);
        else
            puts("heap_node: NULL");
        while(aux){
            printf("head_vertex: %d\tcost: %d\n", aux->head_vertex, aux->cost);
            aux = aux->next;
        }
    }
}

void dijkstra(vertex *vertices, heap *root){
    int min_vertex, old_cost, new_cost, adjacent;
    //printGraph(vertices, n_ver);
    node *min_node;
    while(root->root_node){
        min_node = heapExtractMin(root);
        min_vertex = min_node->vertex;
        vertices[min_vertex].heap_node.key = -1;
        //printf("\nmin_vertex: %d\n", min_vertex);
        edge *aux = vertices[min_vertex].adjacent;
        while(aux){
            adjacent = aux->head_vertex;
            new_cost = vertices[min_vertex].cost + aux->cost;
                //printf("new_cost: %d\tadjacent: %d\n", new_cost, adjacent);
            if(new_cost < vertices[adjacent].cost){
                if(vertices[adjacent].heap_node.key == -1){
                    vertices[adjacent].heap_node.key = new_cost;
                    heapInsert(root, &(vertices[adjacent].heap_node));
                }
                else{
                    heapDecreaseKey(root, &(vertices[adjacent].heap_node), new_cost);
                }
                vertices[adjacent].predecessor = min_vertex;
                vertices[adjacent].cost = new_cost;
                //printGraph(vertices, n_ver);
            }
            aux = aux->next;
        }
    }
}

void resetaGrafo(vertex *vertices, int ver){
    int i;
    vertices[1].predecessor = 1;
    vertices[1].cost = 0;
    for(i=2; i<ver; i++) {
        vertices[i].predecessor = -1;
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
        vertices[dest].predecessor = ori;
        vertices[dest].heap_node.key = vertices[dest].cost;
        heapInsert(root, &(vertices[dest].heap_node));
        dijkstra(vertices, root);
    }
}

void afetados(vertex *vertices, edge **inv, heap *root, int ver, int afetado) {
    vertices[afetado].cost = INF;
    vertices[afetado].predecessor = -1;

    edge *aux;
    aux = vertices[afetado].adjacent;
    while(aux){
        if(vertices[aux->head_vertex].predecessor == afetado)
            afetados(vertices, inv, root, ver, aux->head_vertex);
        aux = aux->next;
    }
    aux = inv[afetado];
    while(aux){
        if(vertices[aux->head_vertex].cost != INF){
            if(vertices[aux->head_vertex].heap_node.key == -1){
                vertices[aux->head_vertex].heap_node.key = vertices[aux->head_vertex].cost;
                heapInsert(root, &(vertices[aux->head_vertex].heap_node));
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
    if(vertices[dest].predecessor == ori){
        afetados(vertices, inv, root, ver, dest);
        dijkstra(vertices, root);
    }
}

#define SIZE_G 7

int main(){

    int ori, dest, custo;
    heap *root = heapCreateRoot();
    char opt;

    vertex *vertices = (vertex*)calloc(SIZE_G, sizeof(vertex));
    edge **inv = (edge**)calloc(SIZE_G, sizeof(edge*));
    initGraph(vertices, SIZE_G);

    vertices[0].heap_node.key = 0;
    heapInsert(root, &(vertices[0].heap_node));

    inserir(vertices, inv, root, SIZE_G, 0, 1, 1);
    inserir(vertices, inv, root, SIZE_G, 0, 5, 1);
    inserir(vertices, inv, root, SIZE_G, 1, 2, 2);
    inserir(vertices, inv, root, SIZE_G, 1, 6, 6);
    inserir(vertices, inv, root, SIZE_G, 2, 3, 2);
    inserir(vertices, inv, root, SIZE_G, 3, 6, 4);
    inserir(vertices, inv, root, SIZE_G, 3, 4, 1);
    inserir(vertices, inv, root, SIZE_G, 4, 5, 2);
    inserir(vertices, inv, root, SIZE_G, 6, 5, 3);

    g_print_graph(vertices, SIZE_G);
    remover(vertices, inv, root, SIZE_G, 0, 1);

    g_print_graph(vertices, SIZE_G);

    inserir(vertices, inv, root, SIZE_G, 5, 1, 1);

    g_print_graph(vertices, SIZE_G);

    freeGraph(vertices, SIZE_G);
    freeInv(inv, SIZE_G);
    free(vertices);
    free(inv);
    free(root);
    return 0;
}
