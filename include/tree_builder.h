#ifndef TREE_BUILDER_H
#define TREE_BUILDER_H

#include "tree_struct.h"
#include "lexical_analysis.h"

void SyntaxError(size_t p, Tokens* command, const char* error_text);

Node_t* GetAssignment(Tree_t* tree, size_t* p, Tokens* command);
Node_t* GetG(Tree_t* tree, size_t* p, Tokens* command);
Node_t* GetAddSub(Tree_t* tree, size_t* p, Tokens* command); // add and subtract
Node_t* GetMulDiv(Tree_t* tree, size_t* p, Tokens* command); // multiply and divide
Node_t* GetOperation(Tree_t* tree, size_t* p, Tokens* command); // unary operations
Node_t* GetPrExp(Tree_t* tree, size_t* p, Tokens* command); // primary expression
Node_t* GetPow(Tree_t* tree, size_t* p, Tokens* command); // degree
Node_t* GetNumber(Tree_t* tree, size_t* p, Tokens* command); // number
Node_t* GetVariable(Tree_t* tree, size_t* p, Tokens* command); // variable

#endif
