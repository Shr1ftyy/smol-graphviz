#include "graph.h" 

#include <stdio.h>

int main()
{
    graph* g = graph_new(false);
    node* n = node_new_params(0.2f, NULL, 0);
    dynamic_node_array* node_arr = reserve_new_array(4);
    node* n1 = node_new_params(0.1f, NULL, 0);
    node* n2 = node_new_params(0.4f, NULL, 0);
    node* n3 = node_new_params(0.7f, NULL, 0);
    node* n4 = node_new_params(0.5f, NULL, 0);
    
    append_node_array(n->connections, n1, n->connections->size);
    append_node_array(n->connections, n2, n->connections->size);
    append_node_array(n->connections, n3, n->connections->size);
    append_node_array(n->connections, n4, n->connections->size);
    
    append_to_graph(g, n);
    
    // check if the connected nodes are present
    for(int i=0; i<n->connections->size; i++)
    {
        printf("%f, ", n->connections->nodes[i]->weight);
    }

    printf("\n----------\n");

    // check if all nodes are present int the graph
    for(int i=0; i<g->node_array->size; i++)
    {
        printf("%f, ", g->node_array->nodes[i]->weight);
    }
    
    return 0;
}