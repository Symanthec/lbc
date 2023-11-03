#include <calc/lang/ast.h>
#include <calc/ops.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>


#define _ALLOC_VAR_NAME(slice, size, dst)		\
			size_t size = slice.length;			\
			char dst[size + 1];					\
			dst[size] = '\0';					\
			strncpy(dst, slice.start, size);


// Create new syntax tree
AST* lang_newTree(void) {
	AST *tree = malloc(sizeof(AST));
	tree->lop = NULL;
	tree->rop = NULL;
	return tree;
}


// Destroy all tree
void lang_freeTree(AST * tree) {
	if (tree == NULL) return;
	if (tree->type == OPERATION) {
		lang_freeTree(tree->lop);
		lang_freeTree(tree->rop);
	}
	free(tree);
}


const char* nodeNames[4] = {
	"OPERATION",
	"IDENTIFIER",
	"VALUE",
	"ERROR"
};


const char* lang_nodeName(NodeType type) {
	if (OPERATION <= type && type <= ERROR)
		return nodeNames[type];
	else
		return "<NULL>";
}


static value_t runOp(calcState_t * state, TokenType op, value_t lop, value_t rop) {
	switch (op) {
	case ADD:
		return calc_add(lop, rop);
	case SUB:
		return calc_sub(lop, rop);
	case MUL:
		return calc_mul(lop, rop);
	case DIV:
		return calc_div(lop, rop);
	case MOD:
		return calc_mod(lop, rop);
	default:
		return NIL;
	}
}


static value_t getValue(Token t) {
	if (t.type == T_NIL)
		return NIL;

	long integer = 0;
	double real = 0;
	bool isReal = false;
	bool sign = false; // positive right now

	const char* in = t.slice.start;
	if (*in == '-' || *in == '+') {
		sign = *in == '-';
		in += 1;
	}

	// integer part
	while (isdigit(*in)) {
		integer = 10 * integer + (*in - '0');
		in += 1;
	};

	// fractional part
	if (*in == '.') {
		isReal = true;
		real = (double) integer;
		double frac = 0.1;

		in += 1;
		while (isdigit(*in)) {
			real += frac * (*in - '0');
			frac /= 10;
			in += 1;
		}
	}

	value_t v = { .type = (isReal ? VALUE_REAL: VALUE_INT) };
	if (isReal)
		v.real = sign ? -real: real;
	else
		v.integer = sign ? -integer: integer;
	return v;
}


// Evaluate expression using identifiers of given calcState_t
// It is assumed that parser checks for all errors except identifiers
value_t calcP_run(calcState_t * state, AST* tree) {
	if (state == NULL || tree == NULL) return NIL;

	value_t result = NIL;
	NodeType type = tree->type;
	if (type == VALUE)
	{ 
		result = getValue(tree->token);
	}
	else if (type == IDENTIFIER)
	{
		// cut substring from
		_ALLOC_VAR_NAME(tree->token.slice, size, name);
		// return variable value
		result = calc_getValue(
			state, 
			name);

		if (calc_isValueNil(&result)) {
			// push BAD_ID error
			error_t e = calc_mkError(BAD_ID, tree->token.slice);
			calcP_setError(state, e);
		}
	} 
	else if (type == OPERATION)
	{
		// if assignment 
		value_t right = calcP_run(state, tree->rop);
		Token op = tree->token;
		if (op.type == EQU) {
			// assignment
			Token lt = tree->lop->token;
			_ALLOC_VAR_NAME(lt.slice, size, name);
			
			// recast back into previous type
			// could also be put into diff option
			value_t prev = calc_getValue(state, name);
			right = (state->valueCaster)(prev, right);

			calc_setValue(state, name, right);
			result = right;
		} else {
			value_t left = calcP_run(state, tree->lop);
			result = runOp(
				state,
				op.type,
				left,
				right);
		}
	}
	
	return result;
}