
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, n, count;
    char buffer[200], countbuf[20], reply[200];

    struct sockaddr_in server;
    socklen_t len = sizeof(server);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Read message
    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    // 4. Send message to server
    sendto(s, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server, sizeof(server));

    // 5. Receive occurrence count
    n = recvfrom(s, countbuf, sizeof(countbuf)-1, 0,
                 NULL, NULL);

    if(n <= 0)
    {
        close(s);
        return 1;
    }

    countbuf[n] = '\0';
    count = atoi(countbuf);

    printf("\nNumber of occurrences: %d\n", count);

    // 6. Receive reply count times
    for(int i = 0; i < count; i++)
    {
        n = recvfrom(s, reply, sizeof(reply)-1, 0,
                     NULL, NULL);

        if(n <= 0)
            break;

        reply[n] = '\0';

        printf("Server: %s\n", reply);
    }

    close(s);

    return 0;
}
