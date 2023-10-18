#pragma once
#ifndef CALC_LANG_TOKEN_H
#define CALC_LANG_TOKEN_H

#include <stddef.h>

// Substring inside another string
typedef struct {
	const char* start;
	size_t length;
} Slice;

// Enumeration of possible token types
typedef enum {
	T_NIL, 		// "nil" from Lua
	T_IDENT,		// variable
	T_REAL,		// "double" value
	T_INTEGER,	// "long" value

	// Math operations
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	EQU,

	// Parentheses
	LPAREN,
	RPAREN,
	
	// Some intermediate tokens, which must not occur in
	// final VALID sequence of tokens
	END,	// End of line
	WHITE,	// Whitespace character 
	MULTI,	// pickSymbol() indicator of identifier or number
	INVAL	// Invalid character, error indicator
} TokenType;

typedef struct Token {
	TokenType type;
	Slice slice;
} Token;

// Return human readable name of token
const char* lang_tokenName(TokenType type);

#endif//CALC_LANG_TOKEN_H