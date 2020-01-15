cmake_minimum_required(VERSION 2.6)

project(find_project C)

add_executable(find src/find.c include/find.h src/kmp.c include/kmp.h src/struct.c include/struct.h src/test.c include/test.h)

