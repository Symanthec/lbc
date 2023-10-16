#include <calc/calc.h>
#include <stdlib.h>


calcState_t * calc_newState(void) {
	calcState_t *state = (calcState_t*) malloc(sizeof(calcState_t));
	
	state->identifiers = calcP_newIdentList();
	state->staged = calcP_newIdentList();
	state->error = NO_ERROR;

	return state;
}


void calc_freeState(calcState_t *state) {
	if (state == NULL) return;

	calcP_freeIdentList(state->identifiers);
	calcP_freeIdentList(state->staged);
	free(state);
}


identList_t * calc_getIdentifiers(calcState_t *state) {
	return (state != NULL) ? state->identifiers : NULL;
}


value_t calc_getValue(calcState_t *state, const char* id_name) {
	if (state == NULL) return NIL;

	identList_t *identifiers = calc_getIdentifiers(state);
	return calc_valueOf(identifiers, id_name);
}
