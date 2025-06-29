#include <stdio.h>
#include <stdlib.h>

#include "color.h"
#include "translator.h"
#include "tree_struct.h"

void CodeToAsm(Tree_t* tree)
{
    FILE* code_asm = fopen("asm/code.s", "wb");
    if (code_asm == NULL)
    {
        printf(RED("error in creating file for asm code\n"));
        return ;
    }
    ConvNode(tree->root, code_asm);
}

void ConvNode(Node_t* node, FILE* file)
{
    if (!node)
    {
        return ;
    }

    ConvNode(node->left, file);



    ConvNode(node->right, file);
}
