cmake_minimum_required(VERSION 2.8.11)
project(IFJ C)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=gnu99")
set(src
	src/main.c
	src/compiler.c
	src/lexical_analysis.c
	src/symtab.c)
set(header
	src/compiler.h
	src/symtab.h
	src/lexical_analysis.h)

add_executable(interpret ${src} ${header})

include_directories(${CMAKE_SOURCE_DIR})
