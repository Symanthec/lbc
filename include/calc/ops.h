#pragma once
#ifndef CALC_OPS_H
#define CALC_OPS_H

#include "value.h"


/* Addition operation between values */
extern value_t calc_add(const value_t, const value_t);


/* Subtraction operation between values */
extern value_t calc_sub(const value_t, const value_t);


/* Multiplication operation between values */
extern value_t calc_mul(const value_t, const value_t);


/* Division operation between values */
extern value_t calc_div(const value_t, const value_t);


/* Modulo division operation between values */
extern value_t calc_mod(const value_t, const value_t);


#endif /* CALC_OPS_H */
