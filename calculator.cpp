#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#include "calculator.h"
#include "color.h"
#include "debug_info.h"
#include "differentiator.h"

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
    ON_DEBUG(printf("GetG : p = %d\n", *p);)
    Node_t* val = GetE(tree, p, s);

    if (s[*p] != '$')
        SyntaxError(*p, s);
    (*p)++;

    return val;
}

Node_t* GetE(Tree_t* tree, size_t* p, char* s)
{
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
    ON_DEBUG(printf("GetT : p = %d\n", *p);)
    Node_t* val = GetP(tree, p, s);

    while (s[*p] == '*' || s[*p] == '/')
    {
        int op = s[*p];

        (*p)++;

        Node_t* val_extra = GetP(tree, p, s);

        if (op == '*') val = _MUL(val, val_extra);
        if (op == '/') val = _DIV(val, val_extra);
    }

    return val;
}

Node_t* GetP(Tree_t* tree, size_t* p, char* s)
{
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
    else if (isdigit(s[*p]))
        return GetN(tree, p, s);
    else
        return GetV(tree, p, s);
}

Node_t* GetN(Tree_t* tree, size_t* p, char* s)
{
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
    ON_DEBUG(printf("GetN : s[p] = %c\n", s[*p]);)
    
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
