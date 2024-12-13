//#include "TxLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "color.h"
#include "differentiator.h"
#include "tree_builder.h"
#include "tree_dump.h"
#include "tree_func.h"
#include "tree_struct.h"

const int MAX_LEN_EXAMPLE = 100;

void TestInput(Tree_t tree, char* logfile);
void HardCodeInput(Tree_t tree, char* logfile);

void BuildTree(Tree_t* tree, char* logfile);

void Calculate(void);

int main(int argc, char* argv[])
{
    const char* logfile = argv[1];
    FILE* log = fopen(logfile, "wb");
    fprintf(log, "<pre>\n");


    Tree_t my_tree = {};
    TreeCtor(&my_tree, log);

    BuildTree(&my_tree, argv[1]);
    TREE_DUMP(&my_tree);

    Tree_t diff_tree = {};
    TreeCtor(&diff_tree, log);
    diff_tree.root = Diff(&my_tree, my_tree.root);
    TREE_DUMP(&diff_tree);

    Tree_t simp_diff = {};
    TreeCtor(&simp_diff, log);
    simp_diff.root = Simplify(&diff_tree, diff_tree.root);
    TREE_DUMP(&simp_diff);


    TreeDtor(&my_tree);
    TreeDtor(&diff_tree);


    fprintf(log, "</pre>\n");
    fclose(log);

    return 0;
}

void BuildTree(Tree_t* tree, char* logfile)
{
    size_t dump_num = 0;
    FILE* log = fopen(logfile, "wb");
    fprintf(log, "<pre>\n");

    size_t p = 0;
    char s[MAX_LEN_EXAMPLE];
    printf("enter the sum : \n");
    scanf("%s", s);

    printf("read string = %s\n", s);

    tree->root = GetG(tree, &p, s);

    TREE_DUMP(tree);

    fprintf(log, "</pre>\n");
    fclose(log);
}

void HardCodeInput(Tree_t my_tree, char* logfile)
{
    FILE* log = fopen(logfile, "wb");
    fprintf(log, "<pre>\n");

    TreeCtor(&my_tree, log);

    Node_t* node2    = CreateNode(&my_tree, 2, NUM, NULL, NULL);
    Node_t* node3    = CreateNode(&my_tree, 3, NUM, NULL, NULL);
    Node_t* node_add = CreateNode(&my_tree, ADD, OP, node2, node3);

    my_tree.root = node_add;

    TREE_DUMP(&my_tree);

    TreeDtor(&my_tree);

    fprintf(log, "</pre>\n");
    fclose(log);
}

void TestInput(Tree_t my_tree, char* logfile)
{
    srand(time(NULL));

    FILE* log = fopen(logfile, "wb");
    fprintf(log, "<pre>\n");

    printf("my-tree addr = %p\n", &my_tree);
    printf("my-tree-root addr = %p\n", my_tree.root);
    printf("enter the tree : \n");

    my_tree.root = ReadInsert(&my_tree, my_tree.root);

    TREE_DUMP(&my_tree);

    TreeDtor(&my_tree);

    fprintf(log, "</pre>\n");
    fclose(log);
}
