#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "color.h"
#include "differentiator.h"

Node_t* Diff(Tree_t* tree, Node_t* node)
{
    assert(tree);
    assert(node);

    if (node->type == NUM) return _NUM(0);
    if (node->type == VAR) return _NUM(1);
    if (node->type == OP)
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
            case POW:   // todo rename to POW
                // new way for differentiating
                return _MUL(CopyNode,
                            _ADD(_MUL(_LOG(CopyLeft),
                                      DiffRight),
                                _MUL(_DIV(DiffLeft,
                                          CopyLeft),
                                    CopyRight)));

                break;
            case SIN:
                return _MUL(_COS(CopyLeft), DiffLeft);
                break;
            case COS:
                return _MUL(_MUL(_NUM(-1), _SIN(CopyLeft)), DiffLeft);
                break;
            case TAN:
                return _DIV(_NUM(1), _POW(_COS(CopyLeft), _NUM(2)));
                break;
            case CTG:
                return _DIV(_NUM(-1), _POW(_SIN(CopyLeft), _NUM(2)));
                break;
            case LOG:
                return _DIV(DiffLeft, CopyLeft);
                break;
            case ERR:
            default:
                printf(RED("undefined differential of function or wrong type of operation : "));
                printf("%s\n", DecryptOperation(node->data.num));
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
    Node_t* new_node = CreateNode(tree, node->data.num, node->type, CopyLeft, CopyRight);

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
            switch (node->data.op)
            {
                case ADD:
                    // return AddSimp(node);
                    //  Node_t* AddSimp(Node_t* node)
                    //  {
                            if ((node->right)->type == NUM && (node->left)->type == NUM)
                            {
                                return _NUM(((node->right)->data.num + (node->left)->data.num));
                            }
                            else if ((node->left)->type == NUM && (node->left)->data.num  == 0)
                            {
                                return CopyRight;
                            }
                            else if ((node->right)->type == NUM && (node->right)->data.num == 0)
                            {
                                return CopyLeft;
                            }
                    //  }
                    break;
                case SUB:
                    //  return SubSimp(node);
                    //  Node_t* SubSimp(Node_t* node)
                    //  {
                            if ((node->right)->type == NUM && (node->left)->type == NUM)
                            {
                                return _NUM(((node->left)->data.num - (node->right)->data.num));
                            }
                            else if ((node->right)->type == NUM && (node->right)->data.num == 0)
                            {
                                return CopyLeft;
                            }
                    //  }
                    break;
                case MUL:
                    //  return MulSimp(node);
                    //  Node_t* MulSimp(node)
                    //  {
                            if ((node->right)->type == NUM && (node->left)->type == NUM)
                            {
                                return _NUM(((node->left)->data.num * (node->right)->data.num));
                            }
                            else if ((node->left)->type == NUM)
                            {
                                if ((node->left)->data.num == 0)
                                {
                                    return _NUM(0);
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
                                    return _NUM(0);
                                }
                                else if ((node->right)->data.num == 1)
                                {
                                    return CopyLeft;
                                }
                            }
                    //  }
                    break;
                case DIV:
                    //  return DivSimp(node);
                    //  Node_t* DivSimp(Node_t* node);
                    //  {
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
                                return _NUM(((node->left)->data.num / (node->right)->data.num));
                            }
                            else if ((node->left)->type == NUM)
                            {
                                if ((node->left)->data.num == 0)
                                {
                                    return _NUM(0);
                                }
                            }
                    //  }
                    break;
                case POW:
                    if ((node->right)->type == NUM)
                    {
                        if ((node->left)->type == NUM)
                        {
                            return _NUM(pow((node->left)->data.flt, (node->right)->data.flt));
                        }
                        else if ((node->right)->data.num == 1)
                        {
                            return CopyLeft;
                        }
                        else if ((node->right)->data.num == 0)
                        {
                            return _NUM(1);
                        }
                    }
                    else if ((node->left)->type == NUM)
                    {
                        if ((node->left)->data.num == 1)
                        {
                            return CopyLeft;
                        }
                    }
                default:
                    return Copy(tree, node);
                    break;
            }
        default:
            return Copy(tree, node);
            break;
    }

    return NULL; //
}
