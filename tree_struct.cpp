#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "debug_info.h"
#include "tree_struct.h"

void TreeCtor (Tree_t* tree)
{
    tree->root = NULL;

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
    node->data = value;
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

    if (node->data > value)
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
            printf("%d", node->data);
            break;
        case OPN:
            printf("%s", DefineOperation(node->data));
            break;
        case VAR:
            //PrintVariable(node->data);
            printf("x");
            break;
        default:
            printf(RED("\nno such operand : %d\n"), node->data);
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

const char* DefineOperation(int operation)
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


#ifdef TRY_THIS_CODE_
void PrintVariable(int variable)
{
    for (size_t i = 0; i < ..._size; i++)
    {
        if (.... == variable)
        {
            printf("%c", ...var);
        }
    }
}
#endif


