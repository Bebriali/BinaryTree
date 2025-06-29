#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "tree_struct.h"

void CodeToAsm(Tree_t* tree);
void ConvNode(Node_t* node, FILE* file);

#endif
