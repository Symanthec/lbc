#pragma once
#ifndef CALC_LANG_TOKEN_H
#define CALC_LANG_TOKEN_H

#include <stddef.h>

/* Substring inside another string */
typedef struct {
	const char* start;
	size_t length;
} Slice;


/* Enumeration of possible token types */
typedef enum {
	FALSE,		/* False value for use in conditions	*/

	/* MULTI-character token types */

	T_NIL, 		/* "nil" from Lua (becomes VALUE_NIL)	*/
	T_IDENT,	/* Name identifier */
	T_REAL,		/* Real value (becomes VALUE_REAL)   	*/
	T_INTEGER,	/* Integer value (becomes VALUE_INT) 	*/

	/* Math operations */
	
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
	EQU,

	/* Parentheses */

	LPAREN,
	RPAREN,
	
	/* Intermediate tokens which must not occur in VALID list of tokens */

	END, 	/* End of line/sequence indicator 					*/ 
	WHITE,	/* Whitespace character (to be skipped) 			*/
	MULTI,	/* MULTI-char token indicator for inner use 		*/
	INVAL	/* Invalid (unexpected) character, error indicator 	*/
} TokenType;


/* Token data structure. Contains type and "slice" where it occurs */
typedef struct Token {
	TokenType type;
	Slice slice;
} Token;


/* Return human-readable name of token */
extern const char* lang_tokenName(const TokenType type);

#endif/* CALC_LANG_TOKEN_H */
