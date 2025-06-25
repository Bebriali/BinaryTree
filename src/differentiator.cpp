#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "color.h"
#include "differentiator.h"

union Data IntToData(int arg)
{
    Data value = {.num = arg};
    return value;
}
union Data VarToData(char* arg)
{
    Data value = {.var = arg};
    return value;
}
union Data FltToData(double arg)
{
    Data value = {.flt = arg};
    return value;
}
union Data OpToData(Operation arg)
{
    Data value = {.op = arg};
    return value;
}

Node_t* Diff(Tree_t* tree, Node_t* node)
{
    assert(tree);
    assert(node);

    if (node->type == NUM)
    {
        _NUM(node, 0);
        return node;
    }
    else if (node->type == VAR)
    {
        _NUM(node, 1);
        return node;
    }
    else if (node->type == OP)
    {
        switch(node->data.op)
        {
            case ADD:
                return _ADD(DiffLeft, DiffRight);
            case SUB:
                return _SUB(DiffLeft, DiffRight);
            case MUL:
                return _ADD(_MUL(DiffLeft, CopyRight),
                            _MUL(CopyLeft, DiffRight));
            case DIV:
                return _DIV(_SUB(_MUL(DiffLeft, CopyRight),
                                 _MUL(CopyLeft, DiffRight)),
                            _MUL(CopyRight, CopyRight));
            case POW:
                return _MUL(CopyNode,
                            _ADD(_MUL(_LOG(CopyLeft),
                                      DiffRight),
                                 _MUL(_DIV(DiffLeft,
                                           CopyLeft),
                                      CopyRight)));
                break;
            case SIN:
            {
                return _MUL(_COS(CopyLeft), DiffLeft);
                break;
            }
            case COS:
            {
                Node_t* leaf1 = NULL; _NUM(leaf1, -1);
                return _MUL(_MUL(leaf1, _SIN(CopyLeft)), DiffLeft);
                break;
            }
            case TAN:
            {
                Node_t* leaf1 = NULL; _NUM(leaf1, 1);
                Node_t* leaf2 = NULL; _NUM(leaf2, 2);
                return _DIV(leaf1, _POW(_COS(CopyLeft), leaf2));
                break;
            }
            case CTG:
            {
                Node_t* leaf1 = NULL; _NUM(leaf1, -1);
                Node_t* leaf2 = NULL; _NUM(leaf2,  2);
                return _DIV(leaf1, _POW(_SIN(CopyLeft), leaf2));
                break;
            }
            case LOG:
                return _DIV(DiffLeft, CopyLeft);
                break;
            case R_PR_EXP:
                return CopyNode;
                break;
            case L_PR_EXP:
                return CopyNode;
                break;
            case AST:
                return CopyNode;
                break;
            case SEM:
                return CopyNode;
                break;
            case EOT:
                return CopyNode;
                break;
            case ERR:
            default:
                printf(RED("undefined differential of function or wrong type of operation : "));
                printf("%s\n", DecryptOperation(node->data.op));
                return NULL;
                break;
        }
    }

    return NULL;
}

Node_t* Copy(Tree_t* tree, Node_t* node)
{
    assert(tree);

    if (node == NULL)
        return NULL;

    Node_t* new_node = CreateNode(tree, node->data, node->type, CopyLeft, CopyRight);

    return new_node;
}

Node_t* Simplify(Tree_t* tree, Node_t* node)
{
    if (node == NULL)
    {
        return node;
    }
    node->left = Simplify(tree, node->left);
    node->right = Simplify(tree, node->right);

    switch(node->type)
    {
        case OP:
            return SimplifyOperation(tree, node);
            break;
        case NUM:
        case FLT:
        case VAR:
        case ERR_T:
        default:
            return Copy(tree, node);
            break;
    }

    return NULL;
}

Node_t* SimplifyOperation(Tree_t* tree, Node_t* node)
{
    switch (node->data.op)
    {
        case ADD:
        {
            return AddSimp(tree, node);
            break;
        }
        case SUB:
        {
            return SubSimp(tree, node);
            break;
        }
        case MUL:
        {
            return MulSimp(tree, node);
            break;
        }
        case DIV:
        {
            return DivSimp(tree, node);
            break;
        }
        case POW:
        {
            return PowSimp(tree, node);
            break;
        }
        case SIN:
        case COS:
        case TAN:
        case CTG:
        case LOG:
        case R_PR_EXP:
        case L_PR_EXP:
        case EOT:
        case ERR:
        default:
        {
            return Copy(tree, node);
            break;
        }
    }
}

Node_t* AddSimp(Tree_t* tree, Node_t* node)
{
    if ((node->right)->type == NUM && (node->left)->type == NUM)
    {
        _NUM(node, ((node->right)->data.num + (node->left)->data.num));
        return node;
    }
    else if ((node->left)->type == NUM && (node->left)->data.num  == 0)
    {
        return CopyRight;
    }
    else if ((node->right)->type == NUM && (node->right)->data.num == 0)
    {
        return CopyLeft;
    }
    return CopyNode;
}

Node_t* SubSimp(Tree_t* tree, Node_t* node)
{
    if ((node->right)->type == NUM && (node->left)->type == NUM)
    {
        return _NUM(node, ((node->left)->data.num - (node->right)->data.num));
    }
    else if ((node->right)->type == NUM && (node->right)->data.num == 0)
    {
        return CopyLeft;
    }
    return CopyNode;
}

Node_t* MulSimp(Tree_t* tree, Node_t* node)
{
    if ((node->right)->type == NUM && (node->left)->type == NUM)
    {
        _NUM(node, ((node->left)->data.num * (node->right)->data.num));
        return node;
    }
    else if ((node->left)->type == NUM)
    {
        if ((node->left)->data.num == 0)
        {
            _NUM(node, 0);
            return node;
        }
        else if ((node->left)->data.num == 1)
        {
            return CopyRight;
        }
    }
    else if ((node->right)->type == NUM)
    {
        if ((node->right)->data.num == 0)
        {
            _NUM(node, 0);
            return node;
        }
        else if ((node->right)->data.num == 1)
        {
            return CopyLeft;
        }
    }
    return CopyNode;
}

Node_t* DivSimp(Tree_t* tree, Node_t* node)
{
    if ((node->right)->type == NUM)
    {
        if ((node->right)->data.num == 0)
        {
            printf(RED("ERROR IN CALCULATIONS, DIVIDING BY ZERO\n"));
            return NULL;
        }
    }
    else if ((node->right)->type == NUM && (node->left)->type == NUM)
    {
        return _NUM(node, ((node->left)->data.num / (node->right)->data.num));
    }
    else if ((node->left)->type == NUM)
    {
        if ((node->left)->data.num == 0)
        {
            _NUM(node, 0);
            return node;
        }
    }
    return CopyNode;
}

Node_t* PowSimp(Tree_t* tree, Node_t* node)
{
    if ((node->right)->type == NUM)
    {
        if ((node->left)->type == NUM)
        {
            _NUM(node, pow((node->left)->data.num, (node->right)->data.num));
            return node;
        }
        else if ((node->right)->data.num == 1)
        {
            return CopyLeft;
        }
        else if ((node->right)->data.num == 0)
        {
            _NUM(node, 1);
            return node;
        }
    }
    else if ((node->left)->type == NUM)
    {
        if ((node->left)->data.num == 1)
        {
            return CopyLeft;
        }
    }
    return CopyNode;
}
