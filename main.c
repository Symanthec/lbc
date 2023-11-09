#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/errno.h>

#include <calc/calc.h>
#include <calc/utils.h>


#define EXIT_COMMAND	"exit"	
#define PROMPT_STRING	">>> "
#define EMBED_DEL		"del"
#define MAXLINE 		256


static bool prompt(const char* prefix, char* dest, size_t maxLength) {
	fputs(prefix, stdout);
	return fgets(dest, maxLength, stdin) != NULL;
}


int main(int argc, char** argv)
{
	calcState_t *state = calc_newState();
	// calcU_onDebugTree(state, calcU_printTree);
	// calcU_onDebugTokens(state, calcU_printTokens);

	char line[MAXLINE] = {0}, del_pending[MAXLINE] = {0};
	error_t err;
	while (prompt(PROMPT_STRING, line, MAXLINE))
	{
		if (strlen(line) <= 1) continue; /* '\0' or '\n' */

		// Exit sequence
		if (strstr(line, EXIT_COMMAND) != NULL)
			break;

		// embed "delete x" functionality over "x = nil"	
		if (sscanf(line, EMBED_DEL " %s", del_pending) > 0) 
		{
			calc_setValue(state, del_pending, NIL);
			calc_commit(state);
		} 
		else 
		{
			// single state execution
			value_t result = calc_doLine(state, line);

			err = calc_getError(state);
			if (err.code == OK) {
				calc_commit(state);
				calcU_printValue(result);
				putchar('\n');
			} else {
				printf("Error occurred...\n");
				calcU_printError(state, line);
				calc_clearError(state);
			}
		}

		// Enumerate identifiers, because why not?
		unsigned count = calcU_printIdentifiers(state->identifiers);
		printf(" Total: %u\n", count);
	}

	calc_freeState(state);
	return 0;
}
