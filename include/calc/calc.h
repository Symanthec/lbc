#pragma once
#ifndef CALC_H
#define CALC_H

/* 
**  Not quite "bc" but a small "calculating" library
**  - Each instance has its own state (variables container)
**  - Functions are written in a way which (in theory) should allow for
**    parallel independent execution of multiple states
**  - Some debug capabilities such as lexer, parser, and some other callbacks
**  - DELETE with "del x", turns into "x = nil" as in Lua
**
**  Known flaws: 
**  - doLine's string must not be changed until AST is freed (no longer used)
**
**  Naming scheme: 
**  - All functions if this "library" have prefix calc_
**  - Helper functions for printing specific ADTs and debug start with calcU_
**  - "lang/" part of this library starts with lang_
**  - "Private functions" start with calcP_
*/  


#include <calc/value.h>
#include <calc/state.h>


/* 
** Compiles provided line and returns its AST. State provides callbacks and
** stores errors if any occur.
*/
AST* calc_compileLine(calcState_t * const state, const char * const line);


/*
** Compiles and executes line for given state. Returns computed value
*/
value_t calc_doLine(calcState_t * const state, const char * const line);


#endif/* CALC_H */
