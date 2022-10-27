
set(tests test_lexer)

set(tests_src
	../src/compiler.c
	../src/lexical_analysis.c
	../src/symtab.c)
set(tests_header
	../src/compiler.h
	../src/symtab.h
	../src/lexical_analysis.h)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test_lexer_1.sh
	"#!/bin/bash\n"
	"${CMAKE_CURRENT_BINARY_DIR}/test_lexer < ${CMAKE_SOURCE_DIR}/tests/ex1.php")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test_lexer_2.sh
	"#!/bin/bash\n"
	"${CMAKE_CURRENT_BINARY_DIR}/test_lexer < ${CMAKE_SOURCE_DIR}/tests/ex2.php")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/test_lexer_3.sh
	"#!/bin/bash\n"
	"${CMAKE_CURRENT_BINARY_DIR}/test_lexer < ${CMAKE_SOURCE_DIR}/tests/ex3.php")

add_executable(test_lexer test_lexer.c ${tests_src} ${tests_header})
add_test(test_lex_ex_1 ${CMAKE_CURRENT_BINARY_DIR}/test_lexer_1.sh)
add_test(test_lex_ex_2 ${CMAKE_CURRENT_BINARY_DIR}/test_lexer_2.sh)
add_test(test_lex_ex_3 ${CMAKE_CURRENT_BINARY_DIR}/test_lexer_3.sh)
