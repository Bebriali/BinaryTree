#include <stdio.h>
#include <assert.h>

#include "color.h"
#include "differentiator.h"

Node_t* Diff(Tree_t* tree, Node_t* node)
{
    if (node->type == NUM) return CreateNode(tree, 0, NUM, NULL, NULL);
    if (node->type == VAR) return CreateNode(tree, 1, NUM, NULL, NULL);
    if (node->type == OPN)
    {
        switch(node->data)
        {
            case ADD:
                return CreateNode(tree, ADD, OPN, Diff(tree, node->left), Diff(tree, node->right));
            case SUB:
                return CreateNode(tree, SUB, OPN, Diff(tree, node->left), Diff(tree, node->right));
            case MUL:
                return CreateNode(tree, ADD, OPN, CreateNode(tree, MUL, OPN, Diff(tree, node->left), Copy(tree, node->right)), \
                                                    CreateNode(tree, MUL, OPN, Copy(tree, node->left), Diff(tree, node->right)));
            case DIV:
                return CreateNode(tree, DIV, OPN, CreateNode(tree, SUB, OPN, CreateNode(tree, MUL, OPN, Diff(tree, node->left), Copy(tree, node->right)), \
                                                                                CreateNode(tree, MUL, OPN, Copy(tree, node->left), Diff(tree, node->right))), \
                                                    CreateNode(tree, MUL, OPN, Copy(tree, node->right), Copy(tree, node->right)));
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
        return node;
    Node_t* new_node = CreateNode(tree, node->data, node->type, Copy(tree, node->left), Copy(tree, node->right));

    NodeDtor(&node);

    return new_node;
}
