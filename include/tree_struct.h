#ifndef TREE_STRUCT_H
#define TREE_STRUCT_H

#include "error_keys.h"

const int MAX_VAR_NUM = 10;

enum NodeType
{
    NUM = 0,
    VAR = 1,
    OPN = 2
};

struct Variable
{
    char var;
    int var_code;
};

enum Operation
{
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3,
    SIN = 4,
    COS = 5,
    TAN = 6,
    CTG = 7,
    LOG = 8
};

struct Node_t
{
    enum NodeType type;
    int data;
    Node_t* left;
    Node_t* right;
};

struct Tree_t
{
    Node_t* root;
    size_t size;
    FILE* log;
    size_t dump_num;
    Variable var_list[MAX_VAR_NUM];
};

void TreeCtor(Tree_t* tree);
ErrorKeys TreeDtor(Tree_t* tree);

Node_t* CreateNode(Tree_t* tree, int value, NodeType type, Node_t* left, Node_t* right);
void NodeDtor(Node_t** node);

Node_t* Insert (Tree_t* tree, Node_t* node, int value, NodeType type);
void Print(Node_t* node);

const char* DefineOperation(int operation);

#endif
