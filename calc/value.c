#include <calc/value.h>


const value_t NIL = { VALUE_NIL, 0 };


bool calc_isValueNil(const value_t *val) {
	return val->type == VALUE_NIL;
}
