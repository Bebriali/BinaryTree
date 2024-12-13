#ifndef LEXICAL_ANALYSIS_H
#define LEXICAL_ANALYSIS_H

#include "tree_struct.h"

const int MAX_TOKEN_SIZE = 100;

Node_t** Tokenize(char* line, size_t* p);

Node_t* GetToken(char* line, size_t* p, Node_t** tokens);

#endif
