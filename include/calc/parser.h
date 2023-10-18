#pragma once
#ifndef CALC_PARSER_H
#define CALC_PARSER_H

#include <calc/lang/lexer.h>
#include <calc/lang/token.h>
#include <calc/error.h>


typedef enum {
	OP,
	VALUE,
	IDENT,
} NodeType;

enum {
	SUCCESS, FAILURE
};


typedef struct AST {
	NodeType type;
	union {
		Slice slice; // identifier or value
		struct {
			TokenType op; // operation
			struct AST* left;
		    struct AST* right;
		};
	};
} AST;


typedef struct {
	int success;
	union {
		AST* tree;
		error_t error;
	};
} ParserResult;


/** Parse tokens and return Abstract Syntax Tree, which may then be executed by state */
ParserResult calcP_parseTokens(TokenList* tokens);


#endif//CALC_PARSER_H
