// Game logic
// game.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CODE_LENGTH 4

// Function to generate a random secret code
void generateSecretCode(char *secretCode) {
    srand(time(NULL));
    for (int i = 0; i < CODE_LENGTH; i++) {
        secretCode[i] = '1' + rand() % 6;
    }
    secretCode[CODE_LENGTH] = '\0';
}

// Function to compare the guessed code with the secret code and provide feedback
void provideFeedback(const char *secretCode, const char *guess, char *feedback) {
    int exactMatches = 0;
    int colorMatches = 0;

    // Check for exact matches
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (guess[i] == secretCode[i]) {
            exactMatches++;
        }
    }

    // Check for color matches
    int secretCodeFreq[6] = {0};
    int guessFreq[6] = {0};

    for (int i = 0; i < CODE_LENGTH; i++) {
        secretCodeFreq[secretCode[i] - '1']++;
        guessFreq[guess[i] - '1']++;
    }

    for (int i = 0; i < 6; i++) {
        colorMatches += (secretCodeFreq[i] < guessFreq[i]) ? secretCodeFreq[i] : guessFreq[i];
    }

    // Build the feedback string
    sprintf(feedback, "Exact Matches: %d, Color Matches: %d", exactMatches, colorMatches - exactMatches);
}

int main() {
    char secretCode[CODE_LENGTH + 1];
    char guess[CODE_LENGTH + 1];
    char feedback[50];

    // Generate the secret code
    generateSecretCode(secretCode);

    printf("Secret code generated. Let's play!\n");

    // Game loop
    while (1) {
        printf("Enter your guess (4 digits from 1 to 6): ");
        scanf("%s", guess);

        // Validate the guess (You can add your own validation logic here if needed)

        // Provide feedback on the guess
        provideFeedback(secretCode, guess, feedback);
        printf("Feedback: %s\n", feedback);

        // Check if the guess is correct
        if (strcmp(secretCode, guess) == 0) {
            printf("Congratulations! You guessed the correct code.\n");
            break;
        }
    }

    return 0;
}

