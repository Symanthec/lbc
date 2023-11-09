#pragma once
#ifndef CALC_ERROR_H
#define CALC_ERROR_H

#include <calc/lang/token.h>

enum Errors {
	OK,
	END_OF_FILE, // Unexpected end of line
	BAD_ID, // Unknown identifier
	SYNTAX, // Syntax error
	BAD_TYPE, // Mostly nil
	ZERO, // Divide by zero
	BAD_STATE, // state == NULL
};


typedef struct error_t {
	enum Errors code;
	Slice where;
} error_t;


#include <calc/state.h>


/*
** Default value which indicates that there is no error
** Though it's better to compare .code == OK
*/
extern const error_t NO_ERROR;


/*
** Returns error message for given error code (see "calc/error.c")
*/
extern const char * calc_getErrorMsg(const enum Errors code);


/*
** Returns error value of given calculator state
*/
extern error_t calc_getError(const calcState_t *state);


/*
** Sets error to given calculator state. If state is NULL, does nothing.
*/
extern void calcP_setError(calcState_t *state, const error_t error);


/*
** Clears any errors for given calculator state. In short, assign NO_ERROR
*/
extern void calc_clearError(calcState_t *state);


/*
** Utility function that creates error value for given code and token slice
*/
extern error_t calc_mkError(const enum Errors code, const Slice where);

#endif/* CALC_ERROR_H */
