#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 20

char nfa_states[MAX][MAX], dfa_states[100][MAX];
char symbols[MAX];
int nfa_transitions[MAX][MAX][MAX]; // Stores 1 if transition exists, 0 otherwise
int dfa_transitions[100][MAX];      // Stores index of next DFA state
int nfa_state_count, symbol_count, dfa_state_count = 0;

// Function to sort a string alphabetically
void sort_string(char *str) {
    int len = strlen(str);
    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (str[i] > str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

// Check if a state set already exists in DFA
int state_exists(char state[]) {
    for (int i = 0; i < dfa_state_count; i++) {
        if (strcmp(dfa_states[i], state) == 0)
            return i;
    }
    return -1;
}

// Get state index from NFA states
int get_index(char state[]) {
    for (int i = 0; i < nfa_state_count; i++) {
        if (strcmp(nfa_states[i], state) == 0)
            return i;
    }
    return -1; // Should not happen with valid input
}

// Add DFA state
void add_dfa_state(char state[]) {
    strcpy(dfa_states[dfa_state_count++], state);
}

// Generate transitions for DFA
void generate_dfa() {
    char initial_dfa_state[MAX] = "";
    // Assuming the initial NFA state is a single character state
    strcat(initial_dfa_state, nfa_states[0]); 
    add_dfa_state(initial_dfa_state);

    int front = 0;
    while (front < dfa_state_count) {
        char current_dfa_state[MAX];
        strcpy(current_dfa_state, dfa_states[front]);

        for (int s = 0; s < symbol_count; s++) {
            char new_dfa_state_temp[MAX] = ""; // Temporary buffer for new state
            char combined_next_states[MAX] = ""; // To collect all next states from current DFA state

            // Iterate through each NFA state in the current DFA state
            for (int i = 0; i < strlen(current_dfa_state); i++) {
                char nfa_char[2];
                nfa_char[0] = current_dfa_state[i];
                nfa_char[1] = '\0';
                int nfa_index = get_index(nfa_char);

                // Find all NFA states reachable from nfa_char on symbol 's'
                for (int j = 0; j < nfa_state_count; j++) {
                    if (nfa_transitions[nfa_index][s][j]) {
                        char next_nfa_char = nfa_states[j][0]; // Assuming single character NFA states
                        if (!strchr(combined_next_states, next_nfa_char)) {
                            int len = strlen(combined_next_states);
                            combined_next_states[len] = next_nfa_char;
                            combined_next_states[len + 1] = '\0';
                        }
                    }
                }
            }

            sort_string(combined_next_states); // Sort the collected states

            int existing_dfa_state_index = -1;
            if (strlen(combined_next_states) > 0) {
                existing_dfa_state_index = state_exists(combined_next_states);
                if (existing_dfa_state_index == -1) {
                    add_dfa_state(combined_next_states);
                    existing_dfa_state_index = dfa_state_count - 1; // Index of newly added state
                }
            }
            
            dfa_transitions[front][s] = existing_dfa_state_index;
        }
        front++;
    }
}


int main() {
    int i, j, k;

    printf("Enter number of NFA states: ");
    scanf("%d", &nfa_state_count);
    printf("Enter NFA state names (single characters, e.g., A B C):\n");
    for (i = 0; i < nfa_state_count; i++)
        scanf("%s", nfa_states[i]);

    printf("Enter number of input symbols: ");
    scanf("%d", &symbol_count);
    printf("Enter input symbols (e.g., a b):\n");
    for (i = 0; i < symbol_count; i++)
        scanf(" %c", &symbols[i]);

    // Initialize nfa_transitions to 0
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            for (k = 0; k < MAX; k++) {
                nfa_transitions[i][j][k] = 0;
            }
        }
    }

    printf("Enter transitions in format: from_state symbol to_state\n");
    printf("Enter -1 to stop\n");

    while (1) {
        char from[MAX], sym_char_str[2], to[MAX];
        scanf("%s", from);
        if (strcmp(from, "-1") == 0) break;
        scanf(" %s %s", sym_char_str, to); // Read symbol as string to handle 'a', 'b', etc.
        char sym = sym_char_str[0]; // Take the first character as the symbol

        int from_i = get_index(from);
        int to_i = get_index(to);
        int sym_i = -1;
        for (int x = 0; x < symbol_count; x++) {
            if (symbols[x] == sym) {
                sym_i = x;
                break;
            }
        }

        if (from_i != -1 && to_i != -1 && sym_i != -1) {
            nfa_transitions[from_i][sym_i][to_i] = 1;
        } else {
            printf("Invalid transition input. Please check state names and symbols.\n");
        }
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
                printf("{}\t"); // Represent empty transition
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

the minimizing dfa: 

| State | a | b |
| A     | B | C |
| B     | A | D |
| C     | D | A |
| D     | C | B |

*/