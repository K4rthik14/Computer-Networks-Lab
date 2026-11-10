
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, num;
    char buffer[100];

    struct sockaddr_in server;

    // 1. Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 3. Connect to server
    connect(s, (struct sockaddr *)&server, sizeof(server));

    // 4. Read number
    printf("Enter a number: ");
    scanf("%d", &num);

    // 5. Convert integer to string
    snprintf(buffer, sizeof(buffer), "%d", num);

    // 6. Send number to server
    send(s, buffer, strlen(buffer), 0);

    printf("Number sent to server\n");

    close(s);

    return 0;
}
