#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "debug_info.h"
#include "tree_struct.h"
#include "differentiator.h"

name_t** NT_Ctor(void)
{
    name_t** name_table = (name_t**) calloc(MAX_VAR_NUM, sizeof(name_t*));
    if (name_table == NULL)
    {
        printf(RED("error in memory allocation for name_table in NT_Ctor\n"));
        return NULL;
    }

    for (int i = 0; i < MAX_VAR_NUM; i++)
    {
        name_table[i] = (name_t*) calloc(1, sizeof(name_t));
        if (name_table[i] == NULL)
        {
            printf(RED("error in memory allocation for name_table[%d] in NT_Ctor\n"), i);
            return NULL;
        }

        name_table[i]->offset = 1;
        name_table[i]->reg    = ERR_REG;
    }

    return name_table;
}

void NT_Dtor(name_t** name_table)
{
    for (int i = 0; i < MAX_VAR_NUM; i++)
    {
        if (name_table[i] != NULL)
        {
            free(name_table[i]);
            name_table[i] = NULL;
        }
    }

    if (name_table != NULL)
    {
        free(name_table);
        name_table = NULL;
    }

    return ;
}
int NT_FindElem(name_t** name_table, char* elem)
{
    for (int i = 0; i < MAX_VAR_NUM; i++)
    {
        if (!strcmp(elem, name_table[i]->name))
        {
            return i;
        }
    }

    return -1;
}

void NT_PutElem(name_t** name_table, char* elem)
{
    for (int i = 0; i < MAX_VAR_NUM; i++)
    {
        if (name_table[i]->offset == 1 && name_table[i]->reg == ERR_REG && name_table[i]->name[0] == '\0')
        {
            name_table[i]->offset = -1 * i * sizeof(int);
            strcpy(name_table[i]->name, elem);
            break;
        }
    }

    return ;
}

void TreeCtor(Tree_t* tree, FILE* log)
{
    tree->root = NULL;

    tree->log = log;
    tree->dump_num = (size_t) tree;

    tree->size = 0;

    tree->name_table = NT_Ctor();
}

ErrorKeys TreeDtor(Tree_t* tree)
{
    if (tree == NULL)
        return PTR_ERROR;

    NT_Dtor(tree->name_table);
    NodeDtor(&tree->root);

    return NO_ERRORS;
}

Node_t* CreateNode(Tree_t* tree, Data value, NodeType type, Node_t* left, Node_t* right)
{
    Node_t* node = (Node_t*)calloc(1, sizeof(Node_t));
    if(node == NULL)
    {
        printf(RED("error in callocation: file %s; func %s\n"), __FILE__, __func__);
        return node;
    }

    node->type = type;
    switch(node->type)
    {
        case OP:
        case VAR:
        case NUM:
        case FLT:
            node->data = value;
            break;
        case ERR_T:
        default:
            printf(RED("error type of operation : cannot create node\n"));
            return NULL;
            break;
    }

    node->left = left;
    node->right = right;

    /* tree always sends as nullptr
                                                            // increasing tree->size value
    if (left != NULL && right != NULL)
    {
        tree->size += 3;
    }
    else if (left != NULL || right != NULL)
    {
        tree->size += 2;
    }
    else
    {
        tree->size += 1;
    }
    */

    return node;
}

void NodeDtor(Node_t** node)
{
    if (*node == NULL)
    {
        return ;
    }

    if ((*node)->left)  NodeDtor(&(*node)->left);
    if ((*node)->right) NodeDtor(&(*node)->right);

    free(*node);
}


Node_t* Insert(Tree_t* tree, Node_t* node, int value, NodeType type)
{
    if (node == NULL)
    {
        return CreateNode(tree, IntToData(value), type, NULL, NULL);
    }

    if (node->data.num > value)
    {
        node->right = Insert(tree, node->right, value, type);
    }
    else
    {
        node->left  = Insert(tree, node->left,  value, type);
    }

    return node;
}

void Print(Node_t* node)
{
    if (node == NULL)
    {
        printf("()");
        return ;
    }

    printf("(");
    switch(node->type)
    {
        case NUM:
            printf("%d", node->data.num);
            break;
        case FLT:
            printf("%lg", node->data.flt);
            break;
        case OP:
            printf("%s", DecryptOperation(node->data.op));
            break;
        case VAR:
            //PrintVariable(node->data);
            printf("%s", node->data.var);
            break;
        case ERR_T:
        default:
            printf(RED("\nno such operand : %d\n"), node->data.num);
            return ;
            break;
    }

    if (node->left != NULL)
    {
        Print(node->left);
    }
    if (node->right != NULL)
    {
        Print(node->right);
    }


    printf(")");
}

const char* DecryptType(NodeType type)
{
    switch(type)
    {
        case NUM:
            return "num";
        case VAR:
            return "var";
        case  OP:
            return "op";
        case FLT:
            return "flt";
        case ERR_T:
            return "err";
        default:
            return "err";
    }

    return NULL;
}

const char* DecryptOperation(Operation operation)
{
    switch(operation)
    {
        case ADD:
            return "+";
        case SUB:
            return "-";
        case DIV:
            return "/";
        case MUL:
            return "*";
        case POW:
            return "^";
        case SIN:
            return "sin";
        case COS:
            return "cos";
        case TAN:
            return "tan";
        case CTG:
            return "ctg";
        case LOG:
            return "log";
        case R_PR_EXP:
            return ")";
        case L_PR_EXP:
            return "(";
        case EOT:
            return "$";
        case SEM:
            return ";";
        case AST:
            return "=";
        case IF:
            return "if";
        case WHILE:
            return "while";
        case FOR:
            return "for";
        case R_CURL:
            return "}";
        case L_CURL:
            return "{";
        case MORE:
            return "\\";
        case LESS:
            return "/";
        case EQ:
            return "==";
        case NOT_EQ:
            return "!=";
        case TOP_EQ:
            return "\\=";
        case BTM_EQ:
            return "/=";
        case ERR:
        default:
            return "NO SUCH OPERAND";
    }

    return NULL;
}

Operation DefineOperation(const char* opn)
{
    if (strcmp("+", opn) == 0)
    {
        return ADD;
    }
    else if (strcmp("-", opn) == 0)
    {
        return SUB;
    }
    else if (strcmp("*", opn) == 0)
    {
        return MUL;
    }
    else if (strcmp("/", opn) == 0)
    {
        return DIV;
    }
    else if (strcmp("^", opn) == 0)
    {
        return POW;
    }
    else if (strcmp("sin", opn) == 0)
    {
        return SIN;
    }
    else if (strcmp("cos", opn) == 0)
    {
        return COS;
    }
    else if (strcmp("tan", opn) == 0)
    {
        return TAN;
    }
    else if (strcmp("ctg", opn) == 0)
    {
        return CTG;
    }
    else if (strcmp("log", opn) == 0)
    {
        return LOG;
    }
    else if (strcmp("$", opn) == 0)
    {
        return EOT;
    }
    else if (strcmp("(", opn) == 0)
    {
        return L_PR_EXP;
    }
    else if (strcmp(")", opn) == 0)
    {
        return R_PR_EXP;
    }
    else if (strcmp("{", opn) == 0)
    {
        return L_CURL;
    }
    else if (strcmp("}", opn) == 0)
    {
        return R_CURL;
    }
    else if (strcmp("=", opn) == 0)
    {
        return AST;
    }
    else if (strcmp("if", opn) == 0)
    {
        return IF;
    }
    else if (strcmp("while", opn) == 0)
    {
        return WHILE;
    }
    else if (strcmp("\\", opn) == 0)
    {
        return MORE;
    }
    else if (strcmp("/", opn) == 0)
    {
        return LESS;
    }
    else if (strcmp("==", opn) == 0)
    {
        return EQ;
    }
    else if (strcmp("!=", opn) == 0)
    {
        return NOT_EQ;
    }
    else if (strcmp("/=", opn) == 0)
    {
        return BTM_EQ;
    }
    else if (strcmp("\\=", opn) == 0)
    {
        return TOP_EQ;
    }
    else if (strcmp(";", opn) == 0)
    {
        return SEM;
    }
    else
    {
        return ERR;
    }
}

bool ComparativeOperation(Operation op)
{
    return op == LESS || op == MORE || op == EQ || op == NOT_EQ || op == BTM_EQ || op == TOP_EQ;
}


