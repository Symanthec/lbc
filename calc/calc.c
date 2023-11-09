#include <calc/lang/lexer.h>
#include <calc/lang/parser.h>
#include <calc/utils.h>

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>



AST* calc_compileLine(calcState_t * const state, const char * const line) {
	TokenVector * tokens = langP_tokenizeLine(line);

	/* Check error presence */
	bool error = false;
	size_t errorIndex = 0;
	for (; errorIndex < tokens->size; ++errorIndex) {
		if (tokens->tokens[errorIndex].type == INVAL) {
			error = true;
			break;
		}
	}

	if (error) {
		/* set error on errorIndex'th token */
		Token err_token = tokens->tokens[errorIndex];
		error_t e = calc_mkError(SYNTAX, err_token.slice);
		calcP_setError(state, e);
		return NULL;
	} else {
		if (state->debugTokens != NULL)
			(state->debugTokens)(tokens);

		/* Parse valid lexemes */
		AST *tree = lang_parseTokens(*tokens);
		lang_freeTokens(tokens);
		return tree;
	}
}


value_t calc_doLine(calcState_t *state, const char* line) {
	AST * const tree = calc_compileLine(state, line);

	if (tree != NULL) {
		value_t result;
		if (tree->type == ERROR) {
			/* Set error */
			error_t e = tree->error;
			calcP_setError(state, e);
		} else {
			if (state->debugTree != NULL) 
				(state->debugTree)(tree);

			result = calc_run(state, tree);
			
			if (calc_getError(state).code == OK)
				calc_commit(state); /* Save changes on success */
		}

		lang_freeTree(tree);
		return result;
	} else {
		/* tree == NULL */
		return NIL;
	}
}
