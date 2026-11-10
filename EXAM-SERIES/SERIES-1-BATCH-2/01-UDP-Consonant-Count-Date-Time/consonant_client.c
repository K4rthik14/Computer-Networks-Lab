
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SIZE 1024

int main()
{
    int s, n;

    char buffer[SIZE];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    if (s < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP socket created successfully\n");

    // 2. Configure server address
    memset(&server, 0, sizeof(server));

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1",
                  &server.sin_addr) != 1)
    {
        perror("Invalid server IP address");
        close(s);
        exit(EXIT_FAILURE);
    }

    // 3. Read message
    printf("Enter a message: ");

    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        printf("Input failed\n");
        close(s);
        exit(EXIT_FAILURE);
    }

    buffer[strcspn(buffer, "\n")] = '\0';

    // 4. Send message to server
    int sent = sendto(s, buffer, strlen(buffer), 0,
                      (struct sockaddr *)&server, len);

    if (sent < 0)
    {
        perror("Send failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    // 5. Receive server response
    n = recvfrom(s, buffer, SIZE - 1, 0,
                 NULL, NULL);

    if (n < 0)
    {
        perror("Receive failed");
        close(s);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';

    // 6. Display response
    printf("\nServer response: %s\n", buffer);

    // 7. Close socket
    close(s);

    return 0;
}
