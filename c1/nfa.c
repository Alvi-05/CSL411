#include <stdio.h>
#include <string.h>

#define MAX 20

char states[MAX][3], symbols[MAX][3], transitions[MAX][3][3];
char new_transitions[MAX][MAX][3][3];
char epsilon_closure[MAX][MAX][3];
int n_states, n_symbols, n_transitions;

// Add a state to a closure if not already present
int add_to_closure(char closure[MAX][3], int count, char *state) {
    for (int i = 0; i < count; i++) {
        if (strcmp(closure[i], state) == 0)
            return count; // already present
    }
    strcpy(closure[count], state);
    return count + 1;
}

// Get epsilon closure of a given state
int get_epsilon_closure(char *state, char closure[MAX][3]) {
    int count = 0;
    count = add_to_closure(closure, count, state);

    int changed = 1;
    while (changed) {
        changed = 0;
        for (int i = 0; i < n_transitions; i++) {
            if (strcmp(transitions[i][1], "e") == 0) {
                for (int j = 0; j < count; j++) {
                    if (strcmp(transitions[i][0], closure[j]) == 0) {
                        int before = count;
                        count = add_to_closure(closure, count, transitions[i][2]);
                        if (count > before)
                            changed = 1;
                    }
                }
            }
        }
    }
    return count;
}

// Find next states from a closure on input symbol
int get_move(char closure[MAX][3], char *symbol, char result[MAX][3]) {
    int count = 0;
    for (int i = 0; i < n_transitions; i++) {
        if (strcmp(transitions[i][1], symbol) == 0) {
            for (int j = 0; j < MAX && strlen(closure[j]) > 0; j++) {
                if (strcmp(transitions[i][0], closure[j]) == 0) {
                    count = add_to_closure(result, count, transitions[i][2]);
                }
            }
        }
    }
    return count;
}

void compute_new_transitions() {
    char closure[MAX][3], move_result[MAX][3], new_closure[MAX][3];
    for (int i = 0; i < n_states; i++) {
        int e_count = get_epsilon_closure(states[i], closure);

        for (int s = 0; s < n_symbols; s++) {
            if (strcmp(symbols[s], "e") == 0) continue;

            int move_count = get_move(closure, symbols[s], move_result);

            int final_count = 0;
            for (int m = 0; m < move_count; m++) {
                final_count = get_epsilon_closure(move_result[m], new_closure);

                for (int z = 0; z < final_count; z++) {
                    int slot = 0;
                    while (strlen(new_transitions[i][s][slot]) > 0) slot++;
                    strcpy(new_transitions[i][s][slot], new_closure[z]);
                }
            }
        }
    }
}

void display_new_transitions() {
    printf("\nNew NFA transitions (without epsilon):\n");
    for (int i = 0; i < n_states; i++) {
        for (int j = 0; j < n_symbols; j++) {
            if (strcmp(symbols[j], "e") == 0) continue;
            printf("δ(%s, %s) = { ", states[i], symbols[j]);
            for (int k = 0; k < MAX && strlen(new_transitions[i][j][k]) > 0; k++) {
                printf("%s ", new_transitions[i][j][k]);
            }
            printf("}\n");
        }
    }
}

int main() {
    FILE *fp = fopen("automata.txt", "r");

    printf("Enter number of states: ");
    scanf("%d", &n_states);
    printf("Enter state names:\n");
    for (int i = 0; i < n_states; i++) {
        scanf("%s", states[i]);
    }

    printf("Enter number of input symbols (including 'e'): ");
    scanf("%d", &n_symbols);
    printf("Enter symbols:\n");
    for (int i = 0; i < n_symbols; i++) {
        scanf("%s", symbols[i]);
    }

    printf("Reading transitions from automata.txt...\n");
    n_transitions = 0;
    while (fscanf(fp, "%s %s %s", transitions[n_transitions][0],
                  transitions[n_transitions][1], transitions[n_transitions][2]) != EOF) {
        n_transitions++;
    }

    compute_new_transitions();
    display_new_transitions();

    fclose(fp);
    return 0;
}
/*
Sample input:

q0 e q1
q0 0 q0
q1 1 q2
q2 0 q2

sample result:

Enter number of states: 3
Enter state names:
q0 q1 q2
Enter number of input symbols (including 'e'): 3
Enter symbols:
0 1 e
Reading transitions from automata.txt...

New NFA transitions (without epsilon):
δ(q0, 0) = { q0 }
δ(q0, 1) = { q2 }
δ(q1, 0) = { }
δ(q1, 1) = { q2 }
δ(q2, 0) = { q2 }
δ(q2, 1) = { }

all hail to genAI
*/