#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include "tree_struct.h"

const int MAX_TOKEN_SIZE = 100;

enum CharParam
{
    MIN_SIZE = 10,
    MUL_SIZE = 2
};

typedef struct Tokens
{
    Node_t** tokens;
    size_t length;
    size_t capacity;
} Tokens;

Tokens* TokensCtor(void);

Tokens* Tokenize(char* line, size_t* ptr);

Node_t* GetToken(char* line, size_t* ptr);

void DumpToken(Tokens* tokens);
void DumpNode(Node_t* node);

int GetNumber(char* line, size_t* ptr);
char* GetCharacter(char* line, size_t* ptr);
void SkipSpace(char* s, size_t* ptr);

#endif
