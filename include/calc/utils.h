#pragma once
#ifndef CALC_UTILS_H
#define CALC_UTILS_H

#include <calc/lang/tokenvector.h>
#include <calc/lang/ast.h>


/*
** Prints all tokens from given list.
** For details see "calc/lang/token.h" and "calc/lang/tokenvector.h".
**
** May be used individually, but to avoid changing source code
** use it with debug callback calcU_onDebugTokens(state, calcU_printTokens);
*/
extern void calcU_printTokens(const TokenList * const list);


/*
** Prints abstract syntax tree (AST) of some expression.
** For details see "calc/lang/ast.h".
**
** May be used individually, but to avoid changing source code
** use it with debug callback calcU_onDebugTree(state, calcU_printTree)
*/
extern void calcU_printTree(const AST * const tree);


/*
** Prints identifier in a form of "<name>" = <value>
** See "calc/ident.h"
*/
extern void calcU_printIdent(const ident_t id);


/*
** Prints calculator-type value. 
** Real numbers are printed with precision of CALC_PRNT_VAL_PREC (default = 6)
** See "calc/value.h"
*/
extern void calcU_printValue(const value_t value);


/*
** Prints identifiers from a list separated with commas between square brackets
** E.g. ["a" = 10, "b" = 3.1415, "c" = 24]
** See "calc/ident.h" and "calc/value.h"
*/
extern unsigned calcU_printIdentifiers(const identList_t * const ids);


/*
** Prints error for given state and line.
** NOTE: Line should be that caused the error.
*/
void calcU_printError(const calcState_t *state, const char * const line);

#endif /* CALC_UTILS_H */
