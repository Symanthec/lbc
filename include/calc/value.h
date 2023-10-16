#pragma once
#ifndef CALC_VALUE_H
#define CALC_VALUE_H


enum _valueType {
	VALUE_NIL,
	VALUE_INT,
	VALUE_REAL,
};

typedef enum _valueType valueType_e;


typedef struct value_t {
	valueType_e type;
	union {
		long integer;
		double real;
	};	
} value_t;


extern const value_t NIL;


extern int calc_isValueNil(value_t *);


#endif//CALC_VALUE_H
