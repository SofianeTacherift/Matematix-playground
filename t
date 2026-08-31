* f098555 Add power operator and not operator ( '^', '!')
* 26c22c2 Fix an bug which caused a double free and amelioration of the error management when there is multiple errors
| * 8c592e7 Change of the error system : only the scopes parsing function decide until which token it skip when an error occur
| * e4edf3f Modification : parser now store every errors instead of just printing them
|/  
* b4a2627 added better error management
* 51fa094 Refactor
* 9baf508 Bug fix
* e315310 parsing clean up
* 0d1a4eb Add operator struct to token and parsing_node
*   b2b1a2a reset main to parser-cleanup content
|\  
* | 5662cb1 Reorganization of project structure, there is now a Makefile for lexing and parsing tests
| | * c8e176b WIP on parser-cleanup: e72bdb7 parsing can now parses if statements ( if, elif, else )
| |/| 
| | * db505a1 index on parser-cleanup: e72bdb7 parsing can now parses if statements ( if, elif, else )
| |/  
| * e72bdb7 parsing can now parses if statements ( if, elif, else )
| * eeb8d6b better implementation of scoping, added scop imbrication
| * 2070f27 first implementation of scope in parsing and parsing output is now readable easily
| * 9ddc04f struct node is now called parsing_node and now has its own .h and .c in ./data_structures
| * 4b910e4 better organization of the project direction and there is now a makefile to compile lexing and parsing test
|/  
* c6a48c1 parser can now parse instructions with comparaisons and logical operators
* ae7d96a lexer can now lex instructions with equality ('=='), greater than ('>'), less than ('<'), greater than or equal to ('>='), less than or equal to ('<='), and ('&') and  or ('|').
