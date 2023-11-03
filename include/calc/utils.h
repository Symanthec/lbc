#include <calc/lang/tokenvector.h>
#include <calc/lang/ast.h>
#include <stdio.h>


// Print all tokens from token list to file
extern void calcU_printTokens(TokenList* list);


// Print abstract syntax tree to file
extern void calcU_printTree(AST* tree);


extern void calcU_printIdent(ident_t id);


extern void calcU_printValue(const value_t v);


// Print list of variables
extern void calcU_printIdentifiers(identList_t *ids);
