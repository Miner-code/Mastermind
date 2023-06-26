#ifndef SERVER_H
#define SERVER_H

void initServer();
int acceptConnection();
void sendMessage(int socket, const char *message);
void receiveMessage(int socket, char *buffer);

#endif

