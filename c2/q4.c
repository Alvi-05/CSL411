// Convert the BNF rules into YACC form and write code to generate abstract syntax tree

/*

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
%}

%token NUMBER PLUS MINUS MULTIPLY DIVIDE LPAREN RPAREN

%%

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
// ast.h
#ifndef AST_H
#define AST_H
#include <stdlib.h>
#include <stdio.h>
typedef struct Expr {
    int type; // Type of expression (e.g., NUMBER, BINARY_OP)
    union {
        int value; // For NUMBER
        struct {
            struct Expr *left;
            struct Expr *right;
            int op; // Operator type (e.g., PLUS, MINUS)
        } binary_op; // For binary operations
    } data;
} Expr;
Expr *new_number_expr(int value) {
    Expr *expr = malloc(sizeof(Expr));
    expr->type = 0; // Assuming 0 is the type for NUMBER
    expr->data.value = value;
    return expr;
}
Expr *new_binary_expr(int op, Expr *left, Expr *right) {
    Expr *expr = malloc(sizeof(Expr));
    expr->type = 1; // Assuming 1 is the type for BINARY_OP
    expr->data.binary_op.left = left;
    expr->data.binary_op.right = right;
    expr->data.binary_op.op = op;
    return expr;
}
\end{code}
// y.tab.h
#ifndef Y_TAB_H
#define Y_TAB_H
#define NUMBER 258
#define PLUS 259
#define MINUS 260
#define MULTIPLY 261
#define DIVIDE 262
#define LPAREN 263
#define RPAREN 264
extern int yylex(void);
extern int yyparse(void);
extern void yyerror(const char *s);
#endif /* Y_TAB_H */ /*
// y.tab.c  
#include "y.tab.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
int yylex(void) {
    // Lexer implementation goes here
    return 0; // Placeholder return
}
// y.tab.c
#include "y.tab.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h> 
int yylex(void) {
    // Lexer implementation goes here
    return 0; // Placeholder return
}
// q3.c
#include <stdio.h>
#include <stdlib.h> 
#include "y.tab.h"
#include "ast.h"
\end{code}
// Function to handle errors
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
int main(void) {
    yyparse();
    return 0;
}
*/