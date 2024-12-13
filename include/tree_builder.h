#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H

#include "tree_struct.h"

void SyntaxError(size_t p, char* s);

Node_t* GetG(Tree_t* tree, size_t* p, char* s);
Node_t* GetE(Tree_t* tree, size_t* p, char* s); // add and subtract
Node_t* GetT(Tree_t* tree, size_t* p, char* s); // multiply and divide
Node_t* GetO(Tree_t* tree, size_t* p, char* s); // unary operations
Node_t* GetP(Tree_t* tree, size_t* p, char* s); // primary expression
Node_t* GetD(Tree_t* tree, size_t* p, char* s); // degree
Node_t* GetN(Tree_t* tree, size_t* p, char* s); // number
Node_t* GetV(Tree_t* tree, size_t* p, char* s); // variable

#endif
