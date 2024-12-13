#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "tree_struct.h"

#define _NUM(arg) CreateNode(tree, arg, NUM, NULL, NULL)
#define _VAR(arg) CreateNode(tree, arg, VAR, NULL, NULL)

#define _ADD(arg1, arg2) CreateNode(tree, ADD, OP, arg1, arg2)
#define _SUB(arg1, arg2) CreateNode(tree, SUB, OP, arg1, arg2)
#define _MUL(arg1, arg2) CreateNode(tree, MUL, OP, arg1, arg2)
#define _DIV(arg1, arg2) CreateNode(tree, DIV, OP, arg1, arg2)
#define _POW(arg1, arg2) CreateNode(tree, POW, OP, arg1, arg2)
#define _SIN(arg)        CreateNode(tree, SIN, OP, arg,  NULL)
#define _COS(arg)        CreateNode(tree, COS, OP, arg,  NULL)
#define _TAN(arg)        CreateNode(tree, TAN, OP, arg,  NULL)
#define _CTG(arg)        CreateNode(tree, CTG, OP, arg,  NULL)
#define _LOG(arg)        CreateNode(tree, LOG, OP, arg,  NULL)

#define CopyNode  Copy(tree, node)
#define CopyLeft  Copy(tree, node->left)
#define CopyRight Copy(tree, node->right)
#define DiffLeft  Diff(tree, node->left)
#define DiffRight Diff(tree, node->right)


Node_t* Diff(Tree_t* tree, Node_t* node);
Node_t* Copy(Tree_t* tree, Node_t* node);

Node_t* Simplify(Tree_t* tree, Node_t* node);

#endif
