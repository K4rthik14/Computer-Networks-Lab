#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {

    int client_fd;
    struct sockaddr_in server_addr;

    char buffer[100];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // Send message
    strcpy(buffer, "Hello Server!");
    send(client_fd, buffer, strlen(buffer) + 1, 0);

    // Receive reply
    recv(client_fd, buffer, sizeof(buffer), 0);

    printf("Server Reply: %s\n", buffer);

    close(client_fd);

    return 0;
}