#include <calc/lang/tokenvector.h>
#include <calc/lang/ast.h>


// Print all tokens from token list to file
extern void calcU_printTokens(const TokenList* list);


// Print abstract syntax tree to file
extern void calcU_printTree(const AST* tree);


extern void calcU_printIdent(const ident_t id);


extern void calcU_printValue(const value_t v);


// Print list of variables
extern unsigned calcU_printIdentifiers(const identList_t *ids);
