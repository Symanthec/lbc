#pragma once
#ifndef CALC_LANG_PARSER_H
#define CALC_LANG_PARSER_H

#include <calc/lang/tokenvector.h>
#include <calc/lang/ast.h>


extern AST* lang_parseTokens(TokenList list);


#endif//CALC_LANG_PARSER_H
