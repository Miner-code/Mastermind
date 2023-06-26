#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "server.h"

#define MAX_PLAYERS 2
#define CODE_LENGTH 4

typedef struct {
    char name[20];
    int *code;
    int turn;
    int socket;
} Player;

void generateCode(int code[]) {
    code = (int *)malloc(CODE_LENGTH * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < CODE_LENGTH; i++) {
        code[i] = rand() % 6 + 1; // Génère un nombre aléatoire entre 1 et 6 inclus
    }
}

void checkCode(int guess[], int code[], int* correctPosition, int* correctColor) {
    int tempCode[CODE_LENGTH];
    int tempGuess[CODE_LENGTH];
    int tempCorrectPosition = 0;
    int tempCorrectColor = 0;

    memcpy(tempCode, code, CODE_LENGTH * sizeof(int));
    memcpy(tempGuess, guess, CODE_LENGTH * sizeof(int));

    // Vérifier les positions correctes
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (tempGuess[i] == tempCode[i]) {
            tempCorrectPosition++;
            tempCode[i] = 0;
            tempGuess[i] = 0;
        }
    }

    // Vérifier les couleurs correctes à la mauvaise place
    for (int i = 0; i < CODE_LENGTH; i++) {
        if (tempGuess[i] != 0) {
            for (int j = 0; j < CODE_LENGTH; j++) {
                if (tempGuess[i] == tempCode[j]) {
                    tempCorrectColor++;
                    tempCode[j] = 0;
                    break;
                }
            }
        }
    }

    *correctPosition = tempCorrectPosition;
    *correctColor = tempCorrectColor;
}

void playGame(Player *players) {
    Player *guesser = &players[rand() % 2];
    Player *chooser = (guesser == &players[0]) ? &players[1] : &players[0];

    printf("Bienvenue dans le jeu Mastermind !\n");
    printf("%s, vous devez deviner le code de %s.\n", guesser->name, chooser->name);
    printf("Le code est une séquence de 4 chiffres entre 1 et 6 inclus.\n");
    printf("Utilisez N pour indiquer une couleur correcte à la bonne place et B pour indiquer une couleur correcte à la mauvaise place.\n");

    int guess[CODE_LENGTH];
    int attempts = 0;
    int found = 0;

    while (!found && attempts < 10) {
        printf("Saisissez votre proposition : ");
        for (int i = 0; i < CODE_LENGTH; i++) {
            scanf("%d", &guess[i]);
        }

        int correctPosition, correctColor;
        checkCode(guess, chooser->code, &correctPosition, &correctColor);

        found = (correctPosition == CODE_LENGTH);

        printf("Résultat : ");
        for (int i = 0; i < correctPosition; i++) {
            printf("N");
        }
        for (int i = 0; i < correctColor; i++) {
            printf("B");
        }
        printf("\n");

        attempts++;
    }

    if (found) {
        printf("Félicitations, vous avez trouvé le code en %d tentatives !\n", attempts);
    } else {
        printf("Dommage, vous n'avez pas trouvé le code. Le code était : ");
        for (int i = 0; i < CODE_LENGTH; i++) {
            printf("%d ", chooser->code[i]);
        }
        printf("\n");
    }
}

int main() {
    Player players[MAX_PLAYERS];
    int playerCount = 0;

    initServer();

    while (1) {
        // Attente d'un nouveau joueur
        int newPlayer = acceptConnection();

        if (newPlayer != -1) {
            printf("Nouveau joueur connecté !\n");

            if (playerCount < MAX_PLAYERS) {
                printf("En attente d'autres joueurs...\n");

                players[playerCount].turn = 0;
                players[playerCount].socket = newPlayer;

                // Demander le nom du joueur
                printf("Veuillez entrer votre nom : ");
                scanf("%s", players[playerCount].name);

                playerCount++;

                if (playerCount == MAX_PLAYERS) {
                    for (int i = 0; i < MAX_PLAYERS; i++) {
                        players[i].code = (int *)malloc(CODE_LENGTH * sizeof(int));
                        generateCode(players[i].code);
                    }

                    // Envoyer un message d'accueil aux joueurs
                    char welcomeMessage[100];
                    snprintf(welcomeMessage, sizeof(welcomeMessage), "Bienvenue dans le jeu Mastermind ! Les joueurs sont %s et %s.\n", players[0].name, players[1].name);
                    sendMessage(players[0].socket, welcomeMessage);
                    sendMessage(players[1].socket, welcomeMessage);

                    playGame(players);
                    playerCount = 0;

                    // Libérer la mémoire allouée pour les codes des joueurs
                    for (int i = 0; i < MAX_PLAYERS; i++) {
                        free(players[i].code);
                    }
                }
            } else {
                printf("Nombre maximal de joueurs atteint. Veuillez réessayer plus tard.\n");
            }
        }
    }

    return 0;
}
