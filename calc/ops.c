#include <calc/ops.h>
#include <stdbool.h>

// ========= "Functional" macrodefinitions =========


#define PICK_TYPE(x, y) ((x).type == VALUE_INT && (y).type == VALUE_INT ? \
		VALUE_INT: \
		VALUE_REAL )

#define PICK_VAL(x) ((x).type == VALUE_INT ? (x).integer : (x).real)


#define OP(op, lop, rop, val) \
	if (lop.type == VALUE_NIL || rop.type == VALUE_NIL) return NIL; \
	value_t val;													\
	if (lop.type == VALUE_INT && rop.type == VALUE_INT) {			\
		val.type = VALUE_INT;										\
		val.integer = PICK_VAL(lop) op PICK_VAL(rop); 				\
	} else {														\
		val.type = VALUE_REAL;										\
		val.real = PICK_VAL(lop) op PICK_VAL(rop);					\
	}


// ========= Math operations =========


value_t calc_add(value_t lop, value_t rop) {
	OP(+, lop, rop, val);
	return val;
}


value_t calc_sub(value_t lop, value_t rop) {
	OP(-, lop, rop, val);
	return val;
}


value_t calc_mul(value_t lop, value_t rop) {
	OP(*, lop, rop, val);
	return val;
}


#define ABS(x) ((x) < 0 ? -(x) : (x))


value_t calc_div(value_t lop, value_t rop) {
	if (lop.type == VALUE_NIL || rop.type == VALUE_NIL) return NIL;

	// APPLY GCC DIVISION SEMANTICS
	bool sign = PICK_VAL(lop) < 0 ^ PICK_VAL(rop) < 0;
	value_t val;
	if (lop.type == VALUE_INT && rop.type == VALUE_INT) {
		val.type = VALUE_INT;
		val.integer = ABS(lop.integer) / ABS(rop.integer);
		if (sign) val.integer = -val.integer;
	} else {
		val.type = VALUE_REAL;
		double lhs = lop.type == VALUE_INT ? lop.integer : lop.real;
		double rhs = rop.type == VALUE_INT ? rop.integer : rop.real;
		val.real = ABS(lhs) / ABS(rhs);
		if (sign) val.real = -val.real;
	}

	return val;
}


value_t calc_mod(value_t lop, value_t rop) {
	if (lop.type == VALUE_NIL || rop.type == VALUE_NIL) return NIL;

	value_t div = calc_div(lop, rop);
	value_t val;
	if (lop.type == VALUE_INT && rop.type == VALUE_INT) {
		val.type = VALUE_INT;
		long div = ABS(lop.integer) / ABS(rop.integer);
		val.integer = PICK_VAL(lop) - div * PICK_VAL(rop);
	} else {
		val.type = VALUE_REAL;
		long lhs = lop.type == VALUE_INT ? lop.integer : lop.real;
		long rhs = rop.type == VALUE_INT ? rop.integer : rop.real;
		long div = ABS(lhs) / ABS(rhs);
		val.real = PICK_VAL(lop) - div * PICK_VAL(rop);
	}
	return val;
}
