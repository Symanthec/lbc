#include <calc/ops.h>

// ========= "Functional" macrodefinitions =========


#define PICK_TYPE(x, y) ((x).type == VALUE_INT && (y).type == VALUE_INT ? \
		VALUE_INT: \
		VALUE_REAL )

#define PICK_VAL(x) ((x).type == VALUE_INT ? (x).integer : (x).real)


#define OP(op, lop, rop) { PICK_TYPE(lop, rop), PICK_VAL(lop) op PICK_VAL(rop) }


// ========= Math operations =========


value_t calc_add(value_t lop, value_t rop) {
	value_t v = OP(+, lop, rop);
	return v;
}


value_t calc_sub(value_t lop, value_t rop) {
	value_t v = OP(-, lop, rop);
	return v;
}


value_t calc_mul(value_t lop, value_t rop) {
	value_t v = OP(*, lop, rop);
	return v;
}


#if !defined(GCC_DIV)
value_t calc_div(value_t lop, value_t rop) {
	value_t v = OP(/, lop, rop);
	return v;
}

value_t calc_mod(value_t lop, value_t rop) {
	value_t v;
	if (PICK_TYPE(lop, rop) != VALUE_INT) {
		v.type = VALUE_NIL;
	} else {
		v.type = VALUE_INT;
		v.integer = lop.integer % rop.integer;
	}
	return v;
}
#else // GCC division semantics
// TODO: GCC semantics

value_t calc_div(value_t lop, value_t rop) {
	value_t v = OP(/, lop, rop);
	return v;
}

value_t calc_mod(value_t lop, value_t rop) {
	value_t v;
	if (PICK_TYPE(lop, rop) != VALUE_INT) {
		v.type = VALUE_NIL;
	} else {
		v.type = VALUE_INT;
		v.integer = lop.integer % rop.integer;
	}
	return v;
}
#endif// GCC division semantics
