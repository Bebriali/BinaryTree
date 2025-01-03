#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include "tree_struct.h"

#define TREE_DUMP(arg1) CreateLog(arg1,  __FILE__, __func__, __LINE__);

ErrorKeys CreateLog(Tree_t* tree, const char* file, const char* func, const int line);

void FormObjects(Node_t* node, FILE* tree_dump, size_t rank);

void ShapeTree(Node_t* node, FILE* tree_dump);

#endif
