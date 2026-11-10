
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, n, limit;
    char buffer[100];

    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    bind(s, (struct sockaddr *)&server, sizeof(server));

    printf("Server waiting...\n");

    // 4. Receive limit from client
    n = recvfrom(s, buffer, sizeof(buffer)-1, 0,
                 (struct sockaddr *)&client, &len);

    if(n <= 0)
    {
        close(s);
        return 1;
    }

    buffer[n] = '\0';

    limit = atoi(buffer);

    printf("Limit received: %d\n", limit);

    // 5. Print Egyptian pyramid
    for(int i = 0; i < limit; i++)
    {
        // Print leading spaces
        for(int k = 0; k < limit-i-1; k++)
        {
            printf("  ");
        }

        // Increasing powers of 2
        for(int j = 0; j <= i; j++)
        {
            printf("%d ", 1 << j);
        }

        // Decreasing powers of 2
        for(int j = i-1; j >= 0; j--)
        {
            printf("%d ", 1 << j);
        }

        printf("\n");
    }

    close(s);

    return 0;
}
