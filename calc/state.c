#include <calc/calc.h>
#include <stdlib.h>

static void fnDbgTree(const AST * const tree) {return;}
static void fnDbgTokens(const TokenList * const list) {return;}
static value_t fnCbCastToOld(const value_t old, const value_t right) {
    value_t casted;

    /* Cast right value to old value's type */
    if (old.type == VALUE_INT && right.type == VALUE_REAL) {
        casted.type = VALUE_INT;
        casted.integer = right.real;
    } else if (old.type == VALUE_REAL && right.type == VALUE_INT) {
        casted.type = VALUE_REAL;
        casted.real = right.integer;
    } else {
        casted = right;
    }

    return casted;
}


calcState_t * calc_newState(void) {
    calcState_t *state = (calcState_t*) malloc(sizeof(calcState_t));
    state->identifiers = calcP_newIdentList();
    state->staging = calcP_newIdentList();
    state->error = NO_ERROR;
    // default callbacks
    calcU_onDebugTree(state, fnDbgTree);
    calcU_onDebugTokens(state, fnDbgTokens);
    calcU_castBehaviour(state, fnCbCastToOld);
    return state;
}


void calc_freeState(calcState_t *state) {
    if (state == NULL) return;
    calcP_freeIdentList(state->identifiers);
    calcP_freeIdentList(state->staging);
    free(state);
}


value_t calc_getValue(const calcState_t *state, const char* name) {
    if (state == NULL) return NIL;
    return calc_valueOf(state->identifiers, name);
}


value_t calc_setValue(calcState_t *state, const char* name, const value_t v) {
    if (state == NULL || name == NULL) return NIL;
    return calcP_setRawIdent(state->staging, name, v);
}


value_t calc_setValueI(calcState_t * const    state,
                       const char * const     name,
                       const long             integer)
{
    value_t val = { VALUE_INT, .integer = integer };
    return calc_setValue(state, name, val);
}


value_t calc_setValueD(calcState_t * const    state,
                       const char * const     name,
                       const double           real)
{
    value_t val = { VALUE_REAL, .real = real };
    return calc_setValue(state, name, val);
}


void __calc_commit(identList_t *from, identList_t *to) {
    if (from == NULL || to == NULL) return;

    if (from->left != NULL)
        __calc_commit(from->left, to);
    if (from->right != NULL)
        __calc_commit(from->right, to);
    calc_setIdentifier(to, from->identifier.name, from->identifier.value);
}


void calc_commit(calcState_t* state) {
    __calc_commit(state->staging, state->identifiers);
    calcP_freeIdentList(state->staging);
    state->staging = calcP_newIdentList();
}


#define REPLACE_CB(type, where, name, new)  \
    if (new != NULL && where != NULL) {     \
        type old = where->name;             \
        where->name = new;                  \
        return old;                         \
    }                                       \
    return NULL;


calc_fnDbgTree calcU_onDebugTree(calcState_t * const    state,
                                const calc_fnDbgTree    cb) {
    REPLACE_CB(calc_fnDbgTree, state, debugTree, cb);
}


calc_fnDbgTokens calcU_onDebugTokens(calcState_t * const    state,
                                    const calc_fnDbgTokens  cb) {
    REPLACE_CB(calc_fnDbgTokens, state, debugTokens, cb);
}


calc_fnCbCast calcU_castBehaviour(calcState_t * const   state,
                                 const calc_fnCbCast    cb) {
    REPLACE_CB(calc_fnCbCast, state, valueCaster, cb);
}

#undef REPLACE_CB
