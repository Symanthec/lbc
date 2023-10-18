#include <calc/calc.h>
#include <calc/lang/lexer.h>
#include <calc/parser.h>

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
		error_t e = { SYNTAX, err_token.slice.start - in };
		calcP_setError(state, e);
	} else {
		// parse valid lexemes
		ParserResult parseResult = calcP_parseTokens(tokens);

		if (parseResult.success != OK) {
			error_t err = parseResult.error;
			calcP_setError(state, err);
		} else {
			AST* ast = parseResult.tree;
			result = calc_doAST(state, ast);
			// free ast
		}
	}

	lang_freeTokens(tokens);
	return result;
}


value_t calc_doAST(calcState_t *state, AST* tree) {
	return NIL;
}
