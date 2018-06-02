/*
 * static program analyizer 101
 */

#include "inc/reg_lang.h"
#include "inc/stat_analyzer.h"
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

void printExpression(expression *expr, int indent)
{
    int i = 0;
    
    if(!expr) {
        return;
    }

    for (i = 0; i< indent; i++) {
        printf("\t");
    }
    
    switch(expr->type)
    {
        case (EQ):
            {
                printf("Compare:\n");
                printExpression(expr->left, indent + 1);
                printExpression(expr->right, indent + 1);
            }break;
        case (ADD):
            {
                printf("Add:\n");
                printExpression(expr->left, indent + 1);
                printExpression(expr->right, indent + 1);
            }break;
        case (MUL):
            {
                printf("Multiply:\n");
                printExpression(expr->left, indent + 1);
                printExpression(expr->right, indent + 1);
            }break;
        case (INT): printf("%s int\n", expr->value);
            break;
        case (VAR): printf("%s var\n", expr->value);
            break;
    }
}

void printAssign(assign *asst)
{
    printf("Assignation of variable %s to expression:\n", asst->var_name);
    printExpression(asst->expr, 1);
}

void printIf(ifstmt *ifst)
{
    printf("Conditional branch\n");
    printf("\tCondition:\n");
    printExpression(ifst->cond, 2);
    printf("\tJump to: %s\n", ifst->target);
}

void printGoto(gotostmt *gtst)
{
    printf("Unconditional branch, jump to:%s\n", gtst->target);
}

void printInput(inputstmt *in)
{
    printf("Read to variable %s\n", in->var_name);
}

void printOutput(outputstmt *out)
{
    printf("Write to output variable %s\n", out->var_name);
}

void printLabel(label *lbl)
{
    printf("Label marker=%s\n", lbl);
}

void printAST(linked_list *stmt_list)
{
    linked_list_node *node = NULL;
    if(!stmt_list)
        return;
    
    node = stmt_list->head;

    while(node)
    {
        statement *stmt = (statement*)node->element;
        switch(stmt->type)
        {
            case (ASSIGN): printAssign((assign*)stmt->instruction); break;
            case (IF): printIf((ifstmt*)stmt->instruction); break;
            case (GOTO): printGoto((gotostmt*)stmt->instruction); break;
            case (INPUT): printInput((inputstmt*)stmt->instruction); break;
            case (OUTPUT): printOutput((outputstmt*)stmt->instruction); break;
            case (LABEL): printLabel((label*)stmt->instruction);
        }
        node = node->next;
    }
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

