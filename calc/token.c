#include <calc/lang/token.h>

// tilde preceeds service tokens
static const char* NAMES[] = {
	"<NIL>",
	"<IDENTIFIER>",
	"<REAL>",
	"<INTEGER>",
	// Math operations
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"MOD",
	"EQU",
	// Parentheses
	"LPAREN",
	"RPAREN",
	// C-string-like terminating token
	"END",
	// What starts with tilde must not occur in valid lines list
	// Inner (service) or erroneous "tokens"
	"~WHITESPACE",
	"~MULTICHAR",
	"~INVALID"
};

const char* lang_tokenName(TokenType type) {
	return NAMES[type];
}
