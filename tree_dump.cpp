#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree_dump.h"
#include "color.h"

const int SYS_COM_SIZE = 40;

ErrorKeys CreateLog(Tree_t tree, FILE* log, size_t* dump_num, \
                    const char* file, const char* func, const int line)
{
    assert(tree.root);
    assert(dump_num);

    fprintf(log, "<FONT color = GREEN>TREE_DUMP : </FONT>\n");
    fprintf(log, "file : %s\n", file);
    fprintf(log, "func : %s\n", func);
    fprintf(log, "<FONT color = >line : %d</FONT>\n", line);

    char* pic_name = (char*)calloc(10, sizeof(char));
    if (pic_name == NULL)
    {
        printf(RED("invalid callocation\n"));
        return PTR_ERROR;
    }

    sprintf(pic_name, "log/%d.png", *dump_num);


    fprintf(log, "<img src = %s>\n", pic_name);

    char* filename = (char*)calloc(10, sizeof(char));
    if (filename == NULL)
    {
        printf(RED("invalid callocation\n"));
        return PTR_ERROR;
    }
    sprintf(filename, "log/%d.dot", *dump_num);

    FILE* tree_dump = fopen(filename, "wb");
    if (tree_dump == NULL)
    {
        printf(RED("invalid file\n"));
        return PTR_ERROR;
    }

    fprintf(tree_dump, "digraph TREE\n");
    fprintf(tree_dump, "{\t");
    fprintf(tree_dump, "rankdir = TB\n\t");
    fprintf(tree_dump, "node[color = \"black\", fontsize = 10];\n\t");
    fprintf(tree_dump, "edge[color = \"#0AF2C8\", fontcolor = \"white\", fontsize = 5];\n\t");

    size_t rank = 1;
    FormObjects(tree.root, tree_dump, rank);
    ShapeTree(tree.root, tree_dump);

    fprintf(tree_dump, "}");

    fclose(tree_dump);
    free(filename);

    char system_command[SYS_COM_SIZE];
    sprintf(system_command, "dot ./log/%d.dot -Tpng -o ./log/%d.png", *dump_num, *dump_num);

    int system_code = system(system_command);
    printf("system_code = %d\n", system_code);

    (*dump_num)++;
    return NO_ERRORS;
}

void FormObjects(Node_t* node, FILE* tree_dump, size_t rank)
{
    fprintf(tree_dump, "x%p [shape = record, label = \"{ ptr:x%p | ", node, node);

    if (node->type == VAR)
    {
        fprintf(tree_dump, "data:x");
    }
    else if (node->type == NUM)
    {
        fprintf(tree_dump, "data:%d ", node->data);
    }
    else if (node->type == OPN)
    {
        fprintf(tree_dump, "data:%s ", DefineOperation(node->data));
    }

    fprintf(tree_dump, "| { <left0>x%p | <right0>x%p }}\"];\n\t", \
                    node->left, node->right);

    if(node->left)  FormObjects(node->left,  tree_dump, rank + 1);
    if(node->right) FormObjects(node->right, tree_dump, rank + 1);
}

void ShapeTree(Node_t* node, FILE* tree_dump)
{
    if (node->left)  fprintf(tree_dump, "x%p:<left0> -> x%p;\n\t", node, node->left);
    if (node->right) fprintf(tree_dump, "x%p:<right0> -> x%p;\n\t", node, node->right);

    if (node->left)  ShapeTree(node->left,  tree_dump);
    if (node->right) ShapeTree(node->right, tree_dump);
}




