#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "debug_info.h"
#include "tree_struct.h"

void TreeCtor (Tree_t* tree, FILE* log)
{
    tree->root = NULL;

    tree->log = log;
    tree->dump_num = (size_t) tree;

    tree->size = 0;
}

ErrorKeys TreeDtor(Tree_t* tree)
{
    if (tree == NULL)
        return PTR_ERROR;

    NodeDtor(&tree->root);

    return NO_ERRORS;
}

Node_t* CreateNode(Tree_t* tree, int value, NodeType type, Node_t* left, Node_t* right)
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
            node->data.op = (Operation) value;
            break;
        case VAR:
            node->data.var = (char) value;
            break;
        case NUM:
            node->data.num = (int) value;
            break;
        case FLT:
            node->data.flt = (double) value;
        default:
            break;
    }

    node->left = left;
    node->right = right;

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
        return CreateNode(tree, value, type, NULL, NULL);
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
        case OP:
            printf("%s", DecryptOperation(node->data.op));
            break;
        case VAR:
            //PrintVariable(node->data);
            printf("%c", node->data.var);
            break;
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

const char* DecryptOperation(int operation)
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
    else
    {
        return ERR;
    }
}
