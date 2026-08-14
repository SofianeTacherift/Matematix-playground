COMPILATOR="GCC"
lexing_parsing.out: lexe_parse.o lexer.o token.o parser.o
	gcc -Og -Wall -Wextra -g  lexe_parse.o parser.o token.o lexer.o -o lexing_parsing.out.out -lm

lexe_parse.o : lexe_parse.c