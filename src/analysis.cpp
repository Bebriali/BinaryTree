#include <stdio.h>

#include "lexical_analysis.h"
#include "syntactic_analysis.h"

Node_t** Tokenize(char* line, size_t* ptr)
{
    Node_t* tokens[MAX_TOKEN_SIZE];
    size_t tokens_ptr = 0;

    if (line[*ptr] == '$')
    {
        return tokens;
    }
    else
    {
        tokens[tokens_ptr++] = GetToken(line, ptr);
    }
}

Node_t* GetToken(char* line, size_t* ptr)
{
    SkipSpace();

    if (isdigit(line[*ptr]))
    {
        return _NUM(GetNumber(line, ptr));
    }
    else if (isalpha(line[*ptr))
    {
        char* character = GetCharacter(line, ptr);
        Operation op;
        if ((op = DefineOperation(character)) != ERR)
        {
            return _OP(op);
        }
        else
        {
            return _VAR(character);
        }
    }
}

void SkipSpace(void)
{
    int c = getchar();
    while (isspace(c)) c = getchar();
    ungetc(c, stdin);
}
