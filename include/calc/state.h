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
	identList_t* staged; // side effect assignments
	error_t error;
} calcState_t;


extern calcState_t * calc_newState(void);


extern void calc_freeState(calcState_t *);


extern identList_t * calc_getIdentifiers(calcState_t *);


extern value_t calc_getValue(calcState_t *, const char *);


#endif//CALC_STATE_H
