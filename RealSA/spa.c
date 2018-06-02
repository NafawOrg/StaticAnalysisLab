/*
 * static program analyizer 101
 */

#include "inc/reg_lang.h"
#include "inc/stat_analyzer.h"
#include "inc/ast_printer.h"
#include "tmp/parser.h"
#include "tmp/lexer.h"

#include <stdio.h>

int yyparse (linked_list **stmt_list, yyscan_t scan);

linked_list *getAST (char *source_code)
{
    linked_list *stmt_list = create_linked_list(); //root of AST change this depending on your AST data structure

    yyscan_t scanner;
    YY_BUFFER_STATE state;

    if (yylex_init(&scanner)) 
    {
        //couldn't initialize
        return NULL;
    }

    state = yy_scan_string(source_code, scanner); //tokenize source_code
    
    if (yyparse(&stmt_list, scanner)) //retreive the AST from parser using tokenize string
    {
        //error parsing not your issue
        return NULL;
    }

    yy_delete_buffer(state, scanner);
    yylex_destroy(scanner);

    return stmt_list; //this should be changed
}

char *textFromFile(char *filename)
{
    FILE *file;
    size_t fileSize;
    char *text;

    file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error reading file %s!\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END); 
    fileSize = ftell(file); // what's the position of the pointer in the end of this stream?
    rewind(file);
    
    text = (char*)malloc(fileSize + 1); //one mor to null terminate the string

    if(!text)
    {
        //failed to allocate text
        return NULL;
    }
    
    if(!fread(text, 1, fileSize, file))
    {
        printf("Error! Unable to read the file %s!", filename);
        free(text);
        return NULL;
    }

    fclose(file);
    
    return text;
}

int main(int argc, char **argv)
{
    char *input;
    
    if(argc < 2)
    {
        printf("Error! a filename to statically analyize is expected!\n");
        exit(1);
    }

    input = textFromFile(argv[1]);
    
    if(!input)
    {	
        printf("No input from file!\n");
        exit(1);
    }

    linked_list *root = getAST(input);
    
    if(!root)
    {
        printf("Error! File does not parse! %s\n", argv[1]);
        exit(1);
    }
    //printAST(root);
    //printf("\n deleting \n\n");
    //linked_list_delete_nth(root, 4);
    printAST(root);
    saUninitializedVars(root);
    //add deletes for all elements
    delete_linked_list(root);	
}

