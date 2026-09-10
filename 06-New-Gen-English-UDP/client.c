#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8008
#define BUFFER_SIZE 2048

int main() {

    int sock;
    int n;

    char buffer[BUFFER_SIZE];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    // Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Get sentence from user
    printf("Enter sentence: ");
    fgets(buffer, BUFFER_SIZE, stdin);

    // Remove newline
    buffer[strcspn(buffer, "\n")] = '\0';

    // Send sentence to server
    sendto(
        sock,
        buffer,
        strlen(buffer),
        0,
        (struct sockaddr *)&server,
        len
    );

    // Receive translated sentence
    n = recvfrom(
        sock,
        buffer,
        BUFFER_SIZE - 1,
        0,
        (struct sockaddr *)&server,
        &len
    );

    if (n < 0) {
        perror("recvfrom failed");
        close(sock);
        return 1;
    }

    buffer[n] = '\0';

    printf("Formal English: %s\n", buffer);

    close(sock);

    return 0;
}