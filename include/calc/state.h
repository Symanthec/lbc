#pragma once
#ifndef CALC_STATE_H
#define CALC_STATE_H


/* Cyclic dependency solution for "calc/error.h" */
typedef struct calcState_t calcState_t;
// typedef struct calcState_t calcState_t;

#include <calc/ident.h>
#include <calc/error.h>
#include <calc/lang/ast.h>


/*
** Callback function that is invoked when AST is created with
** given state. State there serves as error and callback container.
*/
typedef void (* calc_fnDbgTree) (const AST * const);


/*
** Callback function that is invoked when token list is created with
** given state. State there serves as error and callback container.
*/
typedef void (* calc_fnDbgTokens) (const TokenList * const);



/*
** Callback function for type casting. 
** Receives old variable value and the value to be assigned next.
** Returns new (casted) value which is then assigned to variable.
*/
typedef value_t (* calc_fnCbCast) (const value_t, const value_t);


/*
** Independent instance of calculator. It stores variables, errors and 
** some specific callbacks. 
** NOTE: Variables and errors may differ between states.
*/
typedef struct calcState_t {
	identList_t* identifiers;	/* Variables storage */
	identList_t* staging;		/* List to store variable's changes */
	error_t error;				/* Last created error */

	calc_fnDbgTree		debugTree;	 /* AST create callback */
	calc_fnDbgTokens 	debugTokens; /* Token list create callback */
	calc_fnCbCast 		valueCaster; /* Type cast callback */
} calcState_t;


/* Creates new calculator state. */
extern calcState_t * calc_newState(void);


/* Releases calculator state. */
extern void calc_freeState(calcState_t * const state);


/*
** Returns current value of variable. Current value doesn't change immediately
** after calling calc_setValue() - changes should be calc_commit()-ed first.
*/
extern value_t calc_getValue(const calcState_t * const 	state,
							const char * const 			name);


/*
** Adds change to variables value and creates one if necessary. 
** NOTE: It doesn't change variables value immediately - changes need to be
** committed. See calcP_commit()
*/
extern value_t calc_setValue(calcState_t * const 	state,
							const char * const 		name,
							const value_t			val);


/*
** Commits changes in variables to current variable list.
** When expression invokes error, its changes in variables will be aborted,
** otherwise commit is used and changes are saved.
*/
extern void calc_commit(calcState_t * const state);


/* 
** Sets new callback which is invoked by other functions when AST is created
** for given state. Returns old callback.
*/
extern calc_fnDbgTree calcU_onDebugTree(calcState_t * const		state,
										const calc_fnDbgTree	callback);



/*
** Sets new callback which is invoked by other functions when token list is
** created for given state. Returns old callback.
*/
extern calc_fnDbgTokens calcU_onDebugTokens(calcState_t * const 	state,
											const calc_fnDbgTokens 	callback);


/*
** Sets instances value cast behaviour. By default value is casted to the old
** type of variable. This behaviour may be changed with this callback.
** Returns old cast callback.
*/
extern calc_fnCbCast calcU_castBehaviour(calcState_t * const 	state,
										const calc_fnCbCast 	callback);

#endif/* CALC_STATE_H */
