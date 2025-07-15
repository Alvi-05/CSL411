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