#include "../inc/stat_analyzer.h"
#include "../inc/ast_printer.h"
#include "stdio.h"
#include "string.h"

static
void collectExprVars(linked_list *vars_list, expression *expr)
{
    if(!expr) {
        return;
    }
    switch(expr->type)
    {
        case (EQ):
        case (ADD):
        case (MUL):
            collectExprVars(vars_list, expr->left);
            collectExprVars(vars_list, expr->right);
            break;
        case (INT):
            break;
        case (VAR): 
            linked_list_add_end(vars_list, expr);
    }
}

static
statement *findVarInitStatement(
    linked_list *stmt_list, linked_list_node *end, linked_list_node **out, char *var_name)
{
    linked_list_node *curr = end->prev;
    void *elem = end->element;

    while(curr)
    {
        statement *stmt = (statement *)curr->element;
        elem = curr->element;

        switch(stmt->type)
        {
            case (ASSIGN): {
                assign *asgn = (assign *)stmt->instruction;
                if (strcmp(var_name, asgn->var_name) == 0) {
                    *out = curr;
                    return stmt;
                }
            }break;
        }
        curr = curr->prev;
    }

    return NULL;
}

static
BOOL checkStatement(linked_list *stmt_list, linked_list_node *end, statement *stmt, linked_list *deps)
{
    BOOL result = FALSE;
    linked_list *vars_list = create_linked_list();
    switch(stmt->type) {
        case ASSIGN: collectExprVars(vars_list, ((assign *)stmt->instruction)->expr); break;
        case IF: collectExprVars(vars_list, ((ifstmt *)stmt->instruction)->cond); break;
    }

    if (!linked_list_is_empty(vars_list)) {
        linked_list_node *curr_var = vars_list->head;
        linked_list_node *new_end = NULL;

        while(curr_var)
        {
            expression *expr = (expression *)curr_var->element;
            statement *init_stmt = findVarInitStatement(stmt_list, end, &new_end, expr->value);
            if (init_stmt) {
                assign *var_assign = (assign *) init_stmt->instruction;
                if (checkStatement(stmt_list, new_end, init_stmt, deps)) {
                    printf("%s -> ", var_assign->var_name);
                    linked_list_add(deps, init_stmt);
                    result = TRUE;
                    break;
                }
            } else {
                printf("--------------------------------------------------\n");
                printf("Variable uninitialized %s : ", expr->value);
                result = TRUE;
                break;
            }
            curr_var = curr_var->next;
        }
    }

    del_linked_list(vars_list);
    return result;
}

void saUninitializedVars(linked_list *stmt_list)
{
    linked_list *dependency = NULL;
    linked_list_node *curr = NULL;
    if (!stmt_list) {
        return;
    }

    printf("\n");
    printf("+-----------------------------------------------------------------------+\n");
    printf("| Checking that no variable is used without first being assigned value. |\n");
    printf("+-----------------------------------------------------------------------+\n");

    curr = stmt_list->head;
    while(curr) 
    {
        statement *stmt = (statement *)curr->element;
        switch(stmt->type)
        {
            case (ASSIGN):
            case (IF):
                break;
            default: curr = curr->next; 
                continue;
        }

        dependency = create_linked_list();

        if (checkStatement(stmt_list, curr, stmt, dependency)) {
            switch(stmt->type) 
            {
                case (ASSIGN): {
                    char *var_name = ((assign *)stmt->instruction)->var_name;
                    printf("%s\n", var_name);
                }break;
                case (IF): {
                    printf("%s\n", "if statement condition");
                }break;
            }
        }

        printAST(dependency);
        del_linked_list(dependency);
        curr = curr->next;
    }
}