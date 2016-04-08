#define SIZE_G 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "avl.h"
#include "common_types.h"
#include "list.h"

int main(void)
{
    // Teste avl
    int SIZE_G = 10, i;
    heap_node *avl_n;
    heap *queue = heap_new();
    vertex *h, *graph = g_create_graph(SIZE_G);
    if(!graph) return -1;

    for(i = 0; i < SIZE_G; i++)
    {
        ((vtx_node*)graph[i].h_node.key)->cost = i+15;
        heap_insert(&graph[i].h_node, queue);
    }

    heap_print(queue);

    heap_update(&graph[4], 94, 49, queue);

    heap_print(queue);

    avl_n = heap_extract(queue);

    while(avl_n)
    {
        printf("%d\n", ((vtx_node *)avl_n->key)->cost);
        avl_n = heap_extract(queue);
    }

    heap_print(queue);

    //const typeof(((typeof(*g) *)0)->h_node.list ) *__tempptr = queue->list_head.next
    printf("%p\n", avl_n);

    return 0;
}
