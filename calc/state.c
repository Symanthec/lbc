#include <calc/calc.h>
#include <stdlib.h>
#include <stdio.h>


calcState_t * calc_newState(void) {
	calcState_t *state = (calcState_t*) malloc(sizeof(calcState_t));
	state->identifiers = calcP_newIdentList();
	state->staging = calcP_newIdentList();
	state->error = NO_ERROR;
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
