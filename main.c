#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/errno.h>

#include <limits.h>
#include <float.h>

#include <calc/calc.h>
#include <calc/utils.h>
#include "options.h"


#define EXIT_COMMAND    "exit"  
#define PROMPT_STRING   ">>> "
#define EMBED_DEL       "del"
#define MAXLINE         256


const char *usage_msg =
"OVERVIEW: \"bc\" basic calculator clone\n"
"\n"
"USAGE: bc [options]\n"
"\n"
"OPTIONS:\n"
"    --tokens\n"
"            Print token list when compiling line\n"
"    --syntax\n"
"            Print abstract syntax tree (AST) when compiling line\n"
"    --nocast\n"
"            Suppress default type casting to old value's type\n"
"    --novars\n"
"            Do not display variables after prompt\n"
"    --nodelete\n"
"            Suppress \"delete x\" syntax in favor of default \"x = nil\"\n"
"    --precise\n"
"            Print floating point values with 10-digit precision\n"
"    --extras\n"
"            Add extra values __DBL_MIN, __DBL_MAX, __LONG_MAX, __LONG_MIN, __DBL_EPSILON";


static void usage() {
    puts(usage_msg);
}


options_t options = {
    false, /* .debugTokens      */
    false, /* .debugSyntax      */
    false, /* .suppressCast     */
    true,  /* .showVariables    */
    false, /* .noEmbedDelete    */
    false, /* .preciseFloats    */
    false, /* .extraValues      */
};


static value_t noCastBehaviour(const value_t old, const value_t replace) {
    return replace;
}


static bool prompt(const char* prefix, char* dest, size_t maxLength) {
    fputs(prefix, stdout);
    return fgets(dest, maxLength, stdin) != NULL;
}


int main(int argc, char** argv)
{
    if (argc > 1) {
        size_t index;
        for (index = 1; index < argc; ++index) {
            const char *argument = argv[index];
            if (strcmp(argument, "--tokens") == 0)
                options.debugTokens = true; 
            else if (strcmp(argument, "--syntax") == 0)
                options.debugSyntax = true;
            else if (strcmp(argument, "--nocast") == 0)
                options.suppressCast = true;
            else if (strcmp(argument, "--novars") == 0)
                options.showVariables = false;
            else if (strcmp(argument, "--nodelete") == 0)
                options.noEmbedDelete = true;
            else if (strcmp(argument, "--precise") == 0)
                options.preciseFloat = true;
            else if (strcmp(argument, "--extras") == 0)
                options.extraValues = true;
            else {
                /* --help included */
                usage();
                return -1;
            }
        }
    }

    if (options.preciseFloat) calcU_floatPrintPrecision(16);

    calcState_t *state = calc_newState();
    if (options.debugSyntax)  calcU_onDebugTree(state, calcU_printTree);
    if (options.debugTokens)  calcU_onDebugTokens(state, calcU_printTokens);
    if (options.suppressCast) calcU_castBehaviour(state, noCastBehaviour);
    if (options.extraValues) {
        /* LIMITS.H */
        calc_setValueI(state, "__LONG_MAX", LONG_MAX);
        calc_setValueI(state, "__LONG_MIN", LONG_MIN);
        /* FLOAT.H */
        calc_setValueD(state, "__DBL_MAX", DBL_MAX);
        calc_setValueD(state, "__DBL_MIN", DBL_MIN);
        calc_setValueD(state, "__DBL_EPSILON", DBL_EPSILON);
        calc_commit(state);
    }

    char line[MAXLINE] = {0}, del_pending[MAXLINE] = {0};
    error_t err;
    while (prompt(PROMPT_STRING, line, MAXLINE))
    {
        if (strlen(line) <= 1) continue; /* '\0' or '\n' */

        // Exit sequence
        if (strstr(line, EXIT_COMMAND) != NULL)
            break;

        // embed "delete x" functionality over "x = nil"    
        if (!options.noEmbedDelete && 
            sscanf(line, EMBED_DEL " %s", del_pending) > 0) 
        {
            calc_setValue(state, del_pending, NIL);
            calc_commit(state);
        } 
        else 
        {
            // single state execution
            value_t result = calc_doLine(state, line);

            err = calc_getError(state);
            if (err.code == OK) {
                calc_commit(state);
                calcU_printValue(result);
                putchar('\n');
            } else {
                printf("Error occurred...\n");
                calcU_printError(state, line);
                calc_clearError(state);
            }
        }

        // Enumerate identifiers, because why not?
        if (options.showVariables) {
            unsigned count = calcU_printIdentifiers(state->identifiers);
            printf(" Total: %u\n", count);
        }
    }

    calc_freeState(state);
    return 0;
}
