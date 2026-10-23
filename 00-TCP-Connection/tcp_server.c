
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int s, c, n;
    char buffer[1024] = {0};
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 5);

    printf("TCP server waiting...\n");

    c = accept(s, (struct sockaddr *)&client, &len);

    while (1)
    {
        // 1. Receive message
        n = recv(c, buffer, sizeof(buffer) - 1, 0);

        // 2. Check connection
        if (n <= 0)
            break;

        // 3. Terminate the string
        buffer[n] = '\0';

        printf("Client: %s\n", buffer);

        // 4. Check stop condition
        if (strcmp(buffer, "stop") == 0)
            break;

        // 5. Send reply
        strcpy(buffer, "Hello from server");
        send(c, buffer, strlen(buffer), 0);
    }

    close(c);
    close(s);

    return 0;
}
