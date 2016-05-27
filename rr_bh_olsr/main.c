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
//     free(queue);
//
//     return 0;
// }

#include "rr.h"
#define SIZE_G 10


int main(void)
{


    vertex * g = g_create_graph(SIZE_G);

    ((vtx_node*)g[0].h_node.key)->cost = 0;                                                     // Define origem
    g[0].pi = -2;                                                     // Define origem
    heap * queue = heap_new();


    // g_insert_edge(g, 0, 1, 9);
    // g_insert_edge(g, 0, 2, 6);
    // g_insert_edge(g, 0, 3, 4);
    // g_insert_edge(g, 0, 4, 7);
    // g_insert_edge(g, 0, 5, 5);
    // g_insert_edge(g, 0, 6, 2);
    // g_insert_edge(g, 0, 7, 4);
    g_insert_edge(g, 0, 8, 2);
    // g_insert_edge(g, 0, 9, 3);

    // g_insert_edge(g, 1, 0, 5);
    // g_insert_edge(g, 1, 2, 6);
    // g_insert_edge(g, 1, 3, 4);
    // g_insert_edge(g, 1, 4, 2);
    // g_insert_edge(g, 1, 5, 5);
    // g_insert_edge(g, 1, 6, 2);
    // g_insert_edge(g, 1, 7, 1);
    // g_insert_edge(g, 1, 8, 6);
    // g_insert_edge(g, 1, 9, 7);

    // g_insert_edge(g, 2, 0, 1);
    // g_insert_edge(g, 2, 1, 5);
    // g_insert_edge(g, 2, 3, 6);
    // g_insert_edge(g, 2, 4, 8);
    // g_insert_edge(g, 2, 5, 2);
    // g_insert_edge(g, 2, 6, 1);
    // g_insert_edge(g, 2, 7, 4);
    // g_insert_edge(g, 2, 8, 1);
    // g_insert_edge(g, 2, 9, 8);


    // g_insert_edge(g, 3, 0, 5);
    // g_insert_edge(g, 3, 1, 8);
    // g_insert_edge(g, 3, 2, 9);
    // g_insert_edge(g, 3, 4, 4);
    // g_insert_edge(g, 3, 5, 6);
    // g_insert_edge(g, 3, 6, 1);
    // g_insert_edge(g, 3, 7, 2);
    // g_insert_edge(g, 3, 8, 1);
    // g_insert_edge(g, 3, 9, 2);

    // g_insert_edge(g, 4, 0, 9);
    // g_insert_edge(g, 4, 1, 8);
    // g_insert_edge(g, 4, 2, 5);
    // g_insert_edge(g, 4, 3, 8);
    // g_insert_edge(g, 4, 6, 3);
    // g_insert_edge(g, 4, 7, 5);
    // g_insert_edge(g, 4, 9, 2);

    // g_insert_edge(g, 5, 0, 7);
    // g_insert_edge(g, 5, 1, 6);
    // g_insert_edge(g, 5, 2, 6);
    // g_insert_edge(g, 5, 3, 8);
    g_insert_edge(g, 5, 4, 1);
    // g_insert_edge(g, 5, 5, 5);
    // g_insert_edge(g, 5, 6, 2);
    // g_insert_edge(g, 5, 7, 4);
    // g_insert_edge(g, 5, 8, 3);
    // g_insert_edge(g, 5, 9, 3);

    // g_insert_edge(g, 6, 0, 9);
    // g_insert_edge(g, 6, 1, 6);
    // g_insert_edge(g, 6, 2, 5);
    // g_insert_edge(g, 6, 3, 6);
    // g_insert_edge(g, 6, 4, 3);
    // g_insert_edge(g, 6, 5, 5);
    // g_insert_edge(g, 6, 7, 2);
    // g_insert_edge(g, 6, 8, 6);
    // g_insert_edge(g, 6, 9, 8);

    // g_insert_edge(g, 7, 0, 9);
    // g_insert_edge(g, 7, 1, 2);
    // g_insert_edge(g, 7, 2, 5);
    // g_insert_edge(g, 7, 3, 6);
    // g_insert_edge(g, 7, 4, 3);
    // g_insert_edge(g, 7, 5, 8);
    // g_insert_edge(g, 7, 6, 5);
    // g_insert_edge(g, 7, 8, 2);
    // g_insert_edge(g, 7, 9, 9);

    // g_insert_edge(g, 8, 0, 6);
    // g_insert_edge(g, 8, 1, 8);
    // g_insert_edge(g, 8, 2, 9);
    // g_insert_edge(g, 8, 3, 7);
    g_insert_edge(g, 8, 4, 4);
    g_insert_edge(g, 8, 5, 1);
    // g_insert_edge(g, 8, 6, 5);
    g_insert_edge(g, 8, 7, 1);
    // g_insert_edge(g, 8, 9, 2);

    // g_insert_edge(g, 9, 0, 3);
    // g_insert_edge(g, 9, 1, 9);
    // g_insert_edge(g, 9, 2, 2);
    // g_insert_edge(g, 9, 3, 3);
    // g_insert_edge(g, 9, 4, 4);
    // g_insert_edge(g, 9, 5, 5);
    // g_insert_edge(g, 9, 6, 6);
    // g_insert_edge(g, 9, 7, 7);
    // g_insert_edge(g, 9, 8, 4);

//    rr_add_edge(g, 0, 1, 1, queue);
//    rr_add_edge(g, 0, 5, 1, queue);
//    rr_add_edge(g, 1, 2, 2, queue);
//    rr_add_edge(g, 1, 6, 6, queue);
//    rr_add_edge(g, 2, 3, 2, queue);
//    rr_add_edge(g, 3, 6, 4, queue);
//    rr_add_edge(g, 3, 4, 1, queue);
//    rr_add_edge(g, 4, 5, 2, queue);
//    rr_add_edge(g, 6, 5, 3, queue);

    //g_print_graph(g, SIZE_G);
    heap_insert(&g[0].h_node, queue);
    rr_recalculate_shortest_path(g, queue);
    g_print_graph(g, SIZE_G);

//    rr_remove_edge(g, 0, 1, queue);
//    g_print_graph(g, SIZE_G);

//    rr_add_edge(g, 5, 1, 1, queue);
//    g_print_graph(g, SIZE_G);

    g_free_graph(&g, SIZE_G);
    g_print_graph(g, SIZE_G);

    return 0;
}
