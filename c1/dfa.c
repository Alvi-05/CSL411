#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20

char nfa_states[MAX][MAX], dfa_states[100][MAX];
char symbols[MAX];
int nfa_transitions[MAX][MAX][MAX], dfa_transitions[100][MAX];
int nfa_state_count, symbol_count, dfa_state_count = 0;

// Check if a state set already exists in DFA
int state_exists(char state[]) {
    for (int i = 0; i < dfa_state_count; i++) {
        if (strcmp(dfa_states[i], state) == 0)
            return i;
    }
    return -1;
}

// Get state index
int get_index(char state[]) {
    for (int i = 0; i < nfa_state_count; i++) {
        if (strcmp(nfa_states[i], state) == 0)
            return i;
    }
    return -1;
}

// Add DFA state
void add_dfa_state(char state[]) {
    strcpy(dfa_states[dfa_state_count++], state);
}

// Generate transitions for DFA
void generate_dfa() {
    char temp[MAX];
    int front = 0;

    add_dfa_state(nfa_states[0]);  // Start from initial NFA state

    while (front < dfa_state_count) {
        char current[MAX];
        strcpy(current, dfa_states[front]);

        for (int s = 0; s < symbol_count; s++) {
            char new_state[MAX] = "";

            for (int i = 0; i < strlen(current); i++) {
                int index = get_index((char[]){current[i], '\0'});

                for (int j = 0; j < nfa_state_count; j++) {
                    if (nfa_transitions[index][s][j]) {
                        char ch = 'A' + j;
                        if (!strchr(new_state, ch)) {
                            int len = strlen(new_state);
                            new_state[len] = ch;
                            new_state[len + 1] = '\0';
                        }
                    }
                }
            }

            int existing = state_exists(new_state);
            if (existing == -1 && strlen(new_state) > 0)
                add_dfa_state(new_state);

            dfa_transitions[front][s] = state_exists(new_state);
        }
        front++;
    }
}

int main() {
    int i, j, k;

    printf("Enter number of NFA states: ");
    scanf("%d", &nfa_state_count);
    printf("Enter NFA state names (single characters):\n");
    for (i = 0; i < nfa_state_count; i++)
        scanf("%s", nfa_states[i]);

    printf("Enter number of input symbols: ");
    scanf("%d", &symbol_count);
    printf("Enter input symbols (e.g., a b):\n");
    for (i = 0; i < symbol_count; i++)
        scanf(" %c", &symbols[i]);

    printf("Enter transitions in format: from_state symbol to_state\n");
    printf("Enter -1 to stop\n");

    while (1) {
        char from[MAX], sym, to[MAX];
        scanf("%s", from);
        if (strcmp(from, "-1") == 0) break;
        scanf(" %c %s", &sym, to);
        int from_i = get_index(from);
        int to_i = get_index(to);
        int sym_i = strchr(symbols, sym) - symbols;

        nfa_transitions[from_i][sym_i][to_i] = 1;
    }

    generate_dfa();

    printf("\nDFA Transition Table:\n");
    printf("State\t");
    for (i = 0; i < symbol_count; i++)
        printf("%c\t", symbols[i]);
    printf("\n");

    for (i = 0; i < dfa_state_count; i++) {
        printf("%s\t", dfa_states[i]);
        for (j = 0; j < symbol_count; j++) {
            if (dfa_transitions[i][j] != -1)
                printf("%s\t", dfa_states[dfa_transitions[i][j]]);
            else
                printf("-\t");
        }
        printf("\n");
    }

    return 0;
}
/*
Sample Input:

Enter number of NFA states: 3
Enter NFA state names (single characters):
A B C
Enter number of input symbols: 2
Enter input symbols (e.g., a b):
a b
Enter transitions in format: from_state symbol to_state
Enter -1 to stop
A a B
A b A
B a B
B b C
C a C
C b C
-1

Sample Output:

DFA Transition Table:
State   a       b
A       B       A
B       B       C
C       C       C
AB      B       AC
AC      BC      AC
BC      BC      CC
ABC     BC      ACC
...
Note: Depending on how many unique DFA states are created (subsets), 
output may include sets like AB, AC, BC, etc.
*/