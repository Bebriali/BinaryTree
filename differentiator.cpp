#include <stdio.h>
#include <assert.h>

#include "color.h"
#include "differentiator.h"

Node_t* Diff(Tree_t* tree, Node_t* node)
{
    if (node->type == NUM) return _NUM(0);
    if (node->type == VAR) return _NUM(1);
    if (node->type == OPN)
    {
        switch(node->data)
        {
            case ADD:
                return _ADD(Diff(tree, node->left), Diff(tree, node->right));
            case SUB:
                return _SUB(Diff(tree, node->left), Diff(tree, node->right));
            case MUL:
                return _ADD(_MUL(Diff(tree, node->left), Copy(tree, node->right)), \
                            _MUL(Copy(tree, node->left), Diff(tree, node->right)));
            case DIV:
                return _DIV(_SUB(_MUL(Diff(tree, node->left), Copy(tree, node->right)), 
                                 _MUL(Copy(tree, node->left), Diff(tree, node->right))),
                            _MUL(Copy(tree, node->right), Copy(tree, node->right)));
            default:
                printf(RED("undefined differential of function or wrong type of operation : "));
                printf("%s\n", DefineOperation(node->data));
                return NULL;
        }
    }

    return NULL;
}

Node_t* Copy(Tree_t* tree, Node_t* node)
{
    assert(tree);

    if (node == NULL)
        return NULL;
    Node_t* new_node = CreateNode(tree, node->data, node->type, Copy(tree, node->left), Copy(tree, node->right));

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
        case OPN:
            switch (node->data)
            {
                case ADD:
                    if ((node->right)->type == NUM && (node->left)->type == NUM)
                    {
                        return _NUM(((node->right)->data + (node->left)->data));
                    }
                    else if ((node->left)->type == NUM && (node->left)->data == 0)
                    {
                        return Copy(tree, node->right);
                    }
                    else if ((node->right)->type == NUM && (node->right)->data == 0)
                    {
                        return Copy(tree, node->left);
                    }
                    break;
                case SUB:
                    if ((node->right)->type == NUM && (node->left)->type == NUM)
                    {
                        return _NUM(((node->left)->data - (node->right)->data));
                    }
                    else if ((node->right)->type == NUM && (node->right)->data == 0)
                    {
                        return Copy(tree, node->left);
                    }
                    break;
                case MUL:
                    if ((node->right)->type == NUM && (node->left)->type == NUM)
                    {
                        return _NUM(((node->left)->data * (node->right)->data));
                    }
                    else if ((node->left)->type == NUM)
                    {
                        if ((node->left)->data == 0)
                        {
                            return _NUM(0);
                        }
                        else if ((node->left)->data == 1)
                        {
                            return Copy(tree, node->right);
                        }
                    }
                    else if ((node->right)->type == NUM)
                    {
                        if ((node->right)->data == 0)
                        {
                            return _NUM(0);
                        }
                        else if ((node->right)->data == 1)
                        {
                            return Copy(tree, node->left);
                        }
                    }
                    break;
                case DIV:
                    if ((node->right)->type == NUM)
                    {
                        if ((node->right)->data == 0)
                        {
                            printf(RED("ERROR IN CALCULATIONS, DIVIDING BY ZERO\n"));
                            return NULL;
                        }
                    }
                    else if ((node->right)->type == NUM && (node->left)->type == NUM)
                    {
                        return _NUM(((node->left)->data / (node->right)->data));
                    }
                    else if ((node->left)->type == NUM)
                    {
                        if ((node->left)->data == 0)
                        {
                            return _NUM(0);
                        }
                    }
                    break;
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
