#include <string.h>
#include <stdlib.h>

#include <calc/ident.h>
#include <calc/value.h>


#define MAX(x, y) ((x) >= (y) ? (x): (y))


identList_t* calcP_newIdentList(void) {
	identList_t* list = malloc(sizeof(identList_t));
	list->left = NULL;
	list->right = NULL;
	list->identifier.value = NIL;
	list->height = 1;
	memset(list->identifier.name, '\0', IDENTIFIER_LENGTH);
	return list;
}


void calcP_freeIdentList(identList_t* list) {
	if (list == NULL) return;

	if (list->left != NULL)
		calcP_freeIdentList(list->left);
	if (list->right != NULL)
		calcP_freeIdentList(list->right);
	free(list);
}


static inline identList_t* findIdentifier(identList_t* list, const char* name) {
	if (list == NULL) return NULL;

	int cmp;
	identList_t* prev;
	while (list != NULL) {
		cmp = strncmp(name, list->identifier.name, IDENTIFIER_LENGTH);
		if (cmp == 0) {
			// Identifier exists
			return list;
		} else {
			prev = list;
			if (cmp < 0)
				list = list->left;
			else
				list = list->right;
		}
	};

	return NULL;
}


value_t calc_valueOf(identList_t* list, const char* name) {
	identList_t* ident = findIdentifier(list, name);
	return ident != NULL ? ident->identifier.value: NIL;
}


/** Assigns new value to identifier with given name and creates it if necessary
 ** It also allows to remove identifier by assigning NIL to it */
void calc_setIdentifier(identList_t* node, const char* name, value_t val) {
	if (node == NULL) return;
	
	if (calc_isValueNil(&val)) {
		calc_popIdentifier(node, name);
		return;
	}

	identList_t* prev;
	int cmp;
	while (node != NULL) {
		cmp = strncmp(name, node->identifier.name, IDENTIFIER_LENGTH);
		if (cmp == 0) {
			// Identifier exists
			node->identifier.value = val;
			return;
		} else {
			prev = node;
			if (cmp < 0)
				node = node->left;
			else
				node = node->right;
		}
	}

	// Reached the end and did not find identifier
	identList_t *id = calcP_newIdentList();
	strncpy(id->identifier.name, name, IDENTIFIER_LENGTH);
	id->identifier.value = val;
	id->height = prev->height + 1;

	if (cmp < 0)
		prev->left = id;
	else
		prev->right = id;
}


/** Return maximum height of the tree */
unsigned calc_listHeight(identList_t* list) {
	if (list == NULL) return 0;
	unsigned l = calc_listHeight(list->left), r = calc_listHeight(list->right);
	return MAX(l, r) + 1;
}


static inline identList_t* deleteIdentifier(identList_t* root, const char* name) {
	if (root == NULL) return NULL;

	int cmp = strncmp(name, root->identifier.name, IDENTIFIER_LENGTH);
	if (cmp > 0) {
		root->right = deleteIdentifier(root->right, name);
		return root;
	} else if (cmp < 0) {
		root->left = deleteIdentifier(root->left, name);
		return root;
	} 


	// We reach here when root is the node to be deleted.
	if (root->left == NULL) {
		identList_t *temp = root->right;
		free(root);
		return temp;
	} else if (root->right == NULL) {
		identList_t *temp = root->left;
		free(root);
		return temp;
	}

	// Both children exist
	else {
		identList_t *succParent = root;
		identList_t *succ = root->right;

		while (succ->left != NULL) {
			succParent = succ;
			succ = succ->left;
		}

		if (succParent != root)
			succParent->left = succ->right;
		else
			succParent->right = succ->right;

		root->identifier = succ->identifier;
		free(succ);
		return root;
	}
}

/** Remove identifier by the name from the list and return its value */
value_t calc_popIdentifier(identList_t* list, const char* name) {
	if (list == NULL) return NIL;
	
	identList_t *node = deleteIdentifier(list, name);
	if (node != NULL && calc_isValueNil(&(node->identifier.value)) )
		return node->identifier.value;	
	else
		return NIL;
}