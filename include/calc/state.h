#pragma once
#ifndef CALC_STATE_H
#define CALC_STATE_H


// Cyclic dependency solution ("error.h")
struct calcState_t;
typedef struct calcState_t calcState_t;


#include <calc/ident.h>
#include <calc/error.h>


typedef struct calcState_t {
	identList_t* identifiers;

	 // future-proof for ops like i++, also absorbs error on execution
	identList_t* staging;

	error_t error;

	// various debug callbacks
} calcState_t;


extern calcState_t * calc_newState(void);


extern void calc_freeState(calcState_t *);


extern value_t calc_getValue(calcState_t *, const char *);


// Assign value to given identifier and returns previous value
extern value_t calc_setValue(calcState_t *, const char*, value_t);


extern void calcP_commit(calcState_t *);


#endif//CALC_STATE_H
