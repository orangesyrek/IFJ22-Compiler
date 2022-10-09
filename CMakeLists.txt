cmake_minimum_required(VERSION 2.8.11)
project(IFJ C)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -std=gnu99")
set(src
	main.c
	compiler.c
	lexical_analysis.c
	symtab.c)
set(header
	compiler.h
	symtab.h
	lexical_analysis.h)

add_executable(interpret ${src} ${header})

include_directories(${CMAKE_SOURCE_DIR})
