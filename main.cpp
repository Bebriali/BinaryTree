//#include "TxLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "calculator.h"
#include "color.h"
#include "differentiator.h"
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
    TreeCtor(&my_tree);   my_tree.log = log;   my_tree.dump_num   = (size_t) &my_tree;

    BuildTree(&my_tree, argv[1]);
    TREE_DUMP(my_tree,   my_tree.log,   &my_tree.dump_num);

    Tree_t diff_tree = {};
    TreeCtor(&diff_tree); diff_tree.log = log; diff_tree.dump_num = (size_t) &diff_tree;
    diff_tree.root = Diff(&my_tree, my_tree.root);
    TREE_DUMP(diff_tree, diff_tree.log, &diff_tree.dump_num);
    
    Tree_t simp_diff = {};
    TreeCtor(&simp_diff); simp_diff.log = log; simp_diff.dump_num = (size_t) &simp_diff;
    simp_diff.root = Simplify(&diff_tree, diff_tree.root);
    TREE_DUMP(simp_diff, simp_diff.log, &simp_diff.dump_num);


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

    tree->root = GetG(tree, &p, s);

    TREE_DUMP(*tree, log, &dump_num);

    fprintf(log, "</pre>\n");
    fclose(log);
}
/*

void Calculate(void)
{
    printf("enter simple sum : \n");

    size_t p = 0;
    char s[100];
    scanf("%s", s);

    printf(BLUE("the gotten string is : '%s'\n"), s);

    int ans = GetG(&p, s);

    printf("The answer is : %d\n", ans);
}
*/

void HardCodeInput(Tree_t my_tree, char* logfile)
{
    size_t dump_num = 0;
    FILE* log = fopen(logfile, "wb");
    fprintf(log, "<pre>\n");

    Node_t* node2    = CreateNode(&my_tree, 2, NUM, NULL, NULL);
    Node_t* node3    = CreateNode(&my_tree, 3, NUM, NULL, NULL);
    Node_t* node_add = CreateNode(&my_tree, ADD, OPN, node2, node3);

    my_tree.root = node_add;

    TREE_DUMP(my_tree, log, &dump_num);

    TreeDtor(&my_tree);

    fprintf(log, "</pre>\n");
    fclose(log);
}

void TestInput(Tree_t my_tree, char* logfile)
{
    srand(time(NULL));

    FILE* log = fopen(logfile, "wb");
    fprintf(log, "<pre>\n");

    size_t dump_num = 0;

    printf("my-tree addr = %p\n", &my_tree);
    printf("my-tree-root addr = %p\n", my_tree.root);
    printf("enter the tree : \n");

    my_tree.root = ReadInsert(&my_tree, my_tree.root);

    TREE_DUMP(my_tree, log, &dump_num);

    TreeDtor(&my_tree);

    fprintf(log, "</pre>\n");
    fclose(log);
}
