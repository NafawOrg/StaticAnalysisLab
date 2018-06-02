#include "reg_lang.h"
#include "stdio.h"

#ifndef AST_PRINTER_H
#define AST_PRINTER_H

void printExpression(expression *expr, int indent);

void printAssign(assign *asst);

void printIf(ifstmt *ifst);

void printGoto(gotostmt *gtst);

void printInput(inputstmt *in);

void printOutput(outputstmt *out);

void printLabel(label *lbl);

void printAST(linked_list *stmt_list);

#endif