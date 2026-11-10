
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
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    if (s < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    if (bind(s, (struct sockaddr *)&server,
             sizeof(server)) < 0) {
        perror("bind");
        close(s);
        exit(1);
    }

    printf("UDP server waiting...\n");

    while (1)
    {
        memset(buffer, 0, SIZE);

        n = recvfrom(s, buffer, SIZE - 1, 0,
                     (struct sockaddr *)&client, &len);

        if (n < 0) {
            perror("recvfrom");
            break;
        }

        buffer[n] = '\0';

        if (strcmp(buffer, "exit") == 0) {
            printf("Client exited.\n");
            break;
        }

        printf("Client: %s\n", buffer);

        strcpy(buffer, "HELLO");

        if (sendto(s, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&client, len) < 0) {
            perror("sendto");
            break;
        }
    }

    close(s);
    return 0;
}
