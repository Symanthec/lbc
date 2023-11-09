#pragma once
#ifndef CALC_AST_H
#define CALC_AST_H


typedef struct AST AST;


#include <calc/lang/tokenvector.h>
#include <calc/error.h>
#include <calc/state.h>


/* AST node type enumeration */
typedef enum {
	OPERATION,	// Operation
	IDENTIFIER,	// Variable
	VALUE,		// Value (real or int)
	ERROR 		// Error indicator
} NodeType;


/* Abstract syntax tree (AST) node structure */
typedef struct AST {
	NodeType type;
	Token token; /* The token this node is made from */
	union {
		/* Either operands' nodes or error */
		struct {
			struct AST* lop;
			struct AST* rop;
		};
		error_t error;
	};
} AST;


/* Create new syntax tree node and returns it. */
extern AST* langP_newTree(void);


/* Releases the syntax tree */
extern void lang_freeTree(AST * const tree);


/* Return human-readable name of the nodes type */
extern const char* lang_nodeName(const NodeType type);


/*
** Evaluate AST using given state. Cast behaviour and variables values are
** inherited from state, error is also set there.
*/
extern value_t calc_run(calcState_t * const state, const AST * const tree);


#endif/* CALC_AST_H */
