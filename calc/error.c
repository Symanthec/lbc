#include <calc/calc.h>
#include <stddef.h>


const error_t NO_ERROR = { OK, NULL };
static const error_t INVALID_STATE = { BAD_STATE, NULL }; 


const char* ERROR_MESSAGES[] = {
	"No error",					// OK
	"Unexpected end of file",	// END_OF_FILE
	"Unknown identifier",		// BAD_ID
	"Syntax error",				// SYNTAX
	"Bad type",					// BAD_TYPE
	"Zero division",			// ZERO
	"Invalid state",			// BAD_STATE
};


const char* UNKNOWN_CODE_MSG = "Unknown code";


const char* calc_getErrorMsg(const enum Errors code) {
	if (code < 0 || code > sizeof(ERROR_MESSAGES))
		return UNKNOWN_CODE_MSG;
	return ERROR_MESSAGES[code];
}


error_t calc_getError(const calcState_t *state) {
	return (state != NULL) ? state->error: INVALID_STATE;
}


void calcP_setError(calcState_t *state, const error_t err) {
	if (state != NULL) {
		state->error = err;
	}
}


void calc_clearError(calcState_t *state) {
	if (state == NULL) return;
	state->error = NO_ERROR;
}


error_t calc_mkError(const enum Errors code, const Slice where) {
	error_t e = { code, where };
	return  e;
}