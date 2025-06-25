#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "color.h"
#include "debug_info.h"
#include "tree_dump.h"

const int SYS_COM_SIZE = 40;

ErrorKeys CreateLog(Tree_t* tree, const char* file, const char* func, const int line)
{
    assert(tree);

    size_t dump_number = tree->dump_num;
    FILE* log = tree->log;

    if (log)
    {
        fprintf(log, "<FONT color = GREEN>TREE_DUMP : </FONT>\n");
        fprintf(log, "file : %s\n", file);
        fprintf(log, "func : %s\n", func);
        fprintf(log, "<FONT color = >line : %d</FONT>\n", line);
    }

    char* pic_name = (char*)calloc(10, sizeof(char));
    if (pic_name == NULL)
    {
        printf(RED("invalid callocation\n"));
        return PTR_ERROR;
    }

    sprintf(pic_name, "log/%d.png", dump_number);

    if (log)
        fprintf(log, "<img src = %s>\n", pic_name);

    char* filename = (char*)calloc(10, sizeof(char));
    if (filename == NULL)
    {
        printf(RED("invalid callocation\n"));
        return PTR_ERROR;
    }

    sprintf(filename, "log/%d.dot", dump_number);

    FILE* tree_dump = fopen(filename, "wb");
    if (tree_dump == NULL)
    {
        printf(RED("invalid file\n"));
        return PTR_ERROR;
    }

    fprintf(tree_dump, "digraph TREE\n");
    fprintf(tree_dump, "{\t");
    fprintf(tree_dump, "rankdir = TB\n\t");
    fprintf(tree_dump, "node[fontsize = 25, shape = Mrecord];\n\t");
    fprintf(tree_dump, "edge[color = \"#0AF2C8\", fontcolor = \"white\", fontsize = 5];\n\t");

    size_t rank = 1;
    FormObjects(tree->root, tree_dump, rank);
    ShapeTree(tree->root, tree_dump);

    fprintf(tree_dump, "}");

    fclose(tree_dump);
    free(filename);

    char system_command[SYS_COM_SIZE];
    sprintf(system_command, "dot ./log/%d.dot -Tpng -o ./log/%d.png", dump_number, dump_number);

    ON_DEBUG(int system_code =) system(system_command);
    ON_DEBUG(printf("system_code = %d\n", system_code);)

    (tree->dump_num)++;
    return NO_ERRORS;
}

void FormObjects(Node_t* node, FILE* tree_dump, size_t rank)
{
    assert(node);
    assert(tree_dump);

    ON_DEBUG(for (size_t i = 0; i < rank; i++))
    ON_DEBUG({)
    ON_DEBUG(    printf(" ");)
    ON_DEBUG(})
    ON_DEBUG(printf("node_addr = %p\n", node);)
    fprintf(tree_dump, "x%p [", node);

    if (node->type == OP)
        {
            switch(node->data.op)
            {
                case ADD:
                    fprintf(tree_dump, "style = filled, color = \"#e86464\", fontcolor = black, ");
                    break;
                case SUB:
                    fprintf(tree_dump, "style = filled, color = blue, fontcolor = \"#0af3f7\", ");
                    break;
                case MUL:
                    fprintf(tree_dump, "style = filled, color = \"#de90dd\", fontcolor = black, ");
                    break;
                case DIV:
                    fprintf(tree_dump, "style = filled, color = \"#d170db\", fontcolor = black, ");
                    break;
                case POW:
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
                    break;
            }
        }

    fprintf(tree_dump, "label = \"{ ptr:x%p | ", node);

    if (node->type == VAR)
    {
        fprintf(tree_dump, "data:%s", node->data.var);
    }
    else if (node->type == NUM)
    {
        fprintf(tree_dump, "data:%d ", node->data.num);
    }
    else if (node->type == OP)
    {
        fprintf(tree_dump, "data:%s ", DecryptOperation(node->data.op));
    }

    fprintf(tree_dump, "| { <left0>x%p | <right0>x%p }", \
                    node->left, node->right);
    fprintf(tree_dump, "}\"];\n\t");

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




