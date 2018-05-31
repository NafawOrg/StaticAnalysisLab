MAIN=spa
STRUCTS=inc/linked_list.h inc/linked_list_node.h inc/reg_lang.h
FILES=linked_list.c tmp/lexer.c reg_lang.c tmp/parser.c

lexer: lexer.l $(STRUCTS)
	lex lexer.l

parser: lexer parser.y tmp/lexer.c $(STRUCTS)
	bison parser.y

c: parser $(MAIN).c
	gcc -O0 -g -o $(MAIN) $(FILES) $(MAIN).c

cpp: parser $(MAIN).cpp
	g++ -Wno-write-strings -o $(MAIN) $(FILES) $(MAIN).cpp
clean: 
	rm -rf tmp/* $(MAIN)

all: c

redo: clean c
