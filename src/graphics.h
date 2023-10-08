#pragma once

#include "graph.h"
#include "raylib.h"

#include <stdlib.h>
#include <stdio.h>

// node representation for rendering node
typedef struct node_representation
{
    node* node_ptr;
    Vector2 position;
} node_representation;

// an array of node representations
typedef struct node_rep_array
{
    node_representation** nodes;
    size_t size;
} node_rep_array;

// graphics manager, stores data for updating, update rate, node reps., etc.
typedef struct manager
{
    Vector2 screenDims;
    float time_since_last_update;
    float animation_rate;
    node_rep_array node_reps;
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
}

// append to an array of node representations
bool append_to_node_reps(node_rep_array* _node_reps, node* _node, Vector2 _pos)
{
    if(_node == NULL)
    {
        return false;
    }
    
    node_representation* node_rep = node_representation_new(_node, _pos);
    
    _node_reps->nodes[_node_reps->size] = node_rep;
    _node_reps->size++;
    
    return true;
}

bool remove_from_node_reps(node_rep_array* _rep_array, node* _node)
{ }

//manager* manager_new(graph* _graph, Vector2 _screenDims)
//{

//node_reps = (node_rep_array*)malloc(sizeof(node_rep_array));
//dynamic_node_array* graph_nodes = _graph->node_array;
//manager* mgr = (manager*)malloc(sizeof(manager));
//manager->screenDims = _screenDims;

//for(int i=0; i<graph_nodes->size; i++)
//{
//node* n = graph_nodes->nodes[i];
//Vector2 pos = getRandomPositionBetween(0, _screenDims.x, 0, _screenDims.y);

//node_representation* node_rep = node_representation_new(n, pos);
//}
//}


void graphics_draw(manager* _manager, graph* _graph, float dt)
{ }