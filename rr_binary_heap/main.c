#include "rr.h"

int main(void)
{

    int size_g, size_edg, source, target, cost;
    scanf("%d%d", &size_g, &size_edg);
    if(size_g < 1 || size_edg < 1)
        return -1;

    vertex * g = g_create_graph(size_g);

    g[0].heap_node.cost = 0;                                                     // Define origem
    g[0].pi = -2;                                                                // Define origem

    while(scanf("%d%d%d", &source, &target, &cost) != EOF) {
        rr_add_edge(g, source, target, cost);
    }

    g_print_graph(g, size_g);

    g_free_graph(&g, size_g);

    return 0;
}
