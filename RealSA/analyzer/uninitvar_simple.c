#include "../inc/stat_analyzer.h"
#include "../inc/reg_lang.h"
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
assign *findVarInitStatement(
    linked_list *stmt_list, linked_list_node *end, linked_list_node **out, char *var_name)
{
    linked_list_node *curr = end->prev;
    assign *result = NULL;
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
                    result = asgn;
                    *out = curr;
                    return result;
                }
            }break;
        }
        curr = curr->prev;
    }

    return result;
}

// probably better to make double linked list in order to move
// back and forth through the statement list
static
BOOL checkAssignment(linked_list *stmt_list, linked_list_node *end, assign *asgn)
{
    linked_list_node *curr = stmt_list->head;

    linked_list *vars_list = create_linked_list();
    collectExprVars(vars_list, asgn->expr);

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

    printf("\n");
    printf("+-----------------------------------------------------------------------+\n");
    printf("| Checking that no variable is used without first being assigned value. |\n");
    printf("+-----------------------------------------------------------------------+\n");

    node = stmt_list->head;
    while(node) 
    {
        statement *stmt = (statement *)node->element;
        switch(stmt->type) 
        {
            case (ASSIGN): {
                char *var_name = ((assign *)stmt->instruction)->var_name;
                if (checkAssignment(stmt_list, node, (assign *) stmt->instruction)) {
                    printf("%s\n", var_name);
                }
            }break;
            case (IF): {

            }break;
        }
        node = node->next;
    }
}