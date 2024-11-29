#include <stdio.h>
#include <assert.h>

#include "calculator.h"
#include "color.h"
#include "debug_info.h"

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

int GetG(size_t* p, char* s)
{
    ON_DEBUG(printf("GetG : p = %d\n", *p);)
    int val = GetE(p, s);

    if (s[*p] != '$')
        SyntaxError(*p, s);
    (*p)++;

    return val;
}

int GetE(size_t* p, char* s)
{
    ON_DEBUG(printf("GetE : p = %d\n", *p);)
    int val = GetT(p, s);

    while (s[*p] == '+' || s[*p] == '-')
    {
        int op = s[*p];
        (*p)++;

        int val_extra = GetT(p, s);

        if (op == '+') val += val_extra;
        if (op == '-') val -= val_extra;
    }

    return val;
}

int GetT(size_t* p, char* s)
{
    ON_DEBUG(printf("GetT : p = %d\n", *p);)
    int val = GetP(p, s);

    while (s[*p] == '*' || s[*p] == '/')
    {
        int op = s[*p];

        (*p)++;

        int val_extra = GetP(p, s);

        if (op == '*') val *= val_extra;
        if (op == '/') val /= val_extra;
    }

    return val;
}

int GetP(size_t* p, char* s)
{
    ON_DEBUG(printf("GetP : p = %d\n", *p);)
    if (s[*p] == '(')
    {
        (*p)++;
        int val = GetE(p, s);

        if (s[*p] != ')')
            SyntaxError(*p, s);
        (*p)++;

        return val;
    }
    else
        return GetN(p, s);
}

int GetN(size_t* p, char* s)
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

    return val;
}
