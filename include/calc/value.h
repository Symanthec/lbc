#pragma once
#ifndef CALC_VALUE_H
#define CALC_VALUE_H

#include <stdbool.h>


/* Enumeration of possible value types */
typedef enum valueType {
	VALUE_NIL, /* Special value type, equivalent of 'nil' from Lua*/
	VALUE_INT,
	VALUE_REAL,
} valueType_e;


/* Value storage type. Contains its .type and value (.integer or .real). */
typedef struct value_t {
	valueType_e type;
	union {
		long integer;
		double real;
	};	
} value_t;


/*
** Special value for variables. 
** 1. Serves as error indicator
** 2. Combined with setIdentifier() will popIdentifier() with given name
** 3. Erroneous math operations (including ones with NIL) return NIL
**	  P.S. Assuming bad calculations should not be used later, variable
**	  will be deleted and expressions which use it will return error
*/
extern const value_t NIL;


/*
** Boolean function to check whether some value is NIL. Use it instead of
** comparing (.type == VALUE_NIL) as things can change
*/
extern bool calc_isValueNil(const value_t * value);

#endif/* CALC_VALUE_H */
