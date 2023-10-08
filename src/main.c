#include "graph.h"
#include "graphics.h" 

#include <stdio.h>

int main()
{
    graph* g = graph_new(false);
    node* n =  node_new(0.2f);
    node* n1 = node_new(0.1f);
    node* n2 = node_new(0.4f);
    node* n3 = node_new(0.7f);
    node* n4 = node_new(0.5f);
    
    edge* e0 = edge_new(0.3, n, n1);
    edge* e1 = edge_new(0.2, n1, n2);
    edge* e2 = edge_new(0.3, n2, n3);
    edge* e3 = edge_new(0.5, n3, n4);
    edge* e4 = edge_new(0.7, n4, n2);
    
    append_to_graph(g, e0);
    append_to_graph(g, e1);
    append_to_graph(g, e2);
    append_to_graph(g, e3);
    append_to_graph(g, e4);
    
    // nodes and edges present in graph
    for(size_t i=0; i<g->edges->size; i++)
    {
        printf("%f -- %f --> %f\n", g->edges->edges[i]->start->value ,g->edges->edges[i]->weight, g->edges->edges[i]->end->value);
    }
    
    return 0;
}