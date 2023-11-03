#include <calc/calc.h>
#include <stdlib.h>


static void fnDbgTree(AST* tree) {return;}
static void fnDbgTokens(TokenList* list) {return;}
static value_t fnCbCast(value_t old, value_t right) {
	if (old.type == VALUE_INT && right.type == VALUE_REAL) {
		right.type = VALUE_INT;
		right.integer = right.real;
	} else if (old.type == VALUE_REAL && right.type == VALUE_INT) {
		right.type = VALUE_REAL;
		right.real = right.integer;
	}
	return right;
}


calcState_t * calc_newState(void) {
	calcState_t *state = (calcState_t*) malloc(sizeof(calcState_t));
	state->identifiers = calcP_newIdentList();
	state->staging = calcP_newIdentList();
	state->error = NO_ERROR;
	// default callbacks
	calcU_onDebugTree(state, fnDbgTree);
	calcU_onDebugTokens(state, fnDbgTokens);
	calcU_castBehaviour(state, fnCbCast);
	return state;
}


void calc_freeState(calcState_t *state) {
	if (state == NULL) return;
	calcP_freeIdentList(state->identifiers);
	calcP_freeIdentList(state->staging);
	free(state);
}


value_t calc_getValue(calcState_t *state, const char* name) {
	if (state == NULL) return NIL;
	return calc_valueOf(state->identifiers, name);
}


value_t calc_setValue(calcState_t *state, const char* name, value_t v) {
	if (state == NULL || name == NULL) return NIL;
	return calcP_setRawIdent(state->staging, name, v);
}


void __calcP_commit(identList_t *from, identList_t *to) {
	if (from == NULL || to == NULL) return;

	if (from->left != NULL)
		__calcP_commit(from->left, to);
	if (from->right != NULL)
		__calcP_commit(from->right, to);
	calc_setIdentifier(to, from->identifier.name, from->identifier.value);
}


void calcP_commit(calcState_t* state) {
	__calcP_commit(state->staging, state->identifiers);
	calcP_freeIdentList(state->staging);
	state->staging = calcP_newIdentList();
}


#define REPLACE_CB(type, where, name, new)	\
	if (new != NULL && where != NULL) { 	\
		type old = where->name;				\
		where->name = new;					\
		return old;							\
	}										\
	return NULL;


calc_fnDbgTree calcU_onDebugTree(calcState_t *state, calc_fnDbgTree cb) {
	REPLACE_CB(calc_fnDbgTree, state, debugTree, cb);
}


calc_fnDbgTokens calcU_onDebugTokens(calcState_t *state, calc_fnDbgTokens cb) {
	REPLACE_CB(calc_fnDbgTokens, state, debugTokens, cb);
}


calc_fnCbCast calcU_castBehaviour(calcState_t *state, calc_fnCbCast cb) {
	REPLACE_CB(calc_fnCbCast, state, valueCaster, cb);
}

#undef REPLACE_CB
