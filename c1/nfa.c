#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For exit()

#define MAX 20

char states[MAX][3];
char symbols[MAX][3];
char transitions[MAX][3][3];
char new_transitions[MAX][MAX][MAX][3]; // Increased dimension for multiple target states
char epsilon_closure[MAX][MAX][3];
int n_states, n_symbols, n_transitions;

// Helper function to initialize a 2D char array with empty strings
void init_2d_char_array(char arr[][3], int rows) {
    for (int i = 0; i < rows; i++) {
        arr[i][0] = '\0'; // Set first character to null terminator
    }
}

// Add a state to a closure if not already present
// Returns the new count of elements in the closure
int add_to_closure(char closure[MAX][3], int count, char *state) {
    for (int i = 0; i < count; i++) {
        if (strcmp(closure[i], state) == 0)
            return count; // already present
    }
    if (count < MAX) { // Prevent buffer overflow
        strcpy(closure[count], state);
        return count + 1;
    }
    return count; // Should not happen if MAX is sufficiently large
}

// Get epsilon closure of a given state
// Returns the count of elements in the closure
int epsilon(char *state, char closure[MAX][3]) {
    // Clear the closure array before computing for a new state
    init_2d_char_array(closure, MAX);
    int count = 0;
    count = add_to_closure(closure, count, state);

    int changed = 1;
    while (changed) {
        changed = 0;
        // Iterate through the current elements in the closure
        for (int j = 0; j < count; j++) {
            for (int i = 0; i < n_transitions; i++) {
                if (strcmp(transitions[i][0], closure[j]) == 0 && strcmp(transitions[i][1], "e") == 0) {
                    int before = count;
                    count = add_to_closure(closure, count, transitions[i][2]);
                    if (count > before) {
                        changed = 1;
                    }
                }
            }
        }
    }
    return count;
}

// Find next states from a closure on input symbol
// Returns the count of elements in the result set
int move(char closure[MAX][3], int closure_count, char *symbol, char result[MAX][3]) {
    // Clear the result array before computing for a new move
    init_2d_char_array(result, MAX);
    int count = 0;
    for (int j = 0; j < closure_count; j++) { // Iterate through states in the closure
        for (int i = 0; i < n_transitions; i++) {
            if (strcmp(transitions[i][0], closure[j]) == 0 && strcmp(transitions[i][1], symbol) == 0) {
                count = add_to_closure(result, count, transitions[i][2]);
            }
        }
    }
    return count;
}

void compute() {
    char current_epsilon_closure[MAX][3];
    char move_result_states[MAX][3];
    char final_epsilon_closure[MAX][3];

    // Initialize new_transitions array
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            for (int k = 0; k < MAX; k++) {
                new_transitions[i][j][k][0] = '\0';
            }
        }
    }

    for (int i = 0; i < n_states; i++) {
        // Step 1: Find epsilon-closure of the current state
        int e_closure_count = epsilon(states[i], current_epsilon_closure);

        for (int s = 0; s < n_symbols; s++) {
            if (strcmp(symbols[s], "e") == 0) continue; // Skip epsilon symbol

            // Step 2: Find states reachable from the epsilon-closure on the current symbol
            int move_count = move(current_epsilon_closure, e_closure_count, symbols[s], move_result_states);

            // Step 3: Find epsilon-closure of each state in the move result and union them
            // This loop needs to aggregate all final states into new_transitions[i][s]
            char combined_final_states[MAX][3];
            int combined_final_count = 0;
            init_2d_char_array(combined_final_states, MAX); // Clear for each (state, symbol) pair

            for (int m = 0; m < move_count; m++) {
                int final_e_closure_count = epsilon(move_result_states[m], final_epsilon_closure);

                for (int z = 0; z < final_e_closure_count; z++) {
                    combined_final_count = add_to_closure(combined_final_states, combined_final_count, final_epsilon_closure[z]);
                }
            }

            // Store the combined final states in new_transitions
            for (int k = 0; k < combined_final_count; k++) {
                strcpy(new_transitions[i][s][k], combined_final_states[k]);
            }
        }
    }
}

void display() {
    printf("\n--- New NFA transitions (without epsilon) ---\n");
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < n_symbols; j++) {
            if (strcmp(symbols[j], "e") == 0) continue; // Skip epsilon symbol in display
            printf("d(%s, %s) = { ", states[i], symbols[j]);
            // Iterate through the stored new transitions for state i and symbol j
            int first_state_printed = 0;
            for (int k = 0; k < MAX && strlen(new_transitions[i][j][k]) > 0; k++) {
                if (first_state_printed) {
                    printf(", "); // Add comma for readability between states
                }
                printf("%s", new_transitions[i][j][k]);
                first_state_printed = 1;
            }
            printf(" }\n");
        }
    }
    printf("------------------------------------------\n");
}

int main() {
    FILE *fp = fopen("automata.txt", "r");
    if (fp == NULL) {
        perror("Error opening automata.txt");
        return 1;
    }

    printf("Enter number of states: ");
    scanf("%d", &n_states);
    printf("Enter states (e.g., q0 q1 q2):\n");
    for (int i = 0; i < n_states; i++) {
        scanf("%s", states[i]);
    }

    printf("Enter number of input symbols (including 'e'): ");
    scanf("%d", &n_symbols);
    printf("Enter symbols (e.g., 0 1 e):\n");
    for (int i = 0; i < n_symbols; i++) {
        scanf("%s", symbols[i]);
    }

    printf("Reading transitions from automata.txt...\n");
    n_transitions = 0;
    while (fscanf(fp, "%s %s %s", transitions[n_transitions][0], transitions[n_transitions][1], transitions[n_transitions][2]) == 3) {
        n_transitions++;
        if (n_transitions >= MAX) {
            printf("Warning: Maximum number of transitions (%d) reached. Some transitions might be ignored.\n", MAX);
            break;
        }
    }

    compute();
    display();

    fclose(fp);
    return 0;
}

/*
output:

Enter number of states: 3
Enter states (e.g., q0 q1 q2):
q0
q1
q2
Enter number of input symbols (including 'e'): 3
Enter symbols (e.g., 0 1 e):
0 1 e
Reading transitions from automata.txt...

--- New NFA transitions (without epsilon) ---
d(q0, 0) = { q0, q1, q2 }
d(q0, 1) = { q1, q2 }
d(q1, 0) = {  }
d(q1, 1) = { q2 }
d(q2, 0) = {  }
d(q2, 1) = {  }
------------------------------------------

*/
