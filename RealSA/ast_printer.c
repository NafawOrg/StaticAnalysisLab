
#include "inc/ast_printer.h"

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