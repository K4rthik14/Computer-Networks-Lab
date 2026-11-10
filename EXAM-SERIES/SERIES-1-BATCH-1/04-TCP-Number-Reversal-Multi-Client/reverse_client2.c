
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int s, n;
    char buffer[100], reply[100];

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
    scanf("%99s", buffer);

    // 5. Send number to server
    send(s, buffer, strlen(buffer), 0);

    // 6. Receive reversed number
    n = recv(s, reply, sizeof(reply)-1, 0);

    if(n > 0)
    {
        reply[n] = '\0';
        printf("Reversed number received from other client: %s\n", reply);
    }

    close(s);

    return 0;
}
