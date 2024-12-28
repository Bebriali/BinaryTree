#ifndef DIFFERENTIATOR_H
#define DIFFERENTIATOR_H

#include "tree_struct.h"

union Data IntToData(int arg);
union Data VarToData(char* arg);
union Data FltToData(double arg);
union Data OpToData(Operation arg);

#define _NUM(node, arg) (node = CreateNode(NULL, IntToData(arg), NUM, NULL, NULL))
#define _VAR(node, arg) (node = CreateNode(NULL, VarToData(arg), VAR, NULL, NULL))
#define _OP(node, arg)  (node = CreateNode(NULL, OpToData(arg),  OP , NULL, NULL))

#define _ADD(arg1, arg2) CreateNode(tree, OpToData(ADD), OP, arg1, arg2)
#define _SUB(arg1, arg2) CreateNode(tree, OpToData(SUB), OP, arg1, arg2)
#define _MUL(arg1, arg2) CreateNode(tree, OpToData(MUL), OP, arg1, arg2)
#define _DIV(arg1, arg2) CreateNode(tree, OpToData(DIV), OP, arg1, arg2)
#define _POW(arg1, arg2) CreateNode(tree, OpToData(POW), OP, arg1, arg2)
#define _SIN(arg)        CreateNode(tree, OpToData(SIN), OP, arg,  NULL)
#define _COS(arg)        CreateNode(tree, OpToData(COS), OP, arg,  NULL)
#define _TAN(arg)        CreateNode(tree, OpToData(TAN), OP, arg,  NULL)
#define _CTG(arg)        CreateNode(tree, OpToData(CTG), OP, arg,  NULL)
#define _LOG(arg)        CreateNode(tree, OpToData(LOG), OP, arg,  NULL)
#define _SEM(arg1, arg2) CreateNode(tree, OpToData(SEM), OP, arg1, arg2)

#define CopyNode  Copy(tree, node)
#define CopyLeft  Copy(tree, node->left)
#define CopyRight Copy(tree, node->right)
#define DiffLeft  Diff(tree, node->left)
#define DiffRight Diff(tree, node->right)


Node_t* Diff(Tree_t* tree, Node_t* node);
Node_t* Copy(Tree_t* tree, Node_t* node);

Node_t* Simplify(Tree_t* tree, Node_t* node);

Node_t* SimplifyOperation(Tree_t* tree, Node_t* node);

Node_t* AddSimp(Tree_t* tree, Node_t* node);
Node_t* SubSimp(Tree_t* tree, Node_t* node);
Node_t* MulSimp(Tree_t* tree, Node_t* node);
Node_t* DivSimp(Tree_t* tree, Node_t* node);
Node_t* PowSimp(Tree_t* tree, Node_t* node);

#endif
