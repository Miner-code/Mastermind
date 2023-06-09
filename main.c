// Main
// main.c

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t pid;

    // Fork a child process for the server
    pid = fork();
    if (pid == 0) {
        // Child process (server)
        execl("./server", "server", NULL);
    } else if (pid > 0) {
        // Parent process

        // Fork another child process for the first player
        pid = fork();
        if (pid == 0) {
            // Child process (player 1)
            execl("./player", "player", NULL);
        } else if (pid > 0) {
            // Parent process

            // Wait for the first player to finish
            wait(NULL);

            // Fork another child process for the second player
            pid = fork();
            if (pid == 0) {
                // Child process (player 2)
                execl("./player", "player", NULL);
            } else if (pid > 0) {
                // Parent process

                // Wait for the second player to finish
                wait(NULL);
            } else {
                perror("Forking player 2 failed");
                exit(EXIT_FAILURE);
            }
        } else {
            perror("Forking player 1 failed");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("Forking server failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}

