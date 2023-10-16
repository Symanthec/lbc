add_library(calc STATIC value.c ops.c ident.c state.c error.c calc.c)
target_include_directories(calc PUBLIC ../include)
