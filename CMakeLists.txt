cmake_minimum_required(VERSION 3.15)
project(matematix C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Og -Wall -Wextra -g)

include_directories(
        data_structures
        lexing
        parsing
)



add_executable(parsing_test.out
        tests/parsing_test.c
        lexing/lexer.c
        data_structures/token.c
        parsing/parser.c
        data_structures/parsing_node.c
        data_structures/operators.c
)

add_executable(lexing_test.out
        tests/lexing_test.c
        lexing/lexer.c
        data_structures/token.c
        data_structures/operators.c
)