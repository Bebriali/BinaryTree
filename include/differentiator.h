#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "tree_struct.h"

#define _NUM(arg) CreateNode(tree, arg, NUM, NULL, NULL)
#define _VAR(arg) CreateNode(tree, arg, VAR, NULL, NULL)

#define _ADD(arg1, arg2) CreateNode(tree, ADD, OPN, arg1, arg2)
#define _SUB(arg1, arg2) CreateNode(tree, SUB, OPN, arg1, arg2)
#define _MUL(arg1, arg2) CreateNode(tree, MUL, OPN, arg1, arg2)
#define _DIV(arg1, arg2) CreateNode(tree, DIV, OPN, arg1, arg2)


Node_t* Diff(Tree_t* tree, Node_t* node);
Node_t* Copy(Tree_t* tree, Node_t* node);

Node_t* Simplify(Tree_t* tree, Node_t* node);

#endif
