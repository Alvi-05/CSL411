#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

const char *input;
int pos = 0;
char lookahead;

void error(const char *msg) {
    printf("❌ Error: %s at position %d\n", msg, pos);
    exit(1);
}

void advance() {
    lookahead = input[pos++];
}

void match(char expected) {
    if (lookahead == expected) {
        advance();
    } else {
        char msg[50];
        sprintf(msg, "Expected '%c', found '%c'", expected, lookahead);
        error(msg);
    }
}

// Forward declarations with return type int for evaluation
int E();
int E_ (int inherited);
int T();
int T_ (int inherited);
int F();

// Grammar rules:
// E  → T E'
// E' → + T E' | ε
// T  → F T'
// T' → * F T' | ε
// F  → ( E ) | digit

// E() parses T() and then E_(), passing the computed value along
int E() {
    int t_val = T();
    return E_(t_val);
}

// E_'s inherited value is the result computed so far from T()
int E_(int inherited) {
    if (lookahead == '+') {
        match('+');
        int t_val = T();
        int result = inherited + t_val;
        return E_(result);
    } else {
        // epsilon: no more additions
        return inherited;
    }
}

int T() {
    int f_val = F();
    return T_(f_val);
}

int T_(int inherited) {
    if (lookahead == '*') {
        match('*');
        int f_val = F();
        int result = inherited * f_val;
        return T_(result);
    } else {
        // epsilon: no more multiplications
        return inherited;
    }
}

int F() {
    if (isdigit(lookahead)) {
        int val = lookahead - '0';  // convert char digit to int
        match(lookahead);
        return val;
    } else if (lookahead == '(') {
        match('(');
        int val = E();
        match(')');
        return val;
    } else {
        error("Unexpected symbol in F()");
        return 0;  // never reached, but silences compiler warning
    }
}

int main() {
    char buffer[100];
    printf("Enter an  arithmetic expression: ");
    scanf("%s", buffer);

    input = buffer;
    pos = 0;
    advance();  // Load first character

    int result = E();  // Start parsing and evaluating

    if (lookahead == '\0') {
        printf("✅ Parsing successful! Result = %d\n", result);
    } else {
        error("Unexpected input after parsing");
    }

    return 0;
}

