#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 10
#define MAX_SYMBOLS 10
#define MAX_TRANSITIONS 50

typedef struct {
    int numStates;
    int numSymbols;
    int transitionTable[MAX_STATES][MAX_SYMBOLS]; // Transition table
    int startState;
    int acceptingStates[MAX_STATES]; // List of accepting states
    int numAcceptingStates;
} FiniteAutomaton;

// Function to find the index of a symbol
int getSymbolIndex(FiniteAutomaton *fa, char symbol) {
    // Check if symbol is within the range of symbols defined
    if (symbol == 'a') return 0;
    if (symbol == 'b') return 1;
    return -1;  // Invalid symbol
}

// Function to initialize the finite automaton
void initializeAutomaton(FiniteAutomaton *fa) {
    printf("Enter number of input symbols: ");
    scanf("%d", &fa->numSymbols);

    char inputSymbols[MAX_SYMBOLS];
    printf("Input symbols: ");
    scanf("%s", inputSymbols);

    printf("Enter number of states: ");
    scanf("%d", &fa->numStates);

    printf("Initial state: ");
    scanf("%d", &fa->startState);

    printf("Number of accepting states: ");
    scanf("%d", &fa->numAcceptingStates);
    printf("Accepting states: ");
    for (int i = 0; i < fa->numAcceptingStates; i++) {
        scanf("%d", &fa->acceptingStates[i]);
    }

    printf("Transition table:\n");
    for (int i = 0; i < fa->numStates; i++) {
        for (int j = 0; j < fa->numSymbols; j++) {
            char symbol;
            if (j == 0) symbol = 'a';
            else if (j == 1) symbol = 'b';
            printf("State %d to %c -> ", i + 1, symbol);
            scanf("%d", &fa->transitionTable[i][j]);
        }
    }
}

// Function to validate a string against the finite automaton
int validateString(FiniteAutomaton *fa, const char *inputString) {
    int currentState = fa->startState;

    // Process each symbol in the input string
    for (int i = 0; i < strlen(inputString); i++) {
        char symbol = inputString[i];
        int symbolIndex = getSymbolIndex(fa, symbol);

        if (symbolIndex == -1) {
            printf("Invalid symbol in string\n");
            return 0; // Invalid symbol
        }

        currentState = fa->transitionTable[currentState - 1][symbolIndex]; // Transition to next state
    }

    // Check if the final state is an accepting state
    for (int i = 0; i < fa->numAcceptingStates; i++) {
        if (currentState == fa->acceptingStates[i]) {
            return 1; // Valid string
        }
    }

    return 0; // Invalid string
}

int main() {
    FiniteAutomaton fa;

    // Initialize the finite automaton
    initializeAutomaton(&fa);

    // Accept the input string to be validated
    char inputString[100];
    printf("Input string: ");
    scanf("%s", inputString);

    // Validate the string
    if (validateString(&fa, inputString)) {
        printf("Valid string\n");
    } else {
        printf("Invalid string\n");
    }

    return 0;
}
