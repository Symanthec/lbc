#pragma once
#ifndef CALC_IDENT_H
#define CALC_IDENT_H

#include <calc/value.h>

#define IDENTIFIER_LENGTH 256


typedef struct {
	value_t value;
	char name[IDENTIFIER_LENGTH];
} ident_t;


// Binary search tree
typedef struct identList_t {
	ident_t identifier;
	struct identList_t *left;
    struct identList_t *right;
} identList_t;


extern identList_t * calcP_newIdentList(void);
extern void calcP_freeIdentList(identList_t *);

extern value_t calc_valueOf(identList_t *, const char *);

// can assign nil to variable
extern value_t calcP_setRawIdent(identList_t *, const char*, value_t);

// x = nil === delete x
extern value_t calc_setIdentifier(identList_t *, const char *, value_t);
extern value_t calc_popIdentifier(identList_t *, const char*);

#endif//CALC_IDENT_H
