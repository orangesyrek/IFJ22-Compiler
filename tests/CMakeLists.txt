
set(tests test_lexer test_parser)

set(tests_src
	../src/compiler.c
	../src/lexical_analysis.c
	../src/symtab.c
	../src/syntactic_analysis.c
	)
set(tests_header
	../src/compiler.h
	../src/symtab.h
	../src/lexical_analysis.h
	../src/syntactic_analysis.h
	)

foreach(test_name IN LISTS tests)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_1.sh
	"#!/bin/bash\n"
	"${CMAKE_CURRENT_BINARY_DIR}/${test_name} < ${CMAKE_SOURCE_DIR}/tests/ex1.php")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_2.sh
	"#!/bin/bash\n"
	"${CMAKE_CURRENT_BINARY_DIR}/${test_name} < ${CMAKE_SOURCE_DIR}/tests/ex2.php")

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_3.sh
	"#!/bin/bash\n"
	"${CMAKE_CURRENT_BINARY_DIR}/${test_name} < ${CMAKE_SOURCE_DIR}/tests/ex3.php")

add_executable(${test_name} ${test_name}.c ${tests_src} ${tests_header})
add_test(${test_name}_ex_1 ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_1.sh)
add_test(${test_name}_ex_2 ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_2.sh)
add_test(${test_name}_ex_3 ${CMAKE_CURRENT_BINARY_DIR}/${test_name}_3.sh)

endforeach()
