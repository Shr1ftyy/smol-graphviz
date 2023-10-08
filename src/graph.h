#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct node
{
    struct dynamic_node_array* connections;
    float weight;
    bool searched;
} node;

typedef struct dynamic_node_array
{
    node** nodes;
    size_t size;
} dynamic_node_array;


typedef struct graph
{
    dynamic_node_array* node_array;
    bool directed;
} graph;

dynamic_node_array* new_array()
{
    dynamic_node_array* arr = (dynamic_node_array*)malloc(sizeof(dynamic_node_array));
    arr->size = 0;
    return arr;
}

dynamic_node_array* reserve_new_array(size_t _size)
{
    dynamic_node_array* arr = (dynamic_node_array*)malloc(sizeof(dynamic_node_array) * _size);
    arr->nodes =  (node**)malloc(sizeof(node*) * _size);
    arr->size = 0;
    return arr;
}

void* resize_node_array(dynamic_node_array* _array, size_t _size)
{
    if (_array->size == 0)
    {
        _array->nodes = (node**)malloc(_size * sizeof(node*));
    }
    else
    {
        _array->nodes = (node**)realloc(_array->nodes, _size * sizeof(node*));
    }

    _array->size = _size;
}

void append_node_array(dynamic_node_array* _n, node* _new_node, size_t _n_nodes)
{
    resize_node_array(_n, (_n_nodes + 1));
    _n->nodes[_n_nodes] = _new_node;
}

bool remove_node_from_array(dynamic_node_array* _n, node* _to_remove)
{
    // get index of the node in the node array
    int pos;
    for(pos=0; pos<_n->size; pos++)
    {
        if(_n->nodes[pos] != _to_remove) continue;
        
        // free it when found
        free(_to_remove);
        _n->size--;
        
        // resize and reposition elements in dynamic array
        for(; pos<_n->size; pos++)
        {
            _n->nodes[pos] = _n->nodes[pos+1];
        }
        
        resize_node_array(_n, sizeof(node*)*_n->size);
        
        return true;
    }
    
    return false;
}

node* node_new()
{
    node* n = (node*)malloc(sizeof(node));
    n->connections = new_array();
    n->searched = false;
    return n;
}

node* node_new_params(float _weight, dynamic_node_array* connected_to, size_t n_connected)
{
    node* n = node_new();
    n->weight = _weight;

    // resize connections array   
    if(n_connected > 0)
    {
        n->connections = resize_node_array(n->connections, n->connections->size + n_connected);
        for(size_t i=0; i<n_connected-1; i++)
        {
            n->connections->nodes[i] = connected_to->nodes[i];
        }
    }  
    
    return n;
}

graph* graph_new(bool _directed)
{
    graph* g = (graph*)malloc(sizeof(graph));
    g->node_array = new_array();
    g->directed = _directed;
    
    return g;
}

node* graph_search(graph* _graph, float _weight)
{
    for(size_t i=0; i<_graph->node_array->size; i++)
    {
        node* n = _graph->node_array->nodes[i];
        if(n->weight == _weight)
        {
            return n;
        }
    }
    
    return NULL;
}

node* graph_search_node(graph* _graph, node* _n)
{
    for(size_t i=0; i<_graph->node_array->size; i++)
    {
        node* n = _graph->node_array->nodes[i];
        if(n == _n)
        {
            return n;
        }
    }
    
    return NULL;
}

void append_to_graph(graph* _graph, node* _n)
{
    append_node_array(_graph->node_array, _n, _graph->node_array->size);
    
    for(int i=0; i<_n->connections->size; i++)
    {   
        // TODO: do not append a node into the graph if it already present in it
        // append all nodes the node is connected to into the graph
        append_node_array(_graph->node_array, _n->connections->nodes[i], _graph->node_array->size);
    }
}

void append_weight_to_graph(graph* _graph, float _weight)
{
    node* n = node_new_params(_weight, NULL, 0);
    append_to_graph(_graph, n);
}



bool remove_node(graph* _graph, double _weight)
{
    node* to_remove = graph_search(_graph, _weight);
    
    if(to_remove == NULL)
    {
        return false;
    }
    
    for(int i=0; i<_graph->node_array->size; i++)
    {
        node* to_update = _graph->node_array->nodes[i];
        for(int j=0; j<to_update->connections->size; j++)
        {
            node* curr_node = to_update->connections->nodes[j];
            if(curr_node == to_remove)
            {
                remove_node_from_array(to_update->connections, curr_node);
            }
        }
    }
    
    return true;
}

void reset_graph(graph* _graph)
{
    for(int i=0; i<_graph->node_array->size; i++)
    {
        node* n = _graph->node_array->nodes[i];
        n->searched = false;
    }
}