// Player code
// player.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024] = {0};
    char guess[4] = {0};

    // Create client socket
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IP address from string to binary format
    if (inet_pton(AF_INET, "127.0.0.1", &(server_address.sin_addr)) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Receive welcome message from the server
    memset(buffer, 0, sizeof(buffer));
    if (read(client_socket, buffer, sizeof(buffer)) < 0) {
        perror("Read failed");
        exit(EXIT_FAILURE);
    }
    printf("%s\n", buffer);

    // Game loop
    while (1) {
        // Player makes a guess
        printf("Enter your guess (4 digits from 1 to 6): ");
        scanf("%s", guess);

        // Send the guess to the server
        if (send(client_socket, guess, strlen(guess), 0) < 0) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        // Receive feedback from the server
        memset(buffer, 0, sizeof(buffer));
        if (read(client_socket, buffer, sizeof(buffer)) < 0) {
            perror("Read failed");
            exit(EXIT_FAILURE);
        }

        // Display the feedback
        printf("Feedback: %s\n", buffer);

        // Check if the player guessed correctly
        if (strcmp(buffer, "Correct") == 0) {
            printf("Congratulations! You guessed the correct code.\n");
            break;
        }
    }

    // Close the connection
    close(client_socket);

    return 0;
}

