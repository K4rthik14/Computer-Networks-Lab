#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {

    int client_fd;
    struct sockaddr_in server_addr;

    int a, b;
    char buffer[100];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect
    connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    printf("Enter first number : ");
    scanf("%d", &a);

    printf("Enter second number: ");
    scanf("%d", &b);

    // Convert numbers to string
    sprintf(buffer, "%d %d", a, b);

    // Send to server
    send(client_fd, buffer, strlen(buffer) + 1, 0);

    // Receive result
    recv(client_fd, buffer, sizeof(buffer), 0);

    printf("Sum received from server = %s\n", buffer);

    close(client_fd);

    return 0;
}