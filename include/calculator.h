#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "tree_struct.h"

void SyntaxError(size_t p, char* s);

Node_t* GetG(Tree_t* tree, size_t* p, char* s);
Node_t* GetE(Tree_t* tree, size_t* p, char* s);
Node_t* GetT(Tree_t* tree, size_t* p, char* s);
Node_t* GetP(Tree_t* tree, size_t* p, char* s);
Node_t* GetN(Tree_t* tree, size_t* p, char* s);
Node_t* GetV(Tree_t* tree, size_t* p, char* s);

#endif
