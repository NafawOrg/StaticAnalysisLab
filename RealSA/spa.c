/*
 * static program analyizer 101
 */

#include "inc/reg_lang.h"
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
        case (INT):
            printf("%s int\n", expr->value); break;
        case (VAR): 
            printf("%s var\n", expr->value);
    }
}

void printDeclaration(declaration *decl)
{
    printf("Declaration of variable %s\n", decl->var_name);
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
            case (DECLAR): printDeclaration((declaration*)stmt->instruction); break;
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

void collectVars(linked_list *vars_list, expression *expr)
{
    if(!expr) {
        return;
    }
    switch(expr->type)
    {
        case (EQ):
        case (ADD):
        case (MUL):
            collectVars(vars_list, expr->left);
            collectVars(vars_list, expr->right);
            break;
        case (INT):
            break;
        case (VAR): 
            linked_list_add_end(vars_list, expr);
    }
}

assign *findVarInitStatement(
    linked_list *stmt_list, linked_list_node *end, linked_list_node **out, char *var_name)
{
    linked_list_node *curr = stmt_list->head;
    assign *result = NULL;
    void *elem = end->element;

    //while(curr != end)
    // todo: remove kostyl => double linked list
    while(curr = linked_list_get_prev(stmt_list, elem))
    {
        statement *stmt = (statement *)curr->element;
        elem = curr->element;//

        switch(stmt->type)
        {
            case (ASSIGN): {
                assign *asgn = (assign *)stmt->instruction;
                if (strcmp(var_name, asgn->var_name) == 0) {
                    result = asgn;
                    *out = curr;
                    return result;
                }
            }break;
        }
        //curr = curr->next;
    }

    return result;
}

// probably better to make double linked list in order to move
// back and forth through the statement list
BOOL checkAssignment(linked_list *stmt_list, linked_list_node *end, assign *asgn)
{
    linked_list_node *curr = stmt_list->head;

    linked_list *vars_list = create_linked_list();
    collectVars(vars_list, asgn->expr);

    if (linked_list_is_empty(vars_list)) {
        return FALSE;
    }

    linked_list_node *curr_var = vars_list->head;
    linked_list_node *new_end = NULL;

    while(curr_var)
    {
        expression *expr = (expression *)curr_var->element;
        assign *var_assign = findVarInitStatement(stmt_list, end, &new_end, expr->value);
        if (var_assign) {
            if (checkAssignment(stmt_list, new_end, var_assign)) {
                printf("%s -> ", var_assign->var_name);
                return TRUE;
            }
        } else {
            printf("Variable uninitialized %s -> ", expr->value);
            return TRUE;
        }
        curr_var = curr_var->next;
    }

    return FALSE;
}

void saUninitializedVars(linked_list *stmt_list)
{
    linked_list_node *node = NULL;
    if (!stmt_list) {
        return;
    }

    node = stmt_list->head;

    while(node) 
    {
        statement *stmt = (statement *)node->element;
        switch(stmt->type) 
        {
            // for simplicity checking only assignments todo IF stmt...
            case (ASSIGN): {
                char *var_name = ((assign *)stmt->instruction)->var_name;
                if (checkAssignment(stmt_list, node, (assign *) stmt->instruction)) {
                    printf("%s\n", var_name);
                }
            }break;
            default: break;
        }
        node = node->next;
    }
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
    
    printAST(root);
    saUninitializedVars(root);
    //add deletes for all elements
    delete_linked_list(root);	
}

