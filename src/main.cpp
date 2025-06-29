#include "TxLib.h"
// #include "../../../../../../Documents/TX/TXLib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "color.h"
#include "debug_info.h"
#include "differentiator.h"
#include "tokenizer.h"
#include "tree_builder.h"
#include "tree_dump.h"
#include "tree_func.h"
#include "tree_printer.h"
#include "tree_reader.h"
#include "tree_struct.h"

const int MAX_LEN_EXAMPLE = 100;

void TestInput(Tree_t tree, char* logfile);
void HardCodeInput(Tree_t tree, char* logfile);

void BuildTree(Tree_t* tree, FILE* log);

void Calculate(void);

int main(int argc, char* argv[])
{
    const char* logfile = argv[1];
    FILE* log = fopen(logfile, "wb");
    fprintf(log, "<pre>\n");

    if (argc <= 1 || (argc > 1 && strcmp(argv[1], "--use-dump-tree")))
    {
        Tree_t my_tree = {};
        TreeCtor(&my_tree, log);

        BuildTree(&my_tree, log);
        // TREE_DUMP(&my_tree);
    /*
        Tree_t diff_tree = {};
        TreeCtor(&diff_tree, log);
        diff_tree.root = Diff(&my_tree, my_tree.root);
        TREE_DUMP(&diff_tree);

        TreeDtor(&diff_tree);
    */

        printf("filename = %s ; log = %p\n", logfile, log);
        Tree_t simp_tree = {};
        TreeCtor(&simp_tree, log);
        simp_tree.root = Simplify(&my_tree, my_tree.root);
        // TREE_DUMP(&simp_tree);
        TreePrint(&simp_tree, "log/print_tree.txt");
        TreeDtor(&my_tree);
        TreeDtor(&simp_tree);
    }

    ON_DEBUG(printf("reading tree...\n");)
    Tree_t* read_tree = TreeRead("log/print_tree.txt", NULL);
    ON_DEBUG(printf("dumping tree...\n");)
    TREE_DUMP(read_tree);

    TreeDtor(read_tree);

    fprintf(log, "</pre>\n");
    fclose(log);

    return 0;
}

void BuildTree(Tree_t* tree, FILE* log)
{
    // FILE* log = fopen(logfile, "wb");
    // fprintf(log, "<pre>\n");

    size_t p = 0;
    char s[MAX_LEN_EXAMPLE];
    printf("enter the sum : \n");

    /* this function is not safe
    gets(s); */

                                                            // getting string
    int c = 0;
    while((c = getchar()) != EOF)
    {
        s[p] = (char) c;
        ++p;

        if (c == '$')
        {
            s[++p] = '\0';
            break;
        }
    }
    p = 0;

    printf("read string = %s\n", s);

    Tokens* command = Tokenize(s, &p);

    DumpToken(command);

    p = 0;
    tree->root = GetG(tree, &p, command);
    if (tree->root == NULL)
    {
        //free_all();
        return ;
    }

    ON_DEBUG(printf("tree->root addr = %p\n", tree->root);)
    ON_DEBUG(printf("tree->right = %p / tree->left = %p\n", tree->root->right, tree->root->left);)

    // TREE_DUMP(tree);

    // fprintf(log, "</pre>\n");
    // fclose(log);
}
/*

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
*/
