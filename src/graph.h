#pragma once

#include <stdbool.h>
#include <stdlib.h>

typedef struct node
{
    float value;
    bool searched;
} node;

typedef struct edge
{
    float weight;
    node* start;
    node* end;
} edge;

typedef struct dynamic_node_array
{
    node** nodes;
    size_t size;
} dynamic_node_array;

typedef struct dynamic_edge_array
{
    edge** edges;
    size_t size;
} dynamic_edge_array;

typedef struct graph
{
    dynamic_edge_array* edges;
    dynamic_node_array* nodes;
    bool directed;
} graph;

node* node_new(float _value)
{
    node* n = (node*)malloc(sizeof(node));
    n->value = _value;
    n->searched = false;
    
    return n;
}

void del_node(node* _node)
{
    if (_node != NULL)
    {
        _node->searched = false;
        _node->value = 0;
        printf("freeing\n");
        free(_node);
        printf("freed\n");
        _node = NULL;
        printf("set to null\n");
    }
}

dynamic_node_array* new_node_array()
{
    dynamic_node_array* arr = (dynamic_node_array*)malloc(sizeof(dynamic_node_array));
    arr->size = 0;
    return arr;
}

void del_node_array(dynamic_node_array* _array)
{
    if(_array != NULL) free(_array->nodes);
    free(_array);
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

edge* edge_new(float _weight, node* _start, node* _end)
{
    edge* e = (edge*)malloc(sizeof(edge));
    e->weight = _weight;
    e->start = _start;
    e->end = _end;
    
    return e;
}

void del_edge(edge* _edge)
{
    if (_edge != NULL)
    {
        if (_edge->start != NULL)
        {
            del_node(_edge->start);
            _edge->start = NULL;
        }
        if (_edge->end != NULL)
        {
            del_node(_edge->end);
            _edge->end = NULL;
        }

        free(_edge);
        _edge = NULL;
    }
}

dynamic_edge_array* new_edge_array()
{
    dynamic_edge_array* arr = (dynamic_edge_array*)malloc(sizeof(dynamic_edge_array));
    arr->size = 0;
    return arr;
}

void del_edge_array(dynamic_edge_array* _array)
{
    if(_array->edges != NULL) free(_array->edges);
    if(_array != NULL) free(_array);
}

dynamic_edge_array* reserve_new_edge_array(size_t _size)
{
    dynamic_edge_array* arr = (dynamic_edge_array*)malloc(sizeof(dynamic_edge_array) * _size);
    arr->edges =  (edge**)malloc(sizeof(edge*) * _size);
    arr->size = 0;
    return arr;
}

void resize_edge_array(dynamic_edge_array* _array, size_t _size)
{
    if (_array->size == 0)
    {
        _array->edges = (edge**)malloc(_size * sizeof(edge*));
    }
    else
    {
        _array->edges = (edge**)realloc(_array->edges, _size * sizeof(edge*));
    }
    
    _array->size = _size;
}

void append_edge_array(dynamic_edge_array* _n, edge* _new_edge, size_t _n_edges)
{
    resize_edge_array(_n, (_n_edges + 1));
    _n->edges[_n_edges] = _new_edge;
}

bool remove_edge_from_array(dynamic_edge_array* _n, edge* _to_remove)
{
    // get index of the edge in the edge array
    int pos;
    for(pos=0; pos<_n->size; pos++)
    {
        if(_n->edges[pos] != _to_remove) continue;
        
        // free edge  when found
        del_edge(_to_remove);
        _n->size--;
        
        // resize and reposition elements in dynamic array
        for(; pos<_n->size; pos++)
        {
            _n->edges[pos] = _n->edges[pos+1];
        }
        
        resize_edge_array(_n, sizeof(edge*)*_n->size);
        
        return true;
    }
    
    return false;
}

graph* graph_new(bool _directed)
{
    graph* g = (graph*)malloc(sizeof(graph));
    g->edges = new_edge_array();
    g->nodes = new_node_array();
    g->directed = _directed;
    
    return g;
}

void del_graph(graph* _graph)
{
    del_edge_array(_graph->edges);
    del_node_array(_graph->nodes);
    free(_graph);
}

edge* graph_search_edge(graph* _graph, float _weight)
{
    for(size_t i=0; i<_graph->edges->size; i++)
    {
        edge* e = _graph->edges->edges[i];
        if(e->weight == _weight)
        {
            return e;
        }
    }
    
    return NULL;
}

node* graph_search_node(graph* _graph, float _value)
{
    for(size_t i=0; i<_graph->edges->size; i++)
    {
        
        edge* e = _graph->edges->edges[i];
        if(e->start->value == _value)
        {
            return e->start;
        }
        if(e->end->value == _value)
        {
            return e->end;
        }
    }
    
    return NULL;
}

void append_edge_ptr_graph(graph* _graph, edge* _e)
{
    append_edge_array(_graph->edges, _e, _graph->edges->size);
}

void append_edge_to_graph(graph* _graph, float _weight, node* _start, node* _end)
{
    edge* e = edge_new(_weight, _start, _end);
    append_edge_ptr_graph(_graph, e);
}

void append_node_ptr_graph(graph* _graph, node* _n)
{
    append_node_array(_graph->nodes, _n, _graph->nodes->size);
}

void append_node_to_graph(graph* _graph, float _value)
{
    node* n = node_new(_value);
    append_node_ptr_graph(_graph, n);
}

bool remove_edge(graph* _graph, edge* _edge)
{
    if(_edge == NULL)
    {
        return false;
    }
    
    for(int i=0; i<_graph->edges->size; i++)
    {
        edge* to_remove = _graph->edges->edges[i];
        
        if(to_remove == _edge)
        {
            remove_edge_from_array(_graph->edges, _edge);
        }
    }
    
    return true;
}

void reset_graph(graph* _graph)
{
    for(int i=0; i<_graph->edges->size; i++)
    {
        edge* e = _graph->edges->edges[i];
        e->start->searched = false;
        e->end->searched = false;
    }
}