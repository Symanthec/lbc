#pragma once
#ifndef CALC_IDENT_H
#define CALC_IDENT_H

#include <calc/value.h>

/* Compile-time option for maximum variable name length */
#ifndef CALC_IDENTIFIER_LENGTH
#define CALC_IDENTIFIER_LENGTH 256
#endif

/* Identifier type data structure */
typedef struct {
	value_t value;
	char name[CALC_IDENTIFIER_LENGTH];
} ident_t;


/* Binary search tree node structure */
typedef struct identList_t {
	ident_t identifier;
	struct identList_t *left;
    struct identList_t *right;
} identList_t;


/*
** Creates new binary search tree node and fills it with a stub values
** NOTE: You don't need to assign .left and .right to NULL - they already are
*/
extern identList_t * calcP_newIdentList(void);

/* 
** Frees the entire binary search tree
** NOTE: To remove only one node, assign .left = .right = NULL first
*/
extern void calcP_freeIdentList(identList_t *);

/*
** Return current value of variable named 'name' or NIL if doesn't exist.
*/
extern value_t calc_valueOf(const identList_t *list, const char *name);

/*
** Assign new value to identifier and create node if necessary
** Compared to setIdentifier(), it may assign 'nil' to value,
** whereas setIdentifier() to such variable will popIdentifier()
*/
extern value_t calcP_setRawIdent(identList_t*   list,
								const char*		name,
								const value_t	val);

/*
** Assign new value to identifier and create node if necessary
** Compared to setRawIdent() it will popIdentifier() which is set to 'nil'
** instead of 'raw' assigning a NIL value to variable
*/
extern value_t calc_setIdentifier(identList_t*	list, 
								const char*		name,
								const value_t	val);

/*
** Delete identifier and return its value
*/
extern value_t calc_popIdentifier(identList_t*  list,
                                const char*     name);

#endif/* CALC_IDENT_H */
