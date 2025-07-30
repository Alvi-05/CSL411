// Write a YACC program to check the syntax of FOR statement in C


/*
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
%}

%token FOR LPAREN RPAREN LBRACE RBRACE SEMICOLON

%%

stmt: for_stmt
    ;

for_stmt: FOR LPAREN expr SEMICOLON expr SEMICOLON expr RPAREN LBRACE stmt RBRACE
         ;

expr: term
    | expr PLUS term    { $$ = new_binary_expr(PLUS, $1, $3); }
    | expr MINUS term   { $$ = new_binary_expr(MINUS, $1, $3); }
    ;

term: factor
    | term MULTIPLY factor { $$ = new_binary_expr(MULTIPLY, $1, $3); }
    | term DIVIDE factor    { $$ = new_binary_expr(DIVIDE, $1, $3); }
    ;

factor: NUMBER          { $$ = new_number_expr($1); }
      | LPAREN expr RPAREN { $$ = $2; }
      ;

%%

#include "y.tab.h"
#include "ast.h"

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int yylex(void);

int main(void) {
    yyparse();
    return 0;
}
*/