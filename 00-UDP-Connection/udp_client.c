
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, n;
    char buffer[1024] = {0};
    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    while (1)
    {
        // 1. Read message
        printf("Enter message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // 2. Send message
        sendto(s, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server, len);

        // 3. Check stop condition
        if (strcmp(buffer, "stop") == 0)
            break;

        // 4. Receive reply
        n = recvfrom(s, buffer, sizeof(buffer) - 1, 0,
                     NULL, NULL);

        // 5. Check receive result
        if (n < 0)
            break;

        // 6. Terminate and display reply
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    close(s);

    return 0;
}
