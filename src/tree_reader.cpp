#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "color.h"
#include "differentiator.h"
#include "tokenizer.h"
#include "tree_func.h"
#include "tree_reader.h"
#include "tree_struct.h"

Tree_t* TreeRead(const char* filename, FILE* log)
{
    FILE* dump = fopen(filename, "rb");
    if (dump == NULL)
    {
        printf(RED("error in reading file %s\n"), filename);
        return NULL;
    }
    printf("file %s is opened\n", filename);

    // fseek(dump, 0, SEEK_END);
    // size_t file_size = ftell(dump);

    // char* buf = (char*) calloc(file_size, sizeof(char));
    // if (buf == NULL)
    // {
    //     printf(RED("error in buf allocation\n"));
    //     return NULL;
    // }

    // size_t read_file_size = fread(buf, size, count, dump);

    // if (file_size != read_file_size)
    // {
    //     printf(RED("error in reading file %s\n"), filename);
    //     return NULL;
    // }

    Tree_t* tree = (Tree_t*) calloc(1, sizeof(Tree_t));
    if (tree == NULL)
    {
        printf(RED("error in memory allocation for tree\n"));
        return NULL;
    }
    TreeCtor(tree, log);

    printf("tree is callocated\n");
    tree->root = ReadNode(dump);
    printf("the tree is gotten: %p\n", tree);

    fclose(dump);

    return tree;
}

Node_t* ReadNode(FILE* dump)
{
    assert(dump);

    char str[10] = "";
    long pos = ftell(dump);
    printf("pos before checking str way is %ld\n", pos);
    fscanf(dump, "%s", str);
    printf("checked str way : '%s'\n", str);

    if (strcmp(str, "{nullptr}"))
    {
        fseek(dump, pos, SEEK_SET);
    }
    else
    {
        printf("detected '{nullptr}', return NULL\n");
        return NULL;
    }

    pos = ftell(dump);
    printf("file position before fscanf = %ld\n", pos);

    // string example: {type: OP | data: if
    char type[32] = "";
    char data[64] = "";

    // it was int c = 0; here
    char c = 0;
    fscanf(dump, "%c", &c);
    while (c == '\t')
    {
        pos++;
        fscanf(dump, "%c", &c);
    }
    fseek(dump, pos, SEEK_SET);

    int sscanf_scs = fscanf(dump, "{type: %31s | data: %63s\n", type, data);
    printf("fscanf has read %d args\n", sscanf_scs);
    printf("type = %s, data = %s\n", type, data);

    char str1[32] = "";
    char str2[32] = "";
    char str3[32] = "";
    char str4[32] = "";
    char str5[32] = "";

    pos = ftell(dump);
    printf("position before reading nxt string is %ld\n", pos);
    sscanf_scs = fscanf(dump, "%s%s%s%s%s", str1, str2, str3, str4, str5);
    printf("fscanf has read '%s' '%s' '%s' '%s' '%s'\n", str1, str2, str3, str4, str5);
    fseek(dump, pos, SEEK_SET);

    if (sscanf_scs < 2) {
        printf(RED("error: could not parse node\n"));
        return NULL;
    };

    Node_t* node = NULL;

    if (!strcmp(type, "VAR"))
    {
        /*node             = */_VAR(node, data);
    }
    else if (!strcmp(type, "OP"))
    {
        Operation defined_op = DefineOperation(data);

        /*node                 = */_OP(node, defined_op);
    }
    else if (!strcmp(type, "NUM"))
    {
        size_t pointer  = 0;
        int defined_num = GetNumber(data, &pointer);

        /*node            = */_NUM(node, defined_num);
    }
    else
    {
        printf(RED("error in tree_reading: undefined operation - %s\n"), type);
    }

    printf(BLUE("node read addr = %p\n"), node);
    if (node == NULL)
    {
        printf(RED("no node created\n"));
        return NULL;
    }

    if (node == NULL)
    {
        printf(RED("error in node reading\n"));
        return node;
    }

    node->left  = ReadNode(dump);
    node->right = ReadNode(dump);

    char check_l_curl[10] = "";
    pos = ftell(dump);
    fscanf(dump, "%s\n", check_l_curl);
    printf("checked str way : '%s'\n", check_l_curl);

    if (strcmp(check_l_curl, "}"))
    {
        fseek(dump, pos, SEEK_SET);
    }
    else
    {
        printf("detected l_curl\n");
    }

    return node;
}
