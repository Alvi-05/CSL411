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