#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "color.h"
#include "debug_info.h"
#include "differentiator.h"
#include "lexical_analysis.h"

Tokens* TokensCtor(void)
{
    Tokens* cur_tokens = (Tokens*) calloc(1, sizeof(Tokens));
    cur_tokens->tokens = (Node_t**) calloc(MAX_TOKEN_SIZE, sizeof(Node_t*));
    if (cur_tokens == NULL)
    {
        printf(RED("error in callocation:") "file : %s, func : %s\n", __FILE__, __func__);
        return NULL;
    }

    cur_tokens->length = 0;
    cur_tokens->capacity = MAX_TOKEN_SIZE;

    return cur_tokens;
}

Tokens* Tokenize(char* line, size_t* ptr)
{
    Tokens* command = TokensCtor();

    while(true)
    {
        SkipSpace(line, ptr);
        printf("line[ptr] = %c\n", line[*ptr]);
        if (line[*ptr] == '$')
        {
            _OP(command->tokens[command->length++], EOT);
            printf("detected End Of Translation\n");
            _OP(command->tokens[command->length], EOT);
            return command;
        }
        else
        {
            printf("getting token...\n");
            command->tokens[command->length++] = GetToken(line, ptr);
            printf("current tokens_ptr = %d\n", command->length);
        }
    }
    return command;
}

Node_t* GetToken(char* line, size_t* ptr)
{
    printf("line is : %s\n", line);
    SkipSpace(line, ptr);

    ON_DEBUG(printf(GREEN("getting token :\n\t"));)

    if (isdigit(line[*ptr]))
    {
        int number = GetNumber(line, ptr);\
        ON_DEBUG(printf("gotten number : %d\n", number);)
        Node_t* node = NULL;
        _NUM(node, number);
        return node;
    }
    else if (isalpha(line[*ptr]))
    {
        char* character = GetCharacter(line, ptr);
        ON_DEBUG(printf("gotten character : %s\n\t", character));
        Operation op;
        if ((op = DefineOperation(character)) != ERR)
        {
            Node_t* node = NULL;
            ON_DEBUG(printf("character is operation : %d = %s\n", op, DecryptOperation(op));)
            _OP(node, op);
            return node;
        }
        else
        {
            Node_t* node = NULL;
            ON_DEBUG(printf("character is variable\n");)
            _VAR(node, character);
            return node;
        }
    }
    else
    {
        char cmd[2];
        cmd[0] = line[*ptr];
        cmd[1] = '\0';
        Operation op = DefineOperation(cmd);
        if (op != ERR)
        {
            Node_t* node = NULL;
            ON_DEBUG(printf("character is operation : %d = %s\n", op, DecryptOperation(op));)
            _OP(node, op);
            (*ptr)++;
            return node;
        }
        else
        {
            return NULL;
        }
    }


    return NULL;
}

int GetNumber(char* line, size_t* ptr)
{
    int number = 0;
    while (isdigit(line[*ptr]))
    {
        number = number * 10 + line[*ptr] - '0';
        (*ptr)++;
    }

    return number;
}

char* GetCharacter(char* line, size_t* ptr)
{
    size_t char_size = MIN_SIZE;
    char* character = (char*) calloc(MIN_SIZE, sizeof(char));
    if (character == NULL)
    {
        printf(RED("error in callocation:") "file : %s, func : %s\n", __FILE__, __func__);
        return NULL;
    }

    size_t char_ip = 0;

    while (isalpha(line[*ptr]))
    {
        if (char_ip >= char_size)
        {
            char_size *= MUL_SIZE;
            char* t = (char*) realloc(character, char_size);
            if (t == NULL)
            {
                printf(RED("error in callocation:") "file : %s, func : %s\n", __FILE__, __func__);
                return NULL;
            }
            character = t;
        }
        character[char_ip++] = line[(*ptr)++];
    }
    if ((line[*ptr] == '(' || line[*ptr] == ')') && char_ip == 0)
    {
        character[char_ip++] = line[(*ptr)++];
    }
    character[char_ip] = '\0';

    return character;
}

void SkipSpace(char* s, size_t* ptr)
{
    while (isspace(s[*ptr])) (*ptr)++;
}

void DumpToken(Tokens* tokens)
{
    printf("tokens quantity = %d\n", tokens->length);
    for (size_t i = 0; i < tokens->length; i++)
    {
        printf("token %2d ", i);
    }
    printf("\n");

    for (size_t i = 0; i < tokens->length; i++)
    {
        if (tokens->tokens[i] == NULL)
        {
            printf(RED("nullptr "));
        }
        else
        {
            printf("%8s ", DecryptType(tokens->tokens[i]->type));
        }
    }
    printf("\n");
    for (size_t i = 0; i < tokens->length; i++)
    {
        if (tokens->tokens[i] == NULL)
        {
            continue;
        }
        switch(tokens->tokens[i]->type)
        {
            case OP:
                printf("%8s ", DecryptOperation(tokens->tokens[i]->data.op));
                break;
            case NUM:
                printf("%8d ", tokens->tokens[i]->data.num);
                break;
            case FLT:
                printf("%8lf ", tokens->tokens[i]->data.flt);
                break;
            case VAR:
                printf("%8s ", tokens->tokens[i]->data.var);
                break;
            case ERR_T:
            default:
                printf(RED("null\t "));
                break;
        }
    }
    printf("\n");

}

void DumpNode(Node_t* node)
{
    return ;
}

