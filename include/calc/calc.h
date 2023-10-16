#pragma once
#ifndef CALC_H
#define CALC_H

#include "value.h"
#include "state.h"
#include "error.h"
#include "ops.h"
#include "ident.h"

value_t calc_doLine(calcState_t *, const char*);

#endif//CALC_H
