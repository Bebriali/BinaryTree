#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#include "color.h"
#include "debug_info.h"
#include "differentiator.h"
#include "tree_builder.h"

#define ASSERT_BUILD(arg1, arg2, arg3) assert(arg1); \
                                       assert(arg2); \
                                       assert(arg3);

void SyntaxError(size_t p, char* s)
{
    printf(RED("error in syntax\n\t"));
    printf("%s\n\t", s);
    for (size_t i = 0; i < p; i++)
    {
        printf(" ");
    }
    printf("^\n\t");
    for (size_t i = 0; i < p; i++)
    {
        printf(" ");
    }
    printf("|\n");

    assert(0);
}

Node_t* GetG(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetG : p = %d\n", *p);)


    Node_t* val = GetE(tree, p, s);

    if (s[*p] != '$')
        SyntaxError(*p, s);
    (*p)++;

    return val;
}

Node_t* GetE(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetE : p = %d\n", *p);)


    Node_t* val = GetT(tree, p, s);

    while (s[*p] == '+' || s[*p] == '-')
    {
        int op = s[*p];
        (*p)++;

        Node_t* val_extra = GetT(tree, p, s);

        if (op == '+') val = _ADD(val, val_extra);
        if (op == '-') val = _SUB(val, val_extra);
    }

    return val;
}

Node_t* GetT(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetT : p = %d\n", *p);)


    Node_t* val = GetD(tree, p, s);

    while (s[*p] == '*' || s[*p] == '/')
    {
        int op = s[*p];

        (*p)++;

        Node_t* val_extra = GetD(tree, p, s);

        if (op == '*') val = _MUL(val, val_extra);
        if (op == '/') val = _DIV(val, val_extra);
    }

    return val;
}

Node_t* GetP(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetP : p = %d\n", *p);)


    if (s[*p] == '(')
    {
        (*p)++;
        Node_t* val = GetE(tree, p, s);

        if (s[*p] != ')')
            SyntaxError(*p, s);
        (*p)++;

        return val;
    }
    else
    {
        if (isdigit(s[*p]))
        {
            return GetN(tree, p, s);
        }
        else
        {
            return GetV(tree, p, s);
        }
    }
}

Node_t* GetO(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetO : p = %d\n", *p);)


    int old_p = *p;
    char opn[4];
    size_t opn_p = 0;
    while (isalpha(s[*p + opn_p]))
    {
        opn[opn_p] = s[*p + opn_p];
        opn_p++;
    }
    opn[opn_p] = '\0';
    ON_DEBUG(printf(BLUE("operation = %s\n"), opn);)

    Operation type = DefineOperation(opn);
    if (ERR == type)
    {
        return GetP(tree, p, s);
    }

    *p += opn_p;

    if (s[*p] != '(')
    {
        SyntaxError(*p, s);
    }

    return CreateNode(tree, type, OP, GetP(tree, p, s), NULL);
}

Node_t* GetD(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetD : p = %d\n", *p);)


    Node_t* base = GetO(tree, p, s);
    if (s[*p] != '^')
    {
        return base;
    }
    (*p)++;

    Node_t* degree = GetO(tree, p, s);
    if (degree == NULL)
    {
        SyntaxError(*p, s);
        return NULL;
    }
    else
    {
        return _POW(base, degree);
    }
}

Node_t* GetN(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetN : p = %d\n", *p);)


    int val = 0;
    size_t old_p = *p;
    while ('0' <= s[*p] && s[*p] <= '9')
    {
        val = val * 10 + s[*p] - '0';
        (*p)++;
    }

    ON_DEBUG(printf(MAGENTA("val = %d\n"), val);)
    ON_DEBUG(printf("old p = %d\n", old_p);)

    if (*p == old_p)
    {
        SyntaxError(*p, s);
    }

    return _NUM(val);
}

Node_t* GetV(Tree_t* tree, size_t* p, char* s)
{
    ASSERT_BUILD(tree, p, s);

    ON_DEBUG(printf("GetV : s[p] = %c\n", s[*p]);)
    ON_DEBUG(printf("GetV : p = %d\n", *p);)

    printf("string = %s\n", s);

    if ('x' <= s[*p] && s[*p] <= 'z')
    {
        int var = s[*p];
        (*p)++;
        return _VAR(var);
    }
    else
    {
        SyntaxError(*p, s);
    }

    return 0;
}
