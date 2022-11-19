cmake_minimum_required(VERSION 2.8.11)
project(IFJ C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -std=c11")

set(src
	src/main.c
	src/compiler.c
	src/lexical_analysis.c
	src/symtab.c
	src/syntactic_analysis.c
	src/expression.c
	src/stack.c
	src/generator.c
	)
set(header
	src/compiler.h
	src/symtab.h
	src/lexical_analysis.h
	src/syntactic_analysis.h
	src/expression.h
	src/stack.h
	src/generator.h
	)

#tests
enable_testing()
add_subdirectory(tests)

add_executable(compiler ${src} ${header})

include_directories(${CMAKE_SOURCE_DIR})

# add shared tests as make ifj_tester command
add_custom_target(ifj_tester COMMAND ${CMAKE_SOURCE_DIR}/tests/test.py
	${CMAKE_CURRENT_BINARY_DIR}/compiler
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
