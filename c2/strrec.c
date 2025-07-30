%{
#include <stdio.h>
#include <string.h>

char substring[5]; // 4 letters plus null terminator
%}

%option noyywrap

%%
[a-zA-Z]+ {
    if (strstr(yytext, substring) == NULL) { //use strcasestr for case insensitive
        printf("%s\n", yytext);
    }
}
.      ;
\n     ;
%%

int main() {
    extern FILE *yyin;
    char name[100];

    printf("Enter your name: ");
    scanf("%99s", name); // Read name from user
    // Take the first 4 letters, pad with \0 if fewer
    strncpy(substring, name, 4); // Copy up to 4 characters. at most n bytes of src are copied
    substring[4] = '\0';

    yyin = fopen("input.txt", "r");
    if (!yyin) {
        printf("Could not open input.txt\n");
        return 1;
    }
    yylex();
    fclose(yyin);

    return 0;
}

//The program is case-sensitive. If you want case-insensitive matching, you can convert both yytext and substring to lower-case before comparing.
//Only whole words of alphabetic characters (no symbols/numbers) are recognized and checked.