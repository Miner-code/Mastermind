// Server code
// server.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int server_socket, new_socket, player1_socket, player2_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *welcome_message = "Welcome to the Mastermind game!";

    // Create server socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 2) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    // Accept player 1 connection
    if ((player1_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Player 1 accept failed");
        exit(EXIT_FAILURE);
    }
    send(player1_socket, welcome_message, strlen(welcome_message), 0);

    // Accept player 2 connection
    if ((player2_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Player 2 accept failed");
        exit(EXIT_FAILURE);
    }
    send(player2_socket, welcome_message, strlen(welcome_message), 0);

    printf("Both players connected. Game started!\n");

    // Game logic and data exchange between players can be implemented here

    close(player1_socket);
    close(player2_socket);
    close(server_socket);

    return 0;
}

