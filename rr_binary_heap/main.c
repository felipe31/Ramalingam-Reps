#include "heap.h"
#include "rr.h"
#include "graph.h"
// int main(void)
// {
//
//     int size_g, size_edg, source, target, cost;
//     scanf("%d%d", &size_g, &size_edg);
//     if(size_g < 1 || size_edg < 1)
//         return -1;
//
//     vertex * g = g_create_graph(size_g);
//     heap * queue = heap_new();
//
//     ((vtx_node*)g[0].h_node.key)->cost = 0;                                                     // Define origem
//     g[0].pi = -2;                                                                // Define origem
//
//     while(scanf("%d%d%d", &source, &target, &cost) != EOF) {
//         rr_add_edge(g, source, target, cost, queue);
//     }
//
//     g_print_graph(g, size_g);
//
//     g_free_graph(&g, size_g);
//
//     return 0;
// }
#include "rr.h"
#define SIZE_G 7


int main(void)
{


    vertex * g = g_create_graph(SIZE_G);

    ((vtx_node*)g[0].h_node.key)->cost = 0;                                                     // Define origem
    g[0].pi = -2;                                                     // Define origem
    heap * queue = heap_new();

    rr_add_edge(g, 0, 1, 1, queue);
    rr_add_edge(g, 0, 5, 1, queue);
    rr_add_edge(g, 1, 2, 2, queue);
    rr_add_edge(g, 1, 6, 6, queue);
    rr_add_edge(g, 2, 3, 2, queue);
    rr_add_edge(g, 3, 6, 4, queue);
    rr_add_edge(g, 3, 4, 1, queue);
    rr_add_edge(g, 4, 5, 2, queue);
    rr_add_edge(g, 6, 5, 3, queue);


    g_print_graph(g, SIZE_G);

    rr_remove_edge(g, 0, 1, queue);
    g_print_graph(g, SIZE_G);

    rr_add_edge(g, 5, 1, 1, queue);
    g_print_graph(g, SIZE_G);

    g_free_graph(&g, SIZE_G);
    g_print_graph(g, SIZE_G);

    return 0;
}
