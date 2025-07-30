//	Implementation of Calculator using LEX and YACC

/*
// lex file
%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

digit   [0-9]
number  {digit}+(\.{digit}+)?

%%

{number}    {
                yylval = atof(yytext);
                return NUMBER;
            }
[ \t]       ; // Ignore whitespace
\n          return '\n';
"+"         return '+';
"-"         return '-';
"*"         return '*';
"/"         return '/';
"("         return '(';
")"         return ')';
.           { printf("Unexpected character: %s\n", yytext); exit(1); }

%%

int yywrap() {
    return 1;
}

// yacc file
%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);

#define YYSTYPE double
double result;
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%right UMINUS

%%

calculation:
    expression '\n' { result = $1; printf("Result = %g\n", result); }
    ;

expression:
      expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { 
        if ($3 == 0) {
            yyerror("division by zero");
            YYABORT;
        } else {
            $$ = $1 / $3;
        }
      }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    printf("Enter expressions to calculate, one per line.\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to exit.\n");
    while (yyparse() == 0)
        ;
    return 0;
}

*/


/*

#include <stdio.h>
#include <stdlib.h>

/* Definitions for YACC */
#define YYSTYPE double

/* Forward declarations */
int yylex(void);
int yyparse(void);
void yyerror(const char *s);

/* Global variable to hold the result */
double result;

/* YACC grammar rules and actions */
%{
#include <stdio.h>
#include <stdlib.h>
#define YYSTYPE double
void yyerror(const char *s);
int yylex(void);
double result;
%}

%token NUMBER

%left '+' '-'
%left '*' '/'
%right UMINUS

%%

calculation:
    expression '\n' { result = $1; printf("Result = %g\n", result); }
    ;

expression:
      expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression { 
        if ($3 == 0) {
            yyerror("division by zero");
            YYABORT;
        } else {
            $$ = $1 / $3;
        }
      }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')' { $$ = $2; }
    | NUMBER { $$ = $1; }
    ;

%%

/* LEX definitions */
#include <ctype.h>

int yylex(void) {
    int c;

    /* Skip whitespace */
    while ((c = getchar()) == ' ' || c == '\t')
        ;

    if (c == EOF)
        return 0;

    if (isdigit(c) || c == '.') {
        ungetc(c, stdin);
        if (scanf("%lf", &yylval) != 1)
            return 0;
        return NUMBER;
    }

    if (c == '\n')
        return c;

    /* Return character as token */
    return c;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    printf("Enter expressions to calculate, one per line.\n");
    printf("Press Ctrl+D (Unix) or Ctrl+Z (Windows) to exit.\n");
    while (yyparse() == 0)
        ;
    return 0;
}
*/
