#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "calc/calc.h"

const char* EXIT_COMMAND = "quit\n";
const char* PROMPT_STRING = ">>> ";

size_t line_caps = 2048;


static ssize_t prompt(const char* prefix, char** dest, size_t *caps) {
	printf("%s", prefix);
	ssize_t total_read = getline(dest, caps, stdin);
	return total_read;
}


int main(int argc, char** argv)
{
	calcState_t *state = calc_newState();

	char *line = malloc(line_caps);
	error_t err;
	while (prompt(PROMPT_STRING, &line, &line_caps) != -1) {

		// Exit sequence
		if (strcmp(line, EXIT_COMMAND) == 0)
			goto quit;


		value_t result = calc_doLine(state, line);

		if ((err = calc_getError(state)).code != OK) {
			const char* msg = calc_getErrorMsg(err.code);
			fprintf(stderr,
				"Error occurred while evaluating expression: %s\n",
				msg);
			calc_clearError(state);
		}
	}

getline_error:
	fprintf(stderr, "Error occurred while reading line...\n");
quit:
	// Enumerate ids
	calc_freeState(state);
	return 0;
}
