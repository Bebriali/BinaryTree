#include "TxLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "calculator.h"
#include "color.h"
#include "tree_dump.h"
#include "tree_func.h"
#include "tree_struct.h"

void TestInput(Tree_t tree, char* logfile);
void HardCodeInput(Tree_t tree, char* logfile);

int main(int argc, char* argv[])
{
    Tree_t my_tree = {};
    TreeCtor(&my_tree);

    printf("enter simple example : \n");

    size_t p = 0;
    char s[100];
    scanf("%s", s);

    printf(BLUE("the gotten string is : '%s'\n"), s);

    int ans = GetG(&p, s);

    printf("The answer is : %d\n", ans);

    return 0;
}

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
