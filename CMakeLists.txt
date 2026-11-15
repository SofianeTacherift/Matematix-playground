cmake_minimum_required(VERSION 3.15)
project(matematix C)

set(CMAKE_C_STANDARD 11)


option(SANITIZE_ADRESS "active adress sanitizer" FALSE)


add_compile_options(-Og -Wall -Wextra -g )

if( SANITIZE_ADRESS)
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address)
endif( SANITIZE_ADRESS)


include_directories(
        src/data_structures
        src/lexing
        src/parsing
        src/compilation
        src/execution
        lib
)



add_executable(parsing_test
        tests/parsing_test.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/parsing/parser.c
        src/data_structures/parsing_node.c
        src/data_structures/operators.c
        src/data_structures/parsing_node_linked_list.c
)

add_executable(lexing_test
        tests/lexing_test.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/data_structures/operators.c
)

add_executable(compilation_test
        tests/compilation_test.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/parsing/parser.c
        src/data_structures/parsing_node.c
        src/data_structures/operators.c
        src/compilation/compiler.c
        src/compilation/instruction.c
        src/compilation/instruction_block.c
        src/compilation/parsing_node_utils.c
        src/compilation/parsing_node_utils.h
        src/data_structures/parsing_node_linked_list.c
        src/data_structures/parsing_node_linked_list.h
)

add_executable(matpx
        src/compilation/matpx.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/parsing/parser.c
        src/data_structures/parsing_node.c
        src/data_structures/operators.c
        src/compilation/compiler.c
        src/compilation/instruction.c
        src/compilation/instruction_block.c
        src/compilation/parsing_node_utils.c
        src/compilation/parsing_node_utils.h
        src/data_structures/parsing_node_linked_list.c
        src/data_structures/parsing_node_linked_list.h
)

add_executable(execution_context_test
        tests/byte_code_read_test.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/parsing/parser.c
        src/data_structures/parsing_node.c
        src/data_structures/parsing_node_linked_list.c
        src/compilation/parsing_node_utils.c
        src/data_structures/operators.c
        src/compilation/compiler.c
        src/compilation/instruction.c
        src/compilation/instruction_block.c
        src/execution/execution_context.c
        src/execution/matx_object.c
        src/execution/matx_object.h
        src/execution/matpx_object_operations.c
        src/execution/matpx_object_operations.h
        src/execution/execution_context.h
        src/execution/execution.c
        src/execution/instruction_execution.c
        src/execution/instruction_execution.h
        src/execution/instruction_execution.c
)

add_executable(execution_test
        tests/execution_test.c
        src/lexing/lexer.c
        src/data_structures/token.c
        src/parsing/parser.c
        src/data_structures/parsing_node.c
        src/data_structures/parsing_node_linked_list.c
        src/compilation/parsing_node_utils.c
        src/data_structures/operators.c
        src/compilation/compiler.c
        src/compilation/instruction.c
        src/compilation/instruction_block.c
        src/execution/execution_context.c
        src/execution/matx_object.c
        src/execution/matx_object.h
        src/execution/matpx_object_operations.c
        src/execution/matpx_object_operations.h
        src/execution/execution_context.h
        src/execution/execution.c
        src/execution/instruction_execution.c
        src/execution/instruction_execution.h
        src/execution/instruction_execution.c
)