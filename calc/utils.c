#include <calc/utils.h>
#include <stdio.h>

void calcU_printTokens(TokenList* list) {
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


void calcU_printValue(const value_t v) {
    switch(v.type) {
    case VALUE_NIL:
        printf("nil");
        break;
    case VALUE_INT:
        printf("%ld", v.integer);
        break;
    case VALUE_REAL:
        printf("%lf", v.real);
        break;
    };
}


static void _printNode(AST* node) {
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


static void _printTree(AST* root, size_t level) {
	if (root != NULL) {
		for (size_t i = 0; i < level; ++i)
			putchar('\t');
		_printNode(root);
		_printTree(root->lop, level + 1);
		_printTree(root->rop, level + 1);
	}
} 


void calcU_printTree(AST* tree) {
	_printTree(tree, 0);
}


void calcU_printIdent(ident_t id) {
	printf("%s = ", id.name);
	calcU_printValue(id.value);
}


void calcU_printIdentifiers(identList_t *ids) {
	if (ids == NULL) return;
	calcU_printIdentifiers(ids->left);
	calcU_printIdent(ids->identifier);
	printf(", ");
	calcU_printIdentifiers(ids->right);
}