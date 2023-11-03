#pragma once
#ifndef CALC_LANG_TOKEN_VECTOR_H
#define CALC_LANG_TOKEN_VECTOR_H

#include <calc/lang/token.h>
#include <stddef.h>

typedef struct {
	Token* tokens;
	size_t size, capacity;
} TokenVector;


// Vector should rather be considered a list outside the lexer
typedef TokenVector TokenList;


// Create vector for tokens
extern TokenVector* lang_newTokens(void);


// Push token into vector
// If vec == NULL, creates new vector
// Automatically resizes vector when needed
// Returns pointer to new vector or original (!= NULL)
extern TokenVector* lang_pushToken(TokenVector *vec, Token t);


// Destroy vector of tokens
extern void lang_freeTokens(TokenList* vec);


#endif//CALC_LANG_TOKEN_VECTOR_H
