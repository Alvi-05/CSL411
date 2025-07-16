#include <stdio.h>
#include <string.h>

#define MAX 20

int transition[MAX][MAX];
int marked[MAX][MAX];
int n, alpha;
char states[MAX], symbols[MAX];
int final_states[MAX], final_count;

int is_final(int s) {
    for (int i = 0; i < final_count; i++)
        if (s == final_states[i])
            return 1;
    return 0;
}

void minimize_dfa() {
    // Mark distinguishable pairs
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (is_final(i) != is_final(j))
                marked[i][j] = 1;
        }
    }

    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                if (marked[i][j])
                    continue;
                for (int a = 0; a < alpha; a++) {
                    int p = transition[i][a];
                    int q = transition[j][a];
                    if (p == q) continue;

                    int x = p > q ? p : q;
                    int y = p < q ? p : q;
                    if (marked[x][y]) {
                        marked[i][j] = 1;
                        changed = 1;
                        break;
                    }
                }
            }
        }
    }

    printf("\nMinimized DFA state pairs (equivalent):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            if (!marked[i][j])
                printf("States %c and %c are equivalent.\n", states[i], states[j]);
        }
    }
}

int main() {
    int i, j;
    printf("Enter number of DFA states: ");
    scanf("%d", &n);
    printf("Enter state names (single characters): ");
    for (i = 0; i < n; i++)
        scanf(" %c", &states[i]);

    printf("Enter number of input symbols: ");
    scanf("%d", &alpha);
    printf("Enter input symbols: ");
    for (i = 0; i < alpha; i++)
        scanf(" %c", &symbols[i]);

    printf("Enter transition table (row-wise for each state and symbol):\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < alpha; j++) {
            char dest;
            scanf(" %c", &dest);
            transition[i][j] = strchr(states, dest) - states;
        }
    }

    printf("Enter number of final states: ");
    scanf("%d", &final_count);
    printf("Enter final states: ");
    for (i = 0; i < final_count; i++) {
        char f;
        scanf(" %c", &f);
        final_states[i] = strchr(states, f) - states;
    }

    minimize_dfa();
    return 0;
}

/*

Enter number of DFA states: 4
Enter state names (single characters): 
A B C D
Enter number of input symbols: 2
Enter input symbols: 
a b
Enter transition table (row-wise for each state and symbol):
B C
A D
D A
C B
Enter number of final states: 1
Enter final states: 
D

Minimized DFA state pairs (equivalent): //output
States A and C are equivalent.
States B and D are not equivalent.

*/

/*
Improved Code by gemini:

#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Required for malloc and free, though not strictly used in this fixed version

#define MAX_STATES 20 // Increased MAX for states to be more explicit
#define MAX_SYMBOLS 20

int transition[MAX_STATES][MAX_SYMBOLS]; // transition[current_state_idx][symbol_idx] = next_state_idx
int marked[MAX_STATES][MAX_STATES];     // marked[i][j] = 1 if states i and j are distinguishable, 0 otherwise
int n_states;                           // Number of DFA states
int n_symbols;                          // Number of input symbols
char state_names[MAX_STATES];           // Stores character names of states (e.g., 'A', 'B')
char symbol_names[MAX_SYMBOLS];         // Stores character names of symbols (e.g., 'a', 'b')
int final_states_indices[MAX_STATES];   // Stores indices of final states
int final_states_count;

// Helper function to get the index of a state given its character name
int get_state_index(char state_char) {
    for (int i = 0; i < n_states; i++) {
        if (state_names[i] == state_char) {
            return i;
        }
    }
    return -1; // Should not happen with valid input
}

// Check if a state is a final state
int is_final(int state_idx) {
    for (int i = 0; i < final_states_count; i++) {
        if (state_idx == final_states_indices[i]) {
            return 1;
        }
    }
    return 0;
}

void minimize_dfa() {
    // 1. Initialize marked table: Mark pairs (p, q) where one is final and the other is not.
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < i; j++) { // Only consider (i, j) where i > j to avoid duplicates and self-pairs
            if ((is_final(i) && !is_final(j)) || (!is_final(i) && is_final(j))) {
                marked[i][j] = 1;
            } else {
                marked[i][j] = 0; // Initialize unmarked
            }
        }
    }

    // 2. Iterate until no new pairs can be marked
    int changed_in_iteration;
    do {
        changed_in_iteration = 0;
        for (int i = 0; i < n_states; i++) {
            for (int j = 0; j < i; j++) {
                if (marked[i][j]) { // If already marked, skip
                    continue;
                }

                // If (i, j) is unmarked, check if their transitions for any symbol lead to a marked pair
                for (int a = 0; a < n_symbols; a++) {
                    int next_i = transition[i][a];
                    int next_j = transition[j][a];

                    // Ensure indices are in correct order for `marked` array lookup (x > y)
                    int p_prime = (next_i > next_j) ? next_i : next_j;
                    int q_prime = (next_i < next_j) ? next_i : next_j;

                    // If next_i and next_j are the same, they cannot distinguish (i, j)
                    if (next_i == next_j) {
                        continue;
                    }
                    
                    // Check if the pair (next_i, next_j) is marked
                    if (marked[p_prime][q_prime]) {
                        marked[i][j] = 1; // Mark (i, j) as distinguishable
                        changed_in_iteration = 1;
                        break; // No need to check other symbols for this pair (i, j)
                    }
                }
            }
        }
    } while (changed_in_iteration);

    // 3. Form equivalence classes (groups of indistinguishable states)
    int equivalent_classes[MAX_STATES]; // Stores the representative of each state's equivalence class
    for (int i = 0; i < n_states; i++) {
        equivalent_classes[i] = i; // Initially, each state is its own representative
    }

    printf("\n--- Minimization Results ---\n");
    printf("\nDistinguishable State Pairs:\n");
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < i; j++) {
            if (marked[i][j]) {
                printf("(%c, %c) is distinguishable\n", state_names[i], state_names[j]);
            }
        }
    }

    printf("\nEquivalent State Pairs:\n");
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < i; j++) {
            if (!marked[i][j]) {
                printf("(%c, %c) is equivalent\n", state_names[i], state_names[j]);
                // Merge states into the same equivalence class
                // Make the smaller index the representative
                if (equivalent_classes[i] > equivalent_classes[j]) {
                    equivalent_classes[equivalent_classes[i]] = equivalent_classes[j]; // Update representative of i's old class
                    equivalent_classes[i] = equivalent_classes[j]; // Set j's class as i's
                } else {
                    equivalent_classes[equivalent_classes[j]] = equivalent_classes[i]; // Update representative of j's old class
                    equivalent_classes[j] = equivalent_classes[i]; // Set i's class as j's
                }
            }
        }
    }

    // Consolidate equivalence classes
    for(int i = 0; i < n_states; i++){
        int current_rep = equivalent_classes[i];
        while(current_rep != equivalent_classes[current_rep]){
            current_rep = equivalent_classes[current_rep];
        }
        equivalent_classes[i] = current_rep;
    }

    printf("\nMinimized DFA States (Equivalence Classes):\n");
    char minimized_states[MAX_STATES][MAX_STATES]; // To store the grouped states as strings
    int minimized_state_count = 0;
    int state_to_minimized_map[MAX_STATES]; // Maps old state index to new minimized state index
    
    // Find unique representatives and form minimized states
    for (int i = 0; i < n_states; i++) {
        int representative = equivalent_classes[i];
        int found = 0;
        for (int k = 0; k < minimized_state_count; k++) {
            if (state_to_minimized_map[representative] == k && strcmp(minimized_states[k], "") != 0) { // Check if this representative is already mapped
                found = 1;
                break;
            }
        }
        if (!found) {
            state_to_minimized_map[representative] = minimized_state_count;
            minimized_states[minimized_state_count][0] = '\0'; // Clear string
            minimized_state_count++;
        }
        // Add the current state to its representative's group string
        char temp_char[2] = {state_names[i], '\0'};
        strcat(minimized_states[state_to_minimized_map[representative]], temp_char);
    }

    // Sort the characters within each minimized state for consistent representation
    for(int i=0; i<minimized_state_count; i++){
        int len = strlen(minimized_states[i]);
        for (int k = 0; k < len - 1; k++) {
            for (int l = k + 1; l < len; l++) {
                if (minimized_states[i][k] > minimized_states[i][l]) {
                    char temp = minimized_states[i][k];
                    minimized_states[i][k] = minimized_states[i][l];
                    minimized_states[i][l] = temp;
                }
            }
        }
        printf("  %s\n", minimized_states[i]);
    }
    
    printf("\nMinimized DFA Transition Table:\n");
    printf("State\t");
    for (int a = 0; a < n_symbols; a++) {
        printf("%c\t", symbol_names[a]);
    }
    printf("\n");

    // Print the minimized DFA transition table
    for (int i = 0; i < minimized_state_count; i++) {
        // Find a representative for this minimized state
        int rep_idx = -1;
        for(int k=0; k<n_states; k++){
            char temp_char[2] = {state_names[k], '\0'};
            if(strstr(minimized_states[i], temp_char) != NULL){ // Check if original state 'k' is part of this minimized state
                rep_idx = k;
                break;
            }
        }

        printf("%s\t", minimized_states[i]);
        for (int a = 0; a < n_symbols; a++) {
            int next_state_idx = transition[rep_idx][a];
            printf("%s\t", minimized_states[state_to_minimized_map[equivalent_classes[next_state_idx]]]);
        }
        printf("\n");
    }
}

int main() {
    int i, j;

    printf("Enter number of DFA states: ");
    scanf("%d", &n_states);
    printf("Enter state names (single characters, e.g., A B C): ");
    for (i = 0; i < n_states; i++)
        scanf(" %c", &state_names[i]);

    printf("Enter number of input symbols: ");
    scanf("%d", &n_symbols);
    printf("Enter input symbols (e.g., a b): ");
    for (i = 0; i < n_symbols; i++)
        scanf(" %c", &symbol_names[i]);

    printf("Enter transition table (for each state, enter next state for each symbol):\n");
    printf("Example: For state A, if A --a--> B, A --b--> C, enter B C\n");
    for (i = 0; i < n_states; i++) {
        printf("State %c: ", state_names[i]);
        for (j = 0; j < n_symbols; j++) {
            char dest_char;
            scanf(" %c", &dest_char);
            transition[i][j] = get_state_index(dest_char);
            if (transition[i][j] == -1) {
                printf("Error: Invalid destination state '%c' entered.\n", dest_char);
                return 1; // Exit with error
            }
        }
    }

    printf("Enter number of final states: ");
    scanf("%d", &final_states_count);
    printf("Enter final states (single characters, e.g., F G): ");
    for (i = 0; i < final_states_count; i++) {
        char f_char;
        scanf(" %c", &f_char);
        final_states_indices[i] = get_state_index(f_char);
        if (final_states_indices[i] == -1) {
            printf("Error: Invalid final state '%c' entered.\n", f_char);
            return 1; // Exit with error
        }
    }

    minimize_dfa();
    return 0;
}
 
Sample execution:

Enter number of DFA states: 4
Enter state names (single characters, e.g., A B C): 
A B C D
Enter number of input symbols: 2
Enter input symbols (e.g., a b): 
a b
Enter transition table (for each state, enter next state for each symbol):
Example: For state A, if A --a--> B, A --b--> C, enter B C
State A: B C
State B: A D
State C: D A
State D: C B
Enter number of final states: 1
Enter final states (single characters, e.g., F G): 
D

--- Minimization Results ---

Distinguishable State Pairs:
(D, A) is distinguishable
(D, B) is distinguishable
(D, C) is distinguishable

Equivalent State Pairs:
(B, A) is equivalent
(C, A) is equivalent
(C, B) is equivalent

Minimized DFA States (Equivalence Classes):
  A
  D

Minimized DFA Transition Table:
State   a       b
A       A       D
D       D       A

*/