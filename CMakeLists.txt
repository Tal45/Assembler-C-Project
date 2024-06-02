cmake_minimum_required(VERSION 3.27)
project(Assembler_C_Project C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Assembler_C_Project
        assembler.c
        assembler.h
        pre_asm.c)
