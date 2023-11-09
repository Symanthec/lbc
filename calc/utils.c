#include <calc/utils.h>
#include <stdio.h>
#include <string.h>

void calcU_printTokens(const TokenList* list) {
	for (int i = 0; i < list->size; ++i) {
		Token t = list->tokens[i];
		printf("%s(\"", lang_tokenName(t.type));
		fwrite(
			t.slice.start,
			1,
			t.slice.length, 
			stdout);
		printf("\")\n");
	}
}




static unsigned char __precision = 6;
char fltPrintFormat[8] = "%.6lf";
const char fltPrintFormatFormat[] = "%%.%ulf";

unsigned char calcU_floatPrintPrecision(unsigned char precision) {
	unsigned char old = __precision;
	__precision = precision;
	snprintf(fltPrintFormat, 8, fltPrintFormatFormat, precision);
	return old;
}


void calcU_printValue(const value_t v) {
    switch(v.type) {
    case VALUE_NIL:
        printf("nil");
        break;
    case VALUE_INT:
        printf("%ld", v.integer);
        break;
    case VALUE_REAL:
        printf(fltPrintFormat, v.real);
        break;
    };
}


static void _printNode(const AST* node) {
	if (node != NULL) {
		Slice slice = node->token.slice;
		printf("%s(", lang_nodeName(node->type));
		if (node->type != ERROR) {
			fwrite(
				slice.start,
				1,
				slice.length,
				stdout);
		}
		printf(")\n");
	}
}


#define INDENT_CHR '\t'


static void _printTree(const AST* root, const size_t level) {
	if (root != NULL) {
		for (size_t i = 0; i < level; ++i)
			putchar(INDENT_CHR);
		_printNode(root);
		_printTree(root->lop, level + 1);
		_printTree(root->rop, level + 1);
	}
} 


void calcU_printTree(const AST* tree) {
	_printTree(tree, 0);
}


void calcU_printIdent(const ident_t id) {
	printf("\"%s\" = ", id.name);
	calcU_printValue(id.value);
}


static inline bool shouldPrint(const char * const name) {
	return strncmp(name, "__", 2) != 0;
}


static void __calcU_printIdentifiers(const identList_t *ids, unsigned *counter) {
	if (ids == NULL) return;
	__calcU_printIdentifiers(ids->left, counter);
	
	// is stub node?
	if (*(ids->identifier.name) != '\0' && shouldPrint(ids->identifier.name)) {
		if ((*counter)++ > 0) printf(", ");
		calcU_printIdent(ids->identifier);
	}

	__calcU_printIdentifiers(ids->right, counter);
}


unsigned calcU_printIdentifiers(const identList_t *ids) {
	putchar('[');
	unsigned counter = 0;
	__calcU_printIdentifiers(ids, &counter);
	putchar(']');
	return counter;
}


void calcU_printError(const calcState_t * const state, const char * const line) {
	const error_t err = calc_getError(state);
	
	fprintf(stderr,
		"[Calc error]: %s at %zu\n",
		calc_getErrorMsg(err.code),
		err.where.start - line + 1);

	size_t len = strlen(line);
	fprintf(stderr, "%s", line);
	for (size_t i = 0; i < err.where.start - line && i < len; ++i)
		fputc(' ', stderr);
	for (size_t i = 0; i < err.where.length && i < len; ++i)
		fputc('^', stderr);
	fputc('\n', stderr);
}
