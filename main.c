#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/errno.h>

#include <calc/calc.h>

#define EXIT_COMMAND	"quit"	
#define PROMPT_STRING	">>> "
#define MAXLINE 		256


static bool prompt(const char* prefix, char* dest, size_t maxLength) {
	fputs(prefix, stdout);
	return fgets(dest, maxLength, stdin) != NULL;
}


int main(int argc, char** argv)
{
	calcState_t *state = calc_newState();

	char line[MAXLINE] = {0};
	error_t err;
	while (prompt(PROMPT_STRING, line, MAXLINE))
	{
		// Exit sequence
		if (strstr(line, EXIT_COMMAND) != NULL)
			break;

		value_t result = calc_doLine(state, line);
		err = calc_getError(state);
		if (err.code != OK) {
			const char* msg = calc_getErrorMsg(err.code);
			fprintf(
				stderr,
				"Error occurred while evaluating expression: %s\n",
				msg);
			calc_clearError(state);
		}
	}

	// Enumerate identifiers, because why not?
	calc_freeState(state);
	return 0;
}
