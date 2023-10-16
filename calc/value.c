#include <calc/value.h>


const value_t NIL = { VALUE_NIL, 0 };


int calc_isValueNil(value_t *val) {
	return val->type == VALUE_NIL;
}
