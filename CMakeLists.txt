cmake_minimum_required(VERSION 3.15)
project(matematix C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Og -Wall -Wextra -g)

include_directories(
        src/data_structures
        src/lexing
        src/parsing
)



add_executable(parsing_test.out
        tests/parsing_test.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/parsing/parser.c
        src/data_structures/parsing_node.c
        src/data_structures/operators.c
)

add_executable(lexing_test.out
        tests/lexing_test.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/data_structures/operators.c
)