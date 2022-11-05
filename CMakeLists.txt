cmake_minimum_required(VERSION 2.8.11)
project(IFJ C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wextra -std=c11")

set(src
	src/main.c
	src/compiler.c
	src/lexical_analysis.c
	src/symtab.c
	src/stack.c)
set(header
	src/compiler.h
	src/symtab.h
	src/lexical_analysis.h
	src/stack.h)

enable_testing()
add_subdirectory(tests)

add_executable(interpret ${src} ${header})

include_directories(${CMAKE_SOURCE_DIR})
