// Write a YACC program to recognize a valid variable which starts with a letter followed by any number of letters or digits.


/* 
%{
#include <stdio.h>
%} 
%token LETTER DIGIT
%%
variable: LETTER (LETTER | DIGIT)* {
    printf("Valid variable: %s\n", yytext);
}
          | LETTER {
              printf("Valid variable: %s\n", yytext);
          }
          ;
%%
int main(void) {
    printf("Enter a variable: ");
    yyparse();
    return 0;
}
%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
int yylex(void);
void yyerror(const char *s) {   
    fprintf(stderr, "Error: %s\n", s);
}
%}
%{
int yylex(void) {
    char c = getchar();
    if (isalpha(c)) {
        yylval = c;
        return LETTER;
    } else if (isdigit(c)) {
        yylval = c;
        return DIGIT;
    } else if (c == EOF) {
        return 0; // End of input
    } else {
        return c; // Return the character as is
    }
}       
%}
*/

/*
// var.l
%{
#include "y.tab.h"
%}

%%

[a-zA-Z][a-zA-Z0-9]*   { yylval.str = strdup(yytext); return IDENTIFIER; }
.                      { return yytext[0]; }
\n                     { return 0; }

%%

// var.y
%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void yyerror(const char *s);
int yylex(void);
%}

%union {
    char *str;
}

%token <str> IDENTIFIER

%%

input:
    IDENTIFIER    { printf("Valid variable name: %s\n", $1); free($1); }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main() {
    printf("Enter a variable name:\n");
    return yyparse();
}

//bash
yacc -d yacc.y
lex lex.l
gcc y.tab.c lex.yy.c -o parser -ll
./parser
*/
