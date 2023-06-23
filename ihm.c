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
} Player;

void generateCode(int code[]) {
    code = (int *)malloc(CODE_LENGTH * sizeof(int));
    srand(time(NULL));
    for (int i = 0; i < CODE_LENGTH; i++) {
        code[i] = rand() % 6 + 1; // Génère un nombre aléatoire entre 1 et 6 inclus
    }
}

int checkCode(int guess[], int code[]) {
    // La logique de vérification reste inchangée
    // ...
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

        found = checkCode(guess, chooser->code);
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

                // Demander le nom du joueur
                printf("Veuillez entrer votre nom : ");
                scanf("%s", players[playerCount].name);

                playerCount++;

                if (playerCount == MAX_PLAYERS) {
                    for (int i = 0; i < MAX_PLAYERS; i++) {
                        players[i].code = (int *)malloc(CODE_LENGTH * sizeof(int));
                        generateCode(players[i].code);
                    }

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

