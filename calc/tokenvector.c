#include <stdlib.h>
#include <calc/lang/tokenvector.h>

#define VEC_SIZE 10


TokenVector* lang_newTokens(void) {
	TokenVector *vec = malloc(sizeof(TokenVector));
	vec->capacity = VEC_SIZE;
	vec->tokens = malloc(sizeof(Token) * VEC_SIZE);
	vec->size = 0;
	return vec;
}


TokenVector* lang_pushToken(TokenVector* vec, Token t) {
	if (vec == NULL) { // create new vector if NULL
		vec = lang_newTokens();
	}

	if (vec->size == vec->capacity) { // expand vec if small
		vec->tokens = realloc(
			vec->tokens,
			(vec->capacity + VEC_SIZE) * sizeof(Token));
	}

	vec->tokens[vec->size] = t;
	vec->size += 1;
	return vec;
}


void lang_freeTokens(TokenVector* vec) {
	if (vec != NULL) {
		free(vec->tokens);
		free(vec);
	}
}