
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, n;
    char buffer[1024] = {0};
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("UDP server waiting...\n");

    while (1)
    {
        // 1. Receive message
        n = recvfrom(s, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&client, &len);

        // 2. Check receive result
        if (n < 0)
            break;

        // 3. Terminate the string
        buffer[n] = '\0';

        printf("Client: %s\n", buffer);

        // 4. Check stop condition
        if (strcmp(buffer, "stop") == 0)
            break;

        // 5. Prepare and send reply
        strcpy(buffer, "Hello from server");

        sendto(s, buffer, strlen(buffer), 0,
               (struct sockaddr *)&client, len);
    }

    close(s);

    return 0;
}
