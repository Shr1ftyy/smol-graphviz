#pragma once

#include "graph.h"
#include "raylib.h"

#include <stdlib.h>
#include <stdio.h>

// TODO: rewrite graphics rendering module to accomodate for new node representation
typedef struct node_representation
{
    node* node_ptr;
    Vector2 position;
} node_representation;

// edge representation for rendering edge
typedef struct edge_representation
{
    edge* edge_ptr;
    node_representation* start;
    node_representation* end;
} edge_representation;

// an array of node representations
typedef struct node_rep_array
{
    node_representation** nodes;
    size_t size;
} node_rep_array;

// an array of edge representations
typedef struct edge_rep_array
{
    edge_representation** edges;
    size_t size;
} edge_rep_array;

// graphics manager, stores data for updating, update rate, edge reps., etc.
typedef struct manager
{
    Vector2 screenDims;
    float time_since_last_update;
    float animation_rate;
    edge_rep_array* edge_reps;
    node_rep_array* node_reps;
} manager;

// get a random position vector
Vector2 getRandomPositionBetween(float _xmin, float _xmax, float _ymin, float _ymax)
{
    Vector2 pos;
    pos.x = _xmin + ((float)rand()/RAND_MAX) * (_xmax - _xmin);
    pos.y = _ymin + ((float)rand()/RAND_MAX) * (_ymax - _ymin);
    
    return pos;
}

// create and return a pointer to a new node representation
node_representation* node_representation_new(node* _node, Vector2 _pos)
{
    node_representation* node_rep = (node_representation*)malloc(sizeof(node_representation));
    node_rep->node_ptr = _node;
    node_rep->position = _pos;
    
    return node_rep;
}

void del_node_rep(node_representation* _n)
{
    if (_n != NULL)
    {
        del_node(_n->node_ptr);
        free(_n);
        _n->node_ptr = NULL;
        _n = NULL;
    }
}

// create and return a pointer to an array of node representations
node_rep_array* node_rep_array_new()
{
    node_rep_array* arr = (node_rep_array*)malloc(sizeof(node_rep_array));
    arr->nodes = (node_representation**)malloc(sizeof(node_representation*));
    arr->size = 0;
    
    return arr;
}

void del_node_rep_array(node_rep_array* _arr)
{
    free(_arr->nodes);
    free(_arr);
    _arr = NULL;
}

void resize_node_rep_array(node_rep_array* arr, size_t _size)
{
    if (arr == NULL)
    {
        return;
    }
    
    if (arr->size <= 0)
    {
        arr->nodes = (node_representation**)malloc(sizeof(node_representation*) * _size);
        arr->size = _size;
        return;
    }
    
    arr->nodes = (node_representation**)realloc(arr->nodes, sizeof(node_representation*) * _size);
    arr->size = _size;
}

// append to an array of node representations
bool append_to_node_reps(node_rep_array* _node_reps, node* _node, Vector2 _pos)
{
    if(_node == NULL)
    {
        return false;
    }
    
    node_representation* node_rep = node_representation_new(_node, _pos);
    
    resize_node_rep_array(_node_reps, _node_reps->size+1);
    _node_reps->nodes[_node_reps->size-1] = node_rep;
    
    return true;
}

bool remove_from_node_reps(node_rep_array* _rep_array, node_representation* _to_remove)
{
    // get index of the node in the node array
    int pos;
    for(pos=0; pos<_rep_array->size; pos++)
    {
        if(_rep_array->nodes[pos] != _to_remove) continue;
        
        // free it when found
        free(_to_remove);
        _rep_array->size--;
        
        // resize and reposition elements in dynamic array
        for(pos=pos; pos<_rep_array->size; pos++)
        {

            _rep_array->nodes[pos] = _rep_array->nodes[pos+1];
        }
        
        resize_node_rep_array(_rep_array, _rep_array->size);
        
        return true;
    }
    
    return false;
}

// create and return a pointer to a new edge representation
edge_representation* edge_representation_new(edge* _edge, node_representation* _start, node_representation* _end)
{
    edge_representation* edge_rep = (edge_representation*)malloc(sizeof(edge_representation));
    edge_rep->edge_ptr = _edge;
    edge_rep->start = _start;
    edge_rep->end = _end;
    
    return edge_rep;
}

void del_edge_rep(edge_representation* _e)
{
    if (_e != NULL)
    {
        del_edge(_e->edge_ptr);
        if (_e->start)
        {
            del_node_rep(_e->start);
            _e->start = NULL;
        }
        if (_e->end)
        {
            del_node_rep(_e->end);
            _e->end = NULL;
        }
        free(_e);
        _e = NULL;
    }
}

edge_rep_array* edge_rep_array_new()
{
    edge_rep_array* arr = (edge_rep_array*)malloc(sizeof(edge_rep_array));
    arr->edges = (edge_representation**)malloc(sizeof(edge_representation*));
    arr->size = 0;
    
    return arr;
}

void del_edge_rep_array(edge_rep_array* _arr)
{
    free(_arr->edges);
    free(_arr);
    _arr = NULL;
}

void resize_edge_rep_array(edge_rep_array* arr, size_t _size)
{
    if(arr == NULL)
    {
        return;
    }
    
    if (arr->size <= 0)
    {
        arr->edges = (edge_representation**)malloc(sizeof(edge_representation*) * _size);
        arr->size = _size;
        return;
    }
    
    arr->edges = (edge_representation**)realloc(arr->edges, sizeof(edge_representation*) * _size);
    arr->size = _size;
}

// append to an array of edge representations
bool append_to_edge_reps(edge_rep_array* _edge_reps, edge* _edge, node_representation* _start, node_representation* _end)
{
    if(_edge == NULL)
    {
        return false;
    }
    
    edge_representation* edge_rep = edge_representation_new(_edge,  _start, _end);
    
    resize_edge_rep_array(_edge_reps, _edge_reps->size+1);
    _edge_reps->edges[_edge_reps->size-1] = edge_rep;
    
    return true;
}

bool remove_from_edge_reps(edge_rep_array* _rep_array, edge_representation* _to_remove)
{
    // get index of the node in the node array
    int pos;
    for(pos=0; pos<_rep_array->size; pos++)
    {
        if(_rep_array->edges[pos] != _to_remove) continue;
        
        // free it when found
        free(_to_remove);
        _rep_array->size--;

        
        // resize and reposition elements in dynamic array
        for(pos=pos; pos<_rep_array->size; pos++)
        {

            _rep_array->edges[pos] = _rep_array->edges[pos+1];
        }
        
        resize_edge_rep_array(_rep_array, _rep_array->size);
        
        return true;
    }
    
    return false;
}

manager* manager_new(graph* _graph, Vector2 _screenDims)
{    
    dynamic_edge_array* graph_edges = _graph->edges;
    dynamic_node_array* graph_nodes = _graph->nodes;
    
    manager* mgr = (manager*)malloc(sizeof(manager));
    mgr->screenDims = _screenDims;
    mgr->edge_reps = edge_rep_array_new();
    mgr->node_reps = node_rep_array_new();
    
    for(int i=0; i<graph_nodes->size; i++)
    {
        node* n = graph_nodes->nodes[i];
        Vector2 pos = getRandomPositionBetween(0, _screenDims.x, 0, _screenDims.y);
        
        append_to_node_reps(mgr->node_reps, n, pos);
        // printf("updated node array size: %zu\n", mgr->node_reps->size);
    }
    
    // TODO: This is currently O(n^2), improve on this later
    for(int i=0; i<graph_edges->size; i++)
    {
        edge* e = graph_edges->edges[i];
        node_representation* start = NULL;
        node_representation* end = NULL;
        
        for(int j=0; j<mgr->node_reps->size; j++)
        {
            node_representation* n_rep = mgr->node_reps->nodes[j];
            if (e->start == n_rep->node_ptr)
            {
                start = n_rep;
            }
            else if (e->end == n_rep->node_ptr)
            {
                end = n_rep;
            }
        }
        
        append_to_edge_reps(mgr->edge_reps, e, start, end);
        // printf("updated edge array size: %zu\n", mgr->edge_reps->size);
    }
    
    return mgr;
}


void graphics_draw(manager* _manager, float dt)
{
    // check if any nodes need to be deleted
    for(int i=0; i<_manager->node_reps->size; i++)
    {
        node_representation* node = _manager->node_reps->nodes[i];
        Vector2 mouse_pos = GetMousePosition();
        bool selected = CheckCollisionPointCircle(node->position, mouse_pos, 20.0);
        if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) && selected)
        {
            // delete the node from the graph
            remove_from_node_reps(_manager->node_reps, node);
            // delete all edges which contain the graph;'
            
            for(int j=0; j<_manager->edge_reps->size; j++)
            {
                edge_representation* edge = _manager->edge_reps->edges[j];
                if(edge->start == node || edge->end == node)
                {
                    remove_from_edge_reps(_manager->edge_reps, edge);
                }
            }

            // printf("updated nodes size: %zu\n", _manager->node_reps->size);
            // printf("updated edges size: %zu\n", _manager->edge_reps->size);
        }
    }
    
    for(int i=0; i<_manager->node_reps->size; i++)
    {
        node_representation* node_rep = _manager->node_reps->nodes[i];
        // TODO: make size of node graph non-constant
        Vector2 mouse_pos = GetMousePosition();
        bool selected = CheckCollisionPointCircle(node_rep->position, mouse_pos, 20.0);
        
        if ((IsMouseButtonDown(MOUSE_BUTTON_LEFT)) && selected)
        {
            node_rep->position = mouse_pos;
        }
        
        DrawCircleLines(node_rep->position.x, node_rep->position.y, 20.0, RED);
        char txt[100];
        char fmt[] = "%.2f";
        snprintf(txt, 100, fmt, node_rep->node_ptr->value);
        DrawText(txt, node_rep->position.x, node_rep->position.y, 20, WHITE);
    }

    
    for(int j=0; j<_manager->edge_reps->size; j++)
    {
        edge_representation* edge_rep = _manager->edge_reps->edges[j];
        if (edge_rep == NULL)
        {
            continue;
        }
        if (edge_rep->start == NULL || edge_rep->end == NULL)
        {
            continue;
        }
        DrawLineV(edge_rep->start->position, edge_rep->end->position, GREEN);
        //printf("{%f, %f}, {%f, %f}\n", edge_rep->start->position, edge_rep->end->position);
    }
}
