#include <calc/lang/lexer.h>
#include <calc/parser.h>
#include <stdio.h>


static const char* types[] = {
	"END",
	"(",
	")",
	"+",
	"-",
	"%",
	"/",
	"*",
	"IDENT",
	"REAL",
	"INTEGER",
	"NIL"
};


ParserResult calcP_parseTokens(TokenList* list) {
	Token *iter = list->tokens;
	for (int i = 0; i < list->size; ++i) {
		Token t = iter[i];
		printf("%s(\"", lang_tokenName(t.type));
		fwrite(t.slice.start,
			sizeof(char),
			t.slice.length, 
			stdout);
		printf("\")\n");
	}	

	ParserResult r = { SUCCESS };
	return r;
}
