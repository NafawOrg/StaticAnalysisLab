#include "inc/reg_lang.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

/*create a new input statement*/
inputstmt *create_inputstmt(char *var)
{
    inputstmt *in = (inputstmt*)malloc (sizeof(inputstmt));
    if(!in) {
        return NULL;
    }
    if(var)
    {
        char *varname = (char*)malloc((strlen(var) + 1) * sizeof(char));
        strcpy(varname, var);
        in->var_name = varname;
    }
    return in;
}

/*delete an input statement object*/
void delete_inputstmt(inputstmt *in)
{
    if(!in) {
        return;
    }
    if(in->var_name) {
        free (in->var_name);
    }
    free (in);
}

/*create a new output statement*/
outputstmt *create_outputstmt(char *var)
{
    outputstmt *out= (outputstmt*)malloc (sizeof(outputstmt));
    if(!out) {
        return NULL;
    }
    if(var)
    {
        char *varname = (char*)malloc((strlen(var) + 1) * sizeof(char));
        strcpy(varname, var);
        out->var_name = varname;
    }
    return out;
}

/*delete an output statement object*/
void delete_outputstmt(outputstmt *out)
{
    if(!out) {
        return;
    }
    if(out->var_name) {
        free (out->var_name);
    }
    free (out);
}

/*create a new expression*/
expression *create_expression (expression_type type, expression *left, expression *right, char *value)
{
    expression *expr = (expression*)malloc(sizeof(expression));
    char *val = NULL;
    
    if(!expr) {
        return NULL;
    }
    if(value) {
        val = (char*)malloc((strlen(value) + 1) * sizeof(char));
        strcpy(val, value);
        expr->value = val; 
    }
    
    expr->type = type;
    expr->left = left;
    expr->right = right; 
    
    return expr;
}

/*delete an expression, RECURSIVE!*/
void delete_expression(expression *e)
{
    if(!e) {
        return;
    }
    if(e->value) {
        free(e->value);
    }
    if(e->left) {
        delete_expression(e->left);
    }
    if(e->right) {
        delete_expression(e->right);
    }
    free(e);
}

/*create a new label */
label *create_label(char *name)
{
    label *l = NULL;
    if(name) {
        l = (label*)malloc(sizeof(label)*(1+strlen(name)));
    }
    if(!l) {
        return NULL;
    }
    strcpy(l, name);
    return l;
}

/*delete a label*/
void delete_label(label *l)
{
    free(l);
}

/*create a new goto*/
gotostmt *create_gotostmt(char *lbl)
{
    gotostmt *gtst = (gotostmt*)malloc(sizeof(gotostmt));
    char *target = NULL;
    if(!gtst) {
        return NULL;
    }
    if(lbl) {
         target = (char*)malloc(sizeof(char)*(strlen(lbl)+1));
        strcpy(target, lbl);
        gtst->target = target;
    }
    
    return gtst;
}

/*delete a goto statement*/
void delete_gotostmt(gotostmt *gtst)
{
    if(!gtst)
        return;
    
    if(gtst->target)
        free(gtst->target);
    
    free(gtst);
}


/*create a new assignment */
assign *create_assign(char *var, expression *ex)
{
    assign *asst = (assign*)malloc(sizeof(assign));
    char *var_name = NULL; 
    
    if(!asst) {
        return NULL;
    }
    if(var)
    {
        var_name = (char*)malloc(sizeof(char)*(strlen(var)+1));
        strcpy(var_name, var);	
        asst->var_name = var_name;
    }
    
    asst->expr = ex;
    
    return asst;
}

/*delete an assignment statement*/
void delete_assign(assign *asst)
{
    if(!asst) {
        return;
    }
    if(asst->var_name) {
        free(asst->var_name);
    }
    if(asst->expr) {
        delete_expression(asst->expr);
    }
    free(asst);
}

/*create a new if statement*/
ifstmt *create_ifstmt (expression *ex, char *lbl)
{
    ifstmt *ifst = (ifstmt*)malloc(sizeof(ifstmt));
    char *target = (char*)malloc(sizeof(char)*(strlen(lbl)+1));
    
    if(!ifst) {
        return NULL;
    }
    
    ifst->cond = ex; // if this is null will this be an inconditional jump? 
    strcpy(target, lbl);
    ifst->target = target;
    
    return ifst;
}

/*delete an if statement*/
void delete_ifstmt(ifstmt *ifst)
{
    if(!ifst) {
        return;
    }
    
    if(ifst->target) {
        free(ifst->target);
    }
    
    delete_expression(ifst->cond);
    free(ifst);	
}

statement *create_statement(statement_type type, void *stmt)
{
    statement *_stmt = (statement*) malloc(sizeof(statement));
    _stmt->type = type;
    _stmt->instruction = stmt;
}

void delete_statement(statement *stmt)
{
    switch(stmt->type)
    {
        case ASSIGN: delete_assign((assign*)stmt->instruction); break;
        case IF: delete_ifstmt((ifstmt*)stmt->instruction); break;
        case GOTO: delete_gotostmt((gotostmt*)stmt->instruction); break;
        case INPUT: delete_inputstmt((inputstmt*)stmt->instruction); break;
        case OUTPUT: delete_outputstmt((outputstmt*)stmt->instruction); break;
        case LABEL: free(stmt->instruction);
    }
    free (stmt);
}

#ifdef __cplusplus
}
#endif

