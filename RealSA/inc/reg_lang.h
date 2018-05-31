
#include "linked_list.h"
#include <stdlib.h>

/*data structures for register language*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef statement

typedef enum stmt_type
{
    ASSIGN,
    IF,
    GOTO,
    INPUT,
    OUTPUT,
    LABEL
}statement_type;

typedef struct stmt
{
    statement_type type;
    void *instruction;
}statement;

typedef enum express_type
{
    EQ,
    ADD,
    MUL,
    CONS
}expression_type;

typedef struct express
{
    expression_type type; 
    struct express *left;
    struct express *right;
    char *value;
}expression;

typedef struct assign_tag
{
    char *var_name;
    expression *expr;
}assign;

typedef char label;

typedef struct ifstmt_tag
{
    expression *cond;
    char *target;
}ifstmt;

typedef struct gotostmt_tag
{
    char *target;
}gotostmt;

typedef struct in_tag
{
    char *var_name;
}inputstmt;
 
typedef struct out_tag
{
    char *var_name;
}outputstmt;

#endif

/*create a new input*/
inputstmt *create_inputstmt(char *var);

/*delete an input object*/
void delete_inputstmt(inputstmt *in);

/*create a new output*/
outputstmt *create_outputstmt(char *var);

/*delete an output object*/
void delete_outputstmt(outputstmt *out);

/*create a new expression*/
expression *create_expression (expression_type, expression *left, expression *right, char *value);

/*delete an expression, RECURSIVE*/
void delete_expression(expression *e);

/*create a new label */
label *create_label(char *name);

/*delete a label*/
void delete_label(label *l);

/*create a new goto*/
gotostmt *create_gotostmt(char *lbl);

/*delete a goto statement*/
void delete_gotostmt(gotostmt *gtst);

/*create a new assignment */
assign *create_assign(char *var, expression *ex);

/*delete an assignment statement*/
void delete_assign(assign *asst);

/*create a new if statement*/
ifstmt *create_ifstmt (expression *ex, char *lbl);

/*delete an if statement*/
void delete_ifstmt(ifstmt *ifst);

/*create a new statement */
statement *create_statement (statement_type type, void *stmt);

/*delete statement*/
void delete_statement(statement *stmt);

#ifdef __cplusplus
}
#endif