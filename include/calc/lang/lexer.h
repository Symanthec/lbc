#pragma once
#ifndef CALC_LANG_LEXER_H
#define CALC_LANG_LEXER_H

#include <calc/lang/tokenvector.h>

/* 
** Converts a line terminated with '\0' or '\n' into vector of tokens
*/
extern TokenList *langP_tokenizeLine(const char* line);

#endif /* CALC_LANG_LEXER_H */
