#ifndef TREE_FUNC_H
#define TREE_FUNC_H

#include "tree_struct.h"

void UserInsert(Tree_t* tree);

void GenInsert(Tree_t* tree);

Node_t* ReadInfInsert(Node_t* node);

Node_t* ReadInsert(Tree_t* tree, Node_t* node);
union Data GetNodeData(char* command, void* value, NodeType* type);

#endif
