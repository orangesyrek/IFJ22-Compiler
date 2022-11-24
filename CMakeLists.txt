cmake_minimum_required(VERSION 2.8.11)
project(IFJ C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -std=c11")

set(RELEASE 0)

if (RELEASE)
set(src
	main.c
	compiler.c
	lexical_analysis.c
	symtab.c
	syntactic_analysis.c
	expression.c
	stack.c
	generator.c
	)
set(header
	compiler.h
	symtab.h
	lexical_analysis.h
	syntactic_analysis.h
	expression.h
	stack.h
	generator.h
	)
add_executable(compiler ${src} ${header})
else()
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
add_executable(compiler ${src} ${header})
#tests
enable_testing()
add_subdirectory(tests)


include_directories(${CMAKE_SOURCE_DIR})

# add shared tests as make ifj_tester command
add_custom_target(ifj_tester COMMAND ${CMAKE_SOURCE_DIR}/tests/test.py
	${CMAKE_CURRENT_BINARY_DIR}/compiler
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)

add_custom_target(ifj_generator COMMAND ${CMAKE_SOURCE_DIR}/tests/test.py
	${CMAKE_CURRENT_BINARY_DIR}/compiler
	${CMAKE_SOURCE_DIR}/../ic22int
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests)
endif()
