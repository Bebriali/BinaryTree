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
    ADD      = 0,   // +
    SUB      = 1,   // -
    MUL      = 2,   // *
    DIV      = 3,   // /
    POW      = 4,   // ^
    SIN      = 5,   // sin
    COS      = 6,   // cos
    TAN      = 7,   // tan/tg
    CTG      = 8,   // ctg
    LOG      = 9,   // ln/log
    L_PR_EXP = 10,  // (
    R_PR_EXP = 11,  // )
    L_CURL   = 12,  // {
    R_CURL   = 13,  // }
    AST      = 14,  // =
    IF       = 15,  // if
    WHILE    = 16,  // while
    FOR      = 17,  // for
    MORE     = 18,  // >
    LESS     = 19,  // >
    EQ       = 20,  // ==
    NOT_EQ   = 21,  // !=
    BTM_EQ   = 22,  // <=
    TOP_EQ   = 23,  // >=
    SEM      = 24,  // ;
    EOT      = 25,  // $
    ERR      = 26   //
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
bool ComparativeOperation(Operation op);

#endif
