#include <calc/calc.h>
#include <stdlib.h>


const error_t NO_ERROR = { OK, 0 };
const error_t BAD_STATE = { BAD_ID, -1 }; 


const char* ERROR_MESSAGES[] = {
	"No error",			// OK
	"Unexpected end of file",	// END_OF_FILE
	"Unknown identifier",		// BAD_ID
	"Syntax error",			// SYNTAX
	"Bad variable type",		// BAD_TYPE
	"Zero division",		// ZERO
};


const char* UNKNOWN_CODE_MSG = "Unknown code";


const char* calc_getErrorMsg(enum Errors code) {
	if (code < 0 || code > sizeof(ERROR_MESSAGES))
		return UNKNOWN_CODE_MSG;
	return ERROR_MESSAGES[code];
}


error_t calc_getError(calcState_t *state) {
	return (state != NULL) ? state->error: BAD_STATE;
}


void calcP_setError(calcState_t *state, error_t err) {
	if (state != NULL) {
		state->error = err;
	}
}


void calc_clearError(calcState_t *state) {
	if (state == NULL) return;
	state->error = NO_ERROR;
}