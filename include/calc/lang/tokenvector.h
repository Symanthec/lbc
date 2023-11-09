#pragma once
#ifndef CALC_LANG_TOKEN_VECTOR_H
#define CALC_LANG_TOKEN_VECTOR_H

#include <calc/lang/token.h>
#include <stddef.h>


/* Vector of tokens structure */
typedef struct {
	Token* tokens;
	size_t size, capacity;
} TokenVector;


/* Type alias needed to indicate that vector should be treated as immutable. */
typedef TokenVector TokenList;


/* Creates vector list for tokens */
extern TokenVector* lang_newTokens(void);


/*
** Pushes new token into given vector
**  - If vector is NULL, creates a new vector and returns it
**  - Automatically resizes vector when needed 
** Returns pointer to original vector if not NULL or newly created one 
*/
extern TokenVector* lang_pushToken(TokenVector *vec, const Token t);


/* Releases vector of tokens */
extern void lang_freeTokens(TokenList * const vec);


#endif/* CALC_LANG_TOKEN_VECTOR_H */
