#pragma once
#ifndef CALC_H
#define CALC_H

/* 
**  Not quite "bc" but a small "calculating" library
**  - Each instance has its own state (var container)
**  - Functions are written in a way which (in theory) should allow for
**    parallel independent execution of multiple states
**  - Some debug capabilities such as lexer, parser, and some other callbacks
**  - DELETE NOT with a "delete x", but rather "x = nil" as in Lua
**
**
**  Known flaws: 
**  - doLine's string must not be changed until AST is freed (no longer used)
**
**  Naming scheme: 
** 
**  - All functions if this "library" have prefix calc_
**  - Auxiliary functions for printing specific ADTs start with calcU_
**  - "lang/" part of this library starts with lang_
**  - "Private functions" start with calcP_
*/  


#include <calc/value.h>
#include <calc/state.h>

value_t calc_doLine(calcState_t *, const char*);


#endif//CALC_H
