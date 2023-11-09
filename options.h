#pragma once
#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

typedef struct options_t {
    bool debugTokens;   /* Override state's token callback    */
    bool debugSyntax;   /* Override state's AST callback      */
    bool suppressCast;  /* Suppress default cast behaviour    */
    bool showVariables; /* Display variables list each time   */
    bool noEmbedDelete; /* Don't use "delete x" syntax        */
    bool preciseFloat;  /* Print float with better precision  */
    
    bool extraValues; /* Insert extra constants from <limits.h> and 
                         <float.h> */
} options_t;

#endif /* OPTIONS_H */
