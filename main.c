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


static void printError(const char *line, error_t err) {
	fprintf(stderr,
		"[Calc error]: %s at %zu\n",
		calc_getErrorMsg(err.code),
		err.slice.start - line + 1);

	size_t len = strlen(line);
	fprintf(stderr, "%s", line);
	for (size_t i = 0; i < err.slice.start - line && i < len; ++i)
		fputc(' ', stderr);
	for (size_t i = 0; i < err.slice.length && i < len; ++i)
		fputc('^', stderr);
	fputc('\n', stderr);
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
		if (strlen(line) <= 0) continue; 

		// Exit sequence
		if (strstr(line, EXIT_COMMAND) != NULL)
			break;

		// embed "delete x" functionality over "x = nil"	
		if (sscanf(line, EMBED_DEL " %s", del_pending) > 0) 
		{
			calc_setValue(state, del_pending, NIL);
			calcP_commit(state);
		} 
		else 
		{
			value_t result = calc_doLine(state, line);
			err = calc_getError(state);
			if (err.code == OK) {
				calcU_printValue(result);
				putchar('\n');
			} else {
				printError(line, err);
				calc_clearError(state);
			}
		}

		calcU_printIdentifiers(state->identifiers);
		putchar('\n');
	}

	// Enumerate identifiers, because why not?
	calc_freeState(state);
	return 0;
}
