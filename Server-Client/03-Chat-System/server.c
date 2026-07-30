#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {

    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    char message[100];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Listen
    listen(server_fd, 5);

    printf("Waiting for client...\n");

    // Accept connection
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);

    printf("Client Connected!\n");

    while (1) {

        // Receive message
        recv(client_fd, message, sizeof(message), 0);

        printf("\nClient : %s\n", message);

        if (strcmp(message, "stop") == 0)
            break;

        // Send reply
        printf("Server : ");
        scanf("%s", message);

        send(client_fd, message, strlen(message) + 1, 0);

        if (strcmp(message, "stop") == 0)
            break;
    }

    close(client_fd);
    close(server_fd);

    return 0;
}