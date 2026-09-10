#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {

    int server_fd, client_fd;
    int n;
    int client_no = 0;

    char buffer[BUFFER_SIZE];

    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Allow reuse of port
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen for clients
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("Server started...\n");
    printf("Waiting for clients...\n");

    while (1) {

        // Accept client connection
        client_fd = accept(
            server_fd,
            (struct sockaddr *)&client_addr,
            &client_len
        );

        if (client_fd < 0) {
            perror("Accept failed");
            continue;
        }

        client_no++;

        printf("Client %d connected.\n", client_no);

        // Create a child process for each client
        if (fork() == 0) {

            // Child does not need the listening socket
            close(server_fd);

            while (1) {

                // Receive message from client
                n = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);

                if (n <= 0) {
                    break;
                }

                buffer[n] = '\0';

                printf("\nClient %d: %s", client_no, buffer);

                // Check for exit
                if (strncmp(buffer, "exit", 4) == 0) {
                    break;
                }

                // Get reply from server
                printf("Server: ");
                fgets(buffer, BUFFER_SIZE, stdin);

                // Send reply
                send(client_fd, buffer, strlen(buffer), 0);

                // Check for exit
                if (strncmp(buffer, "exit", 4) == 0) {
                    break;
                }
            }

            close(client_fd);
            exit(0);
        }

        // Parent does not need the connected socket
        close(client_fd);
    }

    close(server_fd);

    return 0;
}