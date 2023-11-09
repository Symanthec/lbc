#include <calc/ident.h>
#include <calc/value.h>
#include <calc/utils.h>
#include <stdlib.h>
#include <string.h>
// #include <stdio.h>

#define MAX(x, y) ((x) >= (y) ? (x): (y))


static inline void __stubNode(identList_t *node) {
	node->left = NULL;
	node->right = NULL;
	node->identifier.value = NIL;
	memset(node->identifier.name, '\0', CALC_IDENTIFIER_LENGTH);
}


identList_t* calcP_newIdentList(void) {
	identList_t* list = malloc(sizeof(identList_t));
	__stubNode(list);
	// printf("mk %p\n", list);
	return list;
}


void calcP_freeIdentList(identList_t* list) {
	if (list == NULL) return;

	if (list->left != NULL)
		calcP_freeIdentList(list->left);
	if (list->right != NULL)
		calcP_freeIdentList(list->right);
	// printf("rm %p\n", list);
	free(list);
}


static inline const identList_t* findIdentifier(const identList_t* list, const char* name) {
	if (list == NULL) return NULL;

	int cmp;
	const identList_t* prev;
	while (list != NULL) {
		cmp = strncmp(name, list->identifier.name, CALC_IDENTIFIER_LENGTH);
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


value_t calc_valueOf(const identList_t* list, const char* name) {
	const identList_t* ident = findIdentifier(list, name);
	value_t result = ident != NULL ? ident->identifier.value: NIL;
	return result;
}


value_t calcP_setRawIdent(identList_t * node, const char* name, const value_t val) {
		if (strlen(node->identifier.name) == 0) {
		// empty (primary) node
		strncpy(node->identifier.name, name, CALC_IDENTIFIER_LENGTH);
		node->identifier.value = val;
		return NIL;
	}

	identList_t* prev;
	int cmp;
	while (node != NULL) {
		cmp = strncmp(name, node->identifier.name, CALC_IDENTIFIER_LENGTH);
		if (cmp == 0) {
			// Identifier exists
			value_t old = node->identifier.value;
			node->identifier.value = val;
			return old;
		} else {
			prev = node;
			if (cmp < 0)
				node = node->left;
			else // cmp > 0
				node = node->right;
		}
	}

	// Reached the end and did not find identifier
	identList_t *id = calcP_newIdentList();
	strncpy(id->identifier.name, name, CALC_IDENTIFIER_LENGTH);
	id->identifier.value = val;

	if (cmp < 0)
		prev->left = id;
	else
		prev->right = id;
	return NIL; // since we had to create variable	
}


/** Assigns new value to identifier with given name and creates it if necessary
 ** It also allows to remove identifier by assigning NIL to it */
value_t calc_setIdentifier(identList_t* node, const char* name, const value_t val) {
	if (node == NULL || (*name == '\0')) return NIL;
	if (calc_isValueNil(&val)) 
		return calc_popIdentifier(node, name);
	else
		return calcP_setRawIdent(node, name, val);
}


static inline void swapNodes(identList_t *one, identList_t*two) {
	ident_t swp = one->identifier;
	one->identifier = two->identifier;
	two->identifier = swp;

	identList_t *pswp = one->left;
	one->left = two->left;
	two->left = pswp;

	pswp = one->right;
	one->right = two->right;
	two->right = pswp;
}


// Since this list is pointed to from outside, we cannot delete root without
// leaks and bugs, so any node to be deleted is swapped first
static inline identList_t* deleteIdentifier(identList_t* root, const char* name, identList_t **tofree) {
	if (root == NULL) return NULL;
	
	int cmp = strncmp(name, root->identifier.name, CALC_IDENTIFIER_LENGTH);
	if (cmp > 0) {
		root->right = deleteIdentifier(root->right, name, tofree);
		return root;
	} else if (cmp < 0) {
		root->left = deleteIdentifier(root->left, name, tofree);
		return root;
	}

	// We reach here when root is the node to be deleted.
	identList_t *temp = NULL;
	if (root->left == NULL)
		temp = root->right;
	else if (root->right == NULL) 
		temp = root->left;
	
	else {
		// Both children exist
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
		succ->left = succ->right = NULL;
		*tofree = succ;
		return root;
	}

	// we have temp != NULL, one branch NULL
	if (temp != NULL) {
		swapNodes(temp, root);
		temp->left = temp->right = NULL;
		*tofree = temp;
		return root;
	} else {
		*tofree = root;
		return NULL;
	}
}

// Remove identifier by the name from the list and return its value
value_t calc_popIdentifier(identList_t* list, const char* name) {
	if (list == NULL) return NIL;
	
	identList_t *node = NULL;
	deleteIdentifier(list, name, &node);
	if (node != NULL) {
		value_t result = node->identifier.value;
		if (node == list)
			// cannot delete, referenced from outside
			__stubNode(node);
		else
			calcP_freeIdentList(node);
		return result;
	}

	// node == NULL
	return NIL;
}
