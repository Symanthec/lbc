#pragma once
#ifndef CALC_STATE_H
#define CALC_STATE_H


// Cyclic dependency solution ("error.h")
struct calcState_t;
typedef struct calcState_t calcState_t;

#include <calc/ident.h>
#include <calc/error.h>
#include <calc/lang/ast.h>


// syntax tree debug callback
typedef void (*calc_fnDbgTree) (AST*);
// token vector debug callback
typedef void (*calc_fnDbgTokens) (TokenList*);
// cast override callback
typedef value_t (*calc_fnCbCast) (value_t, value_t);


typedef struct calcState_t {
	identList_t* identifiers;

	 // future-proof for ops like i++, also absorbs error on execution
	identList_t* staging;

	error_t error;

	// various debug callbacks
	calc_fnDbgTree		debugTree;
	calc_fnDbgTokens 	debugTokens;
	calc_fnCbCast 		valueCaster;
} calcState_t;


extern calcState_t * calc_newState(void);


extern void calc_freeState(calcState_t *);


extern value_t calc_getValue(calcState_t *, const char *);


// Assign value to given identifier and returns previous value
extern value_t calc_setValue(calcState_t *, const char*, value_t);


extern void calcP_commit(calcState_t *);


// Callbacks setters
extern calc_fnDbgTree 	calcU_onDebugTree(calcState_t *, calc_fnDbgTree);
extern calc_fnDbgTokens calcU_onDebugTokens(calcState_t *, calc_fnDbgTokens);
extern calc_fnCbCast	calcU_castBehaviour(calcState_t *, calc_fnCbCast);

#endif//CALC_STATE_H
