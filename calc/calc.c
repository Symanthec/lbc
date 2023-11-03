#include <calc/lang/lexer.h>
#include <calc/lang/parser.h>
#include <calc/utils.h>

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

value_t calc_doLine(calcState_t *state, const char* in) {
	TokenVector *tokens = lang_tokenizeLine(in);

	// check error presence
	bool error = false;
	size_t errToken = 0;
	for (; errToken < tokens->size; ++errToken) {
		if (tokens->tokens[errToken].type == INVAL) {
			error = true;
			break;
		}
	}

	value_t result = NIL;
	if (error) {
		// set error on (err - start)th symbol
		Token err_token = tokens->tokens[errToken];
		error_t e = { SYNTAX, err_token.slice };
		calcP_setError(state, e);
	} else {
		// calcU_printTokens(tokens, stdout);
		// parse valid lexemes
		AST *parseResult = lang_parseTokens(*tokens);
		if (parseResult != NULL) {
			if (parseResult->type == ERROR) {
				// set error
				error_t e = parseResult->error;
				calcP_setError(state, e);
			} else {
				calcU_printTree(parseResult, stdout);
				// print value
				result = calcP_run(state, parseResult);

				if (calc_getError(state).code == OK)
					calcP_commit(state); // save changes on success
			}

		}
	
		lang_freeTree(parseResult);
	}

	lang_freeTokens(tokens);
	return result;
}
