#pragma once
#ifndef CALC_AST_H
#define CALC_AST_H


struct AST;
typedef struct AST AST;


#include <calc/lang/tokenvector.h>
#include <calc/error.h>
#include <calc/state.h>





typedef enum {
	OPERATION,	// Operation
	IDENTIFIER,	// Variable
	VALUE,		// Value (real or int)
	ERROR 		// Error indicator
} NodeType;


typedef struct AST {
	NodeType type;
	Token token;
	union {
		// either nodes or error
		struct {
			struct AST* lop;
			struct AST* rop;
		};
		error_t error;
	};
} AST;


// Create new syntax tree
extern AST* lang_newTree();


// Destroy all tree
extern void lang_freeTree(AST *);


// Node name
extern const char* lang_nodeName(NodeType t);


// Evaluate expression using identifiers of given calcState_t
extern value_t calcP_run(calcState_t *, AST*);


#endif//CALC_AST_H
