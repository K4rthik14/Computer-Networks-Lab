#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    int sock;
    int n;
    char buffer[BUFFER_SIZE];

    struct sockaddr_in server_addr;

    fd_set readfds;

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(sock,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {

        perror("Connection failed");
        close(sock);
        exit(1);
    }

    printf("Connected to chat server.\n");
    printf("Type 'exit' to disconnect.\n");

    while (1) {

        // Clear set
        FD_ZERO(&readfds);

        // Monitor keyboard
        FD_SET(STDIN_FILENO, &readfds);

        // Monitor server socket
        FD_SET(sock, &readfds);

        // Wait for keyboard or server message
        select(sock + 1, &readfds, NULL, NULL, NULL);

        // Message from server
        if (FD_ISSET(sock, &readfds)) {

            n = recv(
                sock,
                buffer,
                BUFFER_SIZE - 1,
                0
            );

            if (n <= 0) {
                printf("Server disconnected.\n");
                break;
            }

            buffer[n] = '\0';

            printf("\n%s", buffer);
            printf("You: ");
            fflush(stdout);
        }

        // User entered a message
        if (FD_ISSET(STDIN_FILENO, &readfds)) {

            printf("You: ");
            fflush(stdout);

            fgets(buffer, BUFFER_SIZE, stdin);

            // Send message to server
            send(
                sock,
                buffer,
                strlen(buffer),
                0
            );

            // Exit
            if (strncmp(buffer, "exit", 4) == 0) {
                break;
            }
        }
    }

    close(sock);

    return 0;
}
