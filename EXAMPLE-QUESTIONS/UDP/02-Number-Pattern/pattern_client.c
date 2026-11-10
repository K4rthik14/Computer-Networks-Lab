
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, n;
    char buffer[100];

    struct sockaddr_in server;

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Read limit
    printf("Enter limit: ");
    scanf("%d", &n);

    // 4. Convert integer to string
    snprintf(buffer, sizeof(buffer), "%d", n);

    // 5. Send limit to server
    sendto(s, buffer, strlen(buffer), 0,
           (struct sockaddr *)&server, sizeof(server));

    printf("Limit sent to server\n");

    close(s);

    return 0;
}
