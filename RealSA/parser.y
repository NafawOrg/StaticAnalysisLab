%{

#include "../inc/reg_lang.h"
#include "parser.h"
#include "lexer.h"

int yyerror(linked_list **stmt_list, yyscan_t scanner, const char *msg) 
{
	printf("Unrecognized structure, message: %s\n", msg);
}

%}


%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

%output "tmp/parser.c"
%defines "tmp/parser.h"

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { linked_list **stmt_list }
%parse-param { yyscan_t scanner }

%union 
{
	char *str;
	inputstmt *in;
	outputstmt *out;	
	expression *expr;
	label *lbl;
	gotostmt *gtst;
	ifstmt *ifst;
	assign *asst;
	statement *stmt; 
}

%token _ASSIGN
%token _PLUS
%token _TIMES
%token _EQUALS
%token _NUM
%token _SC
%token _COL
%token _GOTO
%token _INPUT
%token _OUTPUT
%token _IF
%token _LPAREN
%token _RPAREN
%token _IDEN

%type <stmt> statement_
%type <asst> assignation
%type <ifst> if_statement
%type <gtst> goto_statement
%type <in> input_
%type <out> output_
%type <lbl> label_
%type <expr> expression term operand literal
%type <str> _NUM
%type <str> _IDEN

%%

start
    : statement_list
    ;

statement_list
    : statement_ statement_list {linked_list_add_front(*stmt_list, $1);}
    | statement_ {linked_list_add_front(*stmt_list, $1);}
    ;

statement_
    : assignation _SC {$$ = create_statement(ASSIGN, $1);}
    | if_statement _SC {$$ = create_statement(IF, $1);}
    | goto_statement _SC {$$ = create_statement(GOTO, $1);}
    | input_ _SC {$$ = create_statement(INPUT, $1);} 
    | output_ _SC {$$= create_statement(OUTPUT, $1);}
    | label_ _COL {$$ = create_statement(LABEL, $1);}
    ; 

assignation
    : _IDEN _ASSIGN expression {$$ = create_assign($1, $3); free($1);} 
    ;

expression
    : term _EQUALS expression {$$ = create_expression(EQ, $1, $3, NULL );}
    | term {$$ = $1;}
    ;

term
    : operand _TIMES term {$$ = create_expression(MUL, $1, $3, NULL);} 
    | operand {$$ = $1;}
    ;

operand
    : literal _PLUS operand {$$ = create_expression(ADD, $1, $3, NULL);}
    | literal {$$ = $1;}
    ;

literal
    : _IDEN {$$ = create_expression(CONS, NULL, NULL, $1); free($1);}
    | _NUM {$$ = create_expression(CONS, NULL, NULL, $1); free($1);}
    | _LPAREN expression _RPAREN {$$ = $2;}
    ;

if_statement
    : _IF expression _GOTO _IDEN {$$ = create_ifstmt($2, $4); free($4);}
    ;

goto_statement
    : _GOTO _IDEN {$$ = create_gotostmt($2); free($2);}
    ;

input_
    : _INPUT _IDEN {$$ = create_inputstmt($2); free($2);}
    ;

output_
    : _OUTPUT _IDEN {$$ = create_outputstmt($2); free($2);}
    ;

label_
    : _IDEN {$$ = create_label($1); free($1);}
    ;

%%

