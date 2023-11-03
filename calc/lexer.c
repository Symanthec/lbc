#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <calc/lang/lexer.h>


static Token pickSymbol(const char* c) {
	Token t = { 
		.type = INVAL,
		.slice = { .start = c, .length = 1 }
	};

	char sym = *c;
	switch (sym) {
	case '+':
		t.type = ADD;
		break;
	case '-':
		t.type = SUB;
		break;
	case '*':
		t.type = MUL;
		break;
	case '/':
		t.type = DIV;
		break;
	case '%':
		t.type = MOD;
		break;
	case '=':
		t.type = EQU;
		break;
	case '(':
		t.type = LPAREN;
		break;
	case ')':
		t.type = RPAREN;
		break;
	case ' ':
	case '\t':
		t.type = WHITE;
		break;
	case '\0':
	case '\n':
		t.type = END;
		break;
	default:
		if (isalnum(sym) || sym == '_' || sym == '.')
			// [_0-9A-Za-z]
			t.type = MULTI;
		else
			t.type = INVAL;
		break;
	};
	return t;
}


static Token pickNumber(const char* start) {
	const char *in = start;

	// skip sign
	if ((*in == '-') || (*in == '+'))
		in += 1;

	bool real = false, digits = false;
	char sym;
	do {
		sym = *in;

		// (dot and not real) or digit
		if (sym == '.')
		{
			if (real)
				// second dot => end of number
				break;
			else
				// dot => indicator of real number
				real = true;
		} 
		else if (isdigit(sym)) 
		{
			digits = true;
		} else {
			// not dot but not digit
			break;
		}

		// digit or first dot => fine
		in += 1;
	} while (*in != '\n');

	if (digits) {
		Slice s = { start, in - start };
		Token t = {
			real ? T_REAL : T_INTEGER,
			s
		};
		return t;
	} else {
		Slice s = { start , 1 };
		Token bad = {
			INVAL,
			s
		};
		return bad;
	}
}


static Token pickIdentifier(const char* start) {
	const char* in = start;
	// assume that this fun is invoked
	// when first character is valid
	while (*in == '_' || isalnum(*in))
		in += 1;


	size_t len = in - start;
	Slice s = { start, len };
	Token t = { T_IDENT, s };

	if (len == 3 && strncmp(start, "nil", 3) == 0) {
		// identifier is actually a "nil" keyword
		t.type = T_NIL;
	}

	return t;
}


TokenVector* lang_tokenizeLine(const char* line) {
	// return demoTokens();

	TokenVector *vec = lang_newTokens();

	const char* input = line;
	bool afterMulti = false, afterOp = true;
	Token t;
	do {
		// try lex'ing one symbol
		t = pickSymbol(input);

		if ((t.type == SUB || t.type == ADD) && afterOp) {
			// minus after operator like 10 * -3
			t = pickNumber(input);
			afterOp = false;
		}

		if (t.type == MULTI)
		{
			if (!afterMulti) {
				if (isdigit(*input) || (*input == '.') || (*input == '-')) {
					t = pickNumber(input);
				} else { // if (*input == '_' || isletter(*input))
					t = pickIdentifier(input);
				}
				afterMulti = true;
			} else {
				t.type = INVAL;
				afterMulti = false;
			}
		}  else {
			afterMulti = false;
		}


		if ((t.type != WHITE) && (t.type != END)) {
			vec = lang_pushToken(vec, t);
			afterOp = ADD <= t.type && t.type <= EQU;
		}
		input += t.slice.length;
	} while (t.type != END);
	return vec;
}


TokenVector *demoTokens(void) {
	Token _0 = {
		.type = T_IDENT,
		.slice = { .start = "_13var", .length = 6 }};
	Token _1 = {
		.type = EQU,
		.slice = { .start = "=", .length = 1 }};
	Token _2 = {
		.type = T_REAL,
		.slice = { .start = "3.14159" , .length = 7 }};

	TokenVector *vec = lang_pushToken(NULL, _0);
	lang_pushToken(vec, _1);
	lang_pushToken(vec, _2);
	return vec;
}