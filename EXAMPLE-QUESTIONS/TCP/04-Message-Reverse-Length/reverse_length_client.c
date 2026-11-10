
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

    // 1. Create socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        perror("socket");
        exit(1);
    }

    // 2. Configure server address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Connect
    if (connect(s, (struct sockaddr *)&server,
                sizeof(server)) < 0) {
        perror("connect");
        close(s);
        exit(1);
    }

    printf("Connected to server!\n");

    // 4. Communication loop
    while (1)
    {
        printf("Enter message: ");

        if (fgets(buffer, SIZE, stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';

        if (send(s, buffer, strlen(buffer), 0) < 0) {
            perror("send");
            break;
        }

        if (strcmp(buffer, "stop") == 0)
            break;

        memset(buffer, 0, SIZE);

        n = recv(s, buffer, SIZE - 1, 0);

        if (n <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[n] = '\0';

        printf("Server response: %s\n", buffer);
    }

    close(s);

    return 0;
}
