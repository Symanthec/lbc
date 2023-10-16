#pragma once
#ifndef CALC_IDENT_H
#define CALC_IDENT_H

#include "value.h"

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
	unsigned height;
} identList_t;


extern identList_t * calcP_newIdentList(void);
extern void calcP_freeIdentList(identList_t *);

extern value_t calc_valueOf(identList_t *, const char *);
extern unsigned calc_listHeight(identList_t *);

extern void calc_setIdentifier(identList_t *, const char *, value_t);
extern value_t calc_popIdentifier(identList_t *, const char*);

#endif//CALC_IDENT_H
