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

typedef enum Registers
{
    RAX,
    RBX,
    RCX,
    RDX,
    RSI,
    RDI,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    ERR_REG
} regs_t;

typedef struct NameTable
{
    regs_t reg;
    int offset;
    char name[32] = {0};
} name_t;

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

    name_t** name_table;
};

name_t** NT_Ctor(void);
void NT_Dtor(name_t** name_table);
int NT_FindElem(name_t** name_table, char* elem);
void NT_PutElem(name_t** name_table, char* elem);

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
