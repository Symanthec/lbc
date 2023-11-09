#pragma once
#ifndef CALC_LANG_PARSER_H
#define CALC_LANG_PARSER_H

#include <calc/lang/tokenvector.h>
#include <calc/lang/ast.h>


/* 
** Parser function. Consumes list of tokens and returns abstract syntax tree
** for this line. In case of error returns node with .type == ERROR and 
** .error set.
*/
extern AST* lang_parseTokens(const TokenList list);


#endif/* CALC_LANG_PARSER_H */
