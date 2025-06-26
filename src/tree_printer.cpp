#include <stdio.h>

#include "color.h"
#include "tree_func.h"
#include "tree_printer.h"
#include "tree_struct.h"

void TreePrint(Tree_t* tree, const char* filename)
{
    if (filename == NULL)
        filename = "log/tree_print.txt";


    if (tree == NULL)
    {
        printf(RED("no tree to be printed\n"));
        return ;
    }

    FILE* tree_dump = fopen(filename, "wb");
    PrintNode(tree->root, tree_dump, 0);

    fclose(tree_dump);

    return ;
}

void PrintNode(Node_t* node, FILE* tree_dump, int rank)
{
    for (int i = 0; i < rank; i++)
    {
        fprintf(tree_dump, "\t");
    }
    ++rank;
                                                            // prints currents node
    fprintf(tree_dump, "{");
    if (node && node->type == VAR)
    {
        fprintf(tree_dump, "type: VAR | data: %s", node->data.var);
    }
    else if (node && node->type == NUM)
    {
        fprintf(tree_dump, "type: NUM | data: %d ", node->data.num);
    }
    else if (node && node->type == OP)
    {
        fprintf(tree_dump, "type: OP | data: %s ", DecryptOperation(node->data.op));
    }
    else if (node)
    {
        fprintf(tree_dump, "type: ERR | data: %s ", DecryptOperation(node->data.op));
    }
    else
    {
        fprintf(tree_dump, "nullptr}\n");
        return ;
    }
    fprintf(tree_dump, "\n");
    if (node)
    {
                                                            // prints left node
        PrintNode(node->left, tree_dump, rank);
                                                            // prints right node
        PrintNode(node->right, tree_dump, rank);
    }
    for (int i = 1; i < rank; i++)
    {
        fprintf(tree_dump, "\t");
    }
    fprintf(tree_dump, "}\n");

    return ;
}
