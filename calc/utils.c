#include <calc/utils.h>
#include <stdio.h>

void calcU_printTokens(TokenList* list, FILE* stream) {
	for (int i = 0; i < list->size; ++i) {
		Token t = list->tokens[i];
		fprintf(
			stream,
			"%s(\"",
			lang_tokenName(t.type));
		fwrite(
			t.slice.start,
			1,
			t.slice.length, 
			stream);
		fprintf(
			stream,
			"\")\n");
	}
}


void calcU_printValue(const value_t v) {
    switch(v.type) {
    case VALUE_NIL:
        printf("nil\n");
        break;
    case VALUE_INT:
        printf("%ld\n", v.integer);
        break;
    case VALUE_REAL:
        printf("%lf\n", v.real);
        break;
    };
}


static void _printNode(AST* node, FILE* stream) {
	if (node != NULL) {
		Slice slice = node->token.slice;
		fprintf(
			stream,
			"%s(",
			lang_nodeName(node->type));
		if (node->type != ERROR) {
			fwrite(
				slice.start,
				1,
				slice.length,
				stream);
		}
		fprintf(
			stream,
			")\n");
	}
}


static void _printTree(AST* root, FILE* stream, size_t level) {
	if (root != NULL) {
		for (size_t i = 0; i < level; ++i)
			fputc('\t', stream);
		_printNode(root, stream);
		_printTree(root->lop, stream, level + 1);
		_printTree(root->rop, stream, level + 1);
	}
} 


void calcU_printTree(AST* tree, FILE* stream) {
	_printTree(tree, stream, 0);
}


void calcU_printIdent(ident_t id) {
	printf("%s = ", id.name);
	if (id.value.type == VALUE_NIL)
		printf("nil\n");
	else if (id.value.type == VALUE_REAL)
		printf("%f\n", id.value.real);
	else
		printf("%ld\n", id.value.integer);	
}


void calcU_printIdentifiers(identList_t *ids) {
	if (ids == NULL) return;
	calcU_printIdentifiers(ids->left);
	calcU_printIdent(ids->identifier);
	calcU_printIdentifiers(ids->right);
}