#pragma once
#ifndef CALC_LANG_LEXER_H
#define CALC_LANG_LEXER_H

#include <calc/lang/tokenvector.h>

// Convert \0-terminated line into vector of tokens
TokenList *lang_tokenizeLine(const char* line);


// Demo list of tokens
TokenList *lang_demoTokens(void);

#endif