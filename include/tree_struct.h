#ifndef TREE_STRUCT_H
#define TREE_STRUCT_H

#include "error_keys.h"

const int MAX_VAR_NUM = 10;

enum NodeType
{
    NUM   = 0,
    FLT   = 1,
    VAR   = 2,
    OP    = 3,
    ERR_T = 4
};

struct Variable
{
    char var;
    int var_code;
};

enum Operation
{
    ADD      = 0,
    SUB      = 1,
    MUL      = 2,
    DIV      = 3,
    POW      = 4,
    SIN      = 5,
    COS      = 6,
    TAN      = 7,
    CTG      = 8,
    LOG      = 9,
    R_PR_EXP = 11,
    L_PR_EXP = 12,
    AST      = 13,
    SEM      = 14,
    EOT      = 15,
    ERR      = 16
};

union Data
{
    int num;
    double flt;
    char* var;
    Operation op;
};

struct Node_t
{
    enum NodeType type;
    union Data data;
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

void TreeCtor(Tree_t* tree, FILE* log);
ErrorKeys TreeDtor(Tree_t* tree);

Node_t* CreateNode(Tree_t* tree, Data value, NodeType type, Node_t* left, Node_t* right);
void NodeDtor(Node_t** node);

Node_t* Insert (Tree_t* tree, Node_t* node, int value, NodeType type);
void Print(Node_t* node);

const char* DecryptType(NodeType type);
const char* DecryptOperation(Operation operation);
Operation DefineOperation(const char* opn);

#endif
