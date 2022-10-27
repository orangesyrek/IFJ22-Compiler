
set(tests test_lexer)

set(tests_src
	../src/compiler.c
	../src/lexical_analysis.c
	../src/symtab.c)
set(tests_header
	../src/compiler.h
	../src/symtab.h
	../src/lexical_analysis.h)

add_executable(test_lexer test_lexer.c ${tests_src} ${tests_header})
add_test(test_lex_ex_1 ${CMAKE_SOURCE_DIR}/tests/test_lexer_1.sh)
add_test(test_lex_ex_2 ${CMAKE_SOURCE_DIR}/tests/test_lexer_2.sh)
add_test(test_lex_ex_3 ${CMAKE_SOURCE_DIR}/tests/test_lexer_3.sh)
