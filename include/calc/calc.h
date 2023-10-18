#pragma once
#ifndef CALC_H
#define CALC_H

#include <calc/value.h>
#include <calc/state.h>
#include <calc/error.h>
#include <calc/ops.h>
#include <calc/ident.h>
#include <calc/parser.h>


value_t calc_doLine(calcState_t *, const char*);


value_t calc_doAST(calcState_t *, AST *);


#endif//CALC_H
