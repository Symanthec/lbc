add_library(calc STATIC lexer.c calc.c error.c ident.c ops.c state.c token.c tokenvector.c value.c parser.c)
target_include_directories(calc PUBLIC ../include)
