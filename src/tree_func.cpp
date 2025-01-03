#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "color.h"
#include "debug_info.h"
#include "tree_func.h"

void UserInsert(Tree_t* tree)
{
    printf("enter the value : ");
    int value = 0;
    scanf("%d", &value);

    tree->root = Insert(tree, tree->root, value, NUM);
    tree->size++;
}

void GenInsert(Tree_t* tree)
{
    int value = rand()%100 + rand()%5;

    tree->root = Insert(tree, tree->root, value, NUM);
    printf("tree->root.addr = %p\n", tree->root);
    tree->size++;
}

Node_t* ReadInfInsert(Node_t* node)
{
    int c = 0;
    c = getchar();
    printf("char c = %c\n", c);
    printf("node->addr = %p\n", node);

    if (c == '(')
        node->left = ReadInfInsert(node->left);
    if (c == ')')
        return node;

    if (isdigit(c))
        ungetc(c, stdin);

    int value = 0;
    scanf("%d", &value);
    //node = CreateNode(, value, NUM, NULL, NULL);
    printf("created node = %p\n", node);

    getchar();
    node->right = ReadInfInsert(node->right);

    return node;
}

Node_t* ReadInsert(Tree_t* tree, Node_t* node)
{
    ON_DEBUG(Print(node); printf("\n");)

    int c = 0;
    c = getchar();
    printf(BLUE("char c = %c\n"), c);
    if (c == '(')
        return ReadInsert(tree, node);
    if (c == ')')
        return node;

    ungetc(c, stdin);

    char s[10];
    scanf("%[^()]", s);

    printf(GREEN("scanned command is %s\n"), s);

    int value = 0;
    NodeType type = ERR_T;
    Data data = GetNodeData(s, &value, &type);

    if (!node)
    {
        node = CreateNode(tree, data, type, NULL, NULL);
        printf(MAGENTA("created node = %p\n"), node);
    }
    else if (!node->left)
    {
        node->left = CreateNode(tree, data, type, NULL, NULL);

        ON_DEBUG(Print(node); printf("\n");)

        node->left = ReadInsert(tree, node->left);
    }
    else if (!node->right)
    {
        node->right = CreateNode(tree, data, type, NULL, NULL);

        ON_DEBUG(Print(node); printf("\n");)

        node->right = ReadInsert(tree, node->right);
    }
    return ReadInsert(tree, node);
}

union Data GetNodeData(char* command, void* value, NodeType* type)
{
    if (isdigit(command[0]))
    {
        *type = NUM;
        *(int*) value = atoi(command);
        Data data = {.num = *(int*) value};
        return data;
    }

    else if (isalpha(command[0]))
    {
        *type = OP;
        if (strcmp(command, "sin") == 0)
        {
            *(Operation*) value = SIN;
        }
        else if (strcmp(command, "cos") == 0)
        {
            *(Operation*) value = COS;
        }
        else if (strcmp(command, "tan") == 0)
        {
            *(Operation*) value = TAN;
        }
        else if (strcmp(command, "ctg") == 0)
        {
            *(Operation*) value = CTG;
        }
        else if (strcmp(command, "log") == 0)
        {
            *(Operation*) value = CTG;
        }
        Data data = {.op = *(Operation*) value};
        return data;
    }
    else
    {
        *type = OP;
        if (strcmp(command, "+")    == 0)
        {
            *(Operation*) value = ADD;
        }
        else if (strcmp(command, "*") == 0)
        {
            *(Operation*) value = MUL;
        }
        else if (strcmp(command, "-") == 0)
        {
            *(Operation*) value = SUB;
        }
        else if (strcmp(command, "/") == 0)
        {
            *(Operation*) value = DIV;
        }
        Data data = {.op = *(Operation*) value};
        return data;
    }
}
