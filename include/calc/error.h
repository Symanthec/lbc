#pragma once
#ifndef CALC_ERROR_H
#define CALC_ERROR_H


enum Errors {
	OK,
	END_OF_FILE, // Unexpected end of line
	BAD_ID, // Unknown identifier
	SYNTAX, // Syntax error
	BAD_TYPE, // Mostly nil
	ZERO // Divide by zero
};


typedef struct error_t {
	enum Errors code;
	unsigned at;
} error_t;


#include "state.h"


extern const error_t NO_ERROR;
extern const error_t BAD_STATE; // passed when state is NULL


extern const char * calc_getErrorMsg(enum Errors);

extern error_t calc_getError(calcState_t*);

extern void calcP_setError(calcState_t*, error_t);

extern void calc_clearError(calcState_t*);


#endif//CALC_ERROR_H
