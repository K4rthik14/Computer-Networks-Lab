
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
    int s, c, n;
    char buffer[SIZE], response[SIZE];
    struct sockaddr_in server;

    // 1. Create socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0) {
        perror("socket");
        exit(1);
    }

    // 2. Configure address
    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    if (bind(s, (struct sockaddr *)&server,
             sizeof(server)) < 0) {
        perror("bind");
        close(s);
        exit(1);
    }

    // 4. Listen
    if (listen(s, 5) < 0) {
        perror("listen");
        close(s);
        exit(1);
    }

    printf("Server waiting...\n");

    // 5. Accept
    c = accept(s, NULL, NULL);

    if (c < 0) {
        perror("accept");
        close(s);
        exit(1);
    }

    // 6. Communication loop
    while (1)
    {
        memset(buffer, 0, SIZE);

        n = recv(c, buffer, SIZE - 1, 0);

        if (n <= 0)
            break;

        buffer[n] = '\0';

        if (strcmp(buffer, "stop") == 0) {
            printf("Client stopped.\n");
            break;
        }

        int len = strlen(buffer);

        printf("Received: %s\n", buffer);

        if (len % 2 != 0)
        {
            // Reverse message
            for (int i = 0; i < len; i++)
                response[i] = buffer[len - 1 - i];

            response[len] = '\0';
        }
        else
        {
            // Send length
            snprintf(response, SIZE, "%d", len);
        }

        if (send(c, response, strlen(response), 0) < 0) {
            perror("send");
            break;
        }
    }

    close(c);
    close(s);

    return 0;
}
